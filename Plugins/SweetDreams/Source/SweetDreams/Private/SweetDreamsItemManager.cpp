// Fill out your copyright notice in the Description page of Project Settings.

#include "SweetDreamsItemManager.h"

TArray<USweetDreamsItem*> ASweetDreamsItemManager::AllItems;

void ASweetDreamsItemManager::BeginPlay()
{
    InitializeItems();
    Super::BeginPlay();
}

void ASweetDreamsItemManager::InitializeItems()
{
    AllItems.Empty();
    for (TSubclassOf<USweetDreamsItem> Item : ItemClasses)
    {
        USweetDreamsItem* NewItem = NewObject<USweetDreamsItem>(this, Item);
        if (NewItem)
        {
            AllItems.Add(NewItem);
        }
    }
}

bool ASweetDreamsItemManager::AddItem(AActor* NewOwner, TArray<USweetDreamsItem*>& Inventory, USweetDreamsItem*& AddedItem, int32 ItemID)
{
    UWorld* World = GEngine->GetWorldFromContextObject(NewOwner, EGetWorldErrorMode::ReturnNull);
    if (!ensureAlwaysMsgf(IsValid(NewOwner), TEXT("World Context was not valid.")))
    {
        return false;
    }
    USweetDreamsItem* NewItem;
    if (!NewOwner || !FindItem(NewItem, ItemID))
    {
        return false;
    }
    if (Inventory.Contains(NewItem) && !NewItem->bIsStackable && NewItem->Amount > 0)
    {
        int32 UniqueID = FMath::RandRange(1, MAX_int32);
        while (AllItems.ContainsByPredicate([UniqueID](const USweetDreamsItem* Item) { return Item->ItemID == UniqueID; }))
        {
            UniqueID = FMath::RandRange(1, MAX_int32);
        }
        NewItem->ItemID = UniqueID;
    }
    if (NewItem)
    {
        NewItem->SetItemOwner(NewOwner);
        NewItem->Amount++;
        Inventory.Add(NewItem);
        AddedItem = NewItem;
        if (!ensureAlwaysMsgf(IsValid(AddedItem), TEXT("Added Item not valid.")))
        {
            return false;
        }
        if (World) AddedItem->OnAddItem();
        return true;
    }
    return false;
}

bool ASweetDreamsItemManager::FindItem(USweetDreamsItem*& FoundItem, int32 ItemID)
{
    return FindItemOnInventory(AllItems, FoundItem, ItemID);
}

bool ASweetDreamsItemManager::FindItemOnInventory(const TArray<USweetDreamsItem*>& Inventory, USweetDreamsItem*& FoundItem, int32 ItemID)
{
    FoundItem = nullptr;
	if (Inventory.Num() <= 0) return false;
	for (const USweetDreamsItem* Item : Inventory)
	{
		if (Item->ItemID == ItemID)
		{
			FoundItem = const_cast<USweetDreamsItem*>(Item);
			return true;
		}
	}
	return false;
}

void ASweetDreamsItemManager::RemoveItem(TArray<USweetDreamsItem*>& Inventory, USweetDreamsItem* Item)
{
    if (!Item || Inventory == AllItems) return;
    Inventory.Remove(Item);
    Item->SetItemOwner(nullptr);
}

bool ASweetDreamsItemManager::UseItem(TArray<USweetDreamsItem*>& Inventory, USweetDreamsItem* Item)
{
    if (Item && Item->bIsUsable && Item->Amount > 0)
    {
        Item->OnUseItem();
        if (Item->bConsumeWhenUse)
        {
            Item->Amount--;
            if (Item->Amount <= 0)
            {
                RemoveItem(Inventory, Item);
            }
        }
        return true;
    }
    return false;
}

void USweetDreamsItem::SetItemOwner(AActor* NewOwner)
{
    if (!NewOwner) return;
    ItemOwner = NewOwner;
}
