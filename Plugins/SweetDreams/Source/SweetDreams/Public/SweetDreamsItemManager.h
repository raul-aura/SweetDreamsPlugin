// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "Engine/ObjectLibrary.h"
#include "SweetDreamsItemManager.generated.h"

UCLASS(Blueprintable)
class SWEETDREAMS_API ASweetDreamsItemManager : public AInfo
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContextObject="NewOwner"))
	static bool AddItem(AActor* NewOwner, UPARAM(ref)TArray<USweetDreamsItem*>& Inventory, USweetDreamsItem*& AddedItem, int32 ItemID);
	UFUNCTION(BlueprintPure)
	static bool FindItem(USweetDreamsItem*& FoundItem, int32 ItemID);
	UFUNCTION(BlueprintPure)
	static bool FindItemOnInventory(const TArray<USweetDreamsItem*>& Inventory, USweetDreamsItem*& FoundItem, int32 ItemID);
	UFUNCTION(BlueprintCallable)
	static bool UseItem(TArray<USweetDreamsItem*>& Inventory, USweetDreamsItem* Item);
	UFUNCTION(BlueprintCallable)
	static void RemoveItem(TArray<USweetDreamsItem*>& Inventory, USweetDreamsItem* Item);

	UFUNCTION(BlueprintPure)
	static TArray<USweetDreamsItem*> GetAllItems() { return AllItems; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void InitializeItems();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (DisplayName = "Items"))
	TArray<TSubclassOf<USweetDreamsItem>> ItemClasses;

public:
	static TArray<USweetDreamsItem*> AllItems;
};

UCLASS(Blueprintable, BlueprintType)
class SWEETDREAMS_API USweetDreamsItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, SaveGame)
	int32 ItemID = 0;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, SaveGame)
	FText ItemName = FText::FromString("Item");
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, SaveGame)
	FText ItemDescription = FText::FromString("This is an item description.");
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, SaveGame)
	int32 Amount = 0;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, SaveGame)
	bool bIsStackable = true;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, SaveGame)
	bool bIsUsable = true;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, SaveGame)
	bool bConsumeWhenUse = true;

	bool operator==(const USweetDreamsItem& Other) const
	{
		return IsItemEqual(Other);
	}

	bool IsItemEqual(const USweetDreamsItem& Other) const
	{
		return ItemID == Other.ItemID;
	}

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnAddItem();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnRemoveItem();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnUseItem();

	UFUNCTION(BlueprintCallable)
	void SetItemOwner(AActor* NewOwner);
	UFUNCTION(BlueprintCallable)
	AActor* GetItemOwner() const { return ItemOwner; }

	UPROPERTY(BlueprintReadOnly)
	AActor* ItemOwner;
};
