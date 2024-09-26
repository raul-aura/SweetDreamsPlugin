// Fill out your copyright notice in the Description page of Project Settings.

#include "SweetDreamsWidget.h"
#include "SweetDreamsHUD.h"

void USweetDreamsWidget::HideSelf()
{
	ASweetDreamsHUD::HideWidget(this);
}
