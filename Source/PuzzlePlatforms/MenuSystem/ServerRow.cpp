// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"
#include "MainMenu.h"
#include "Components/Button.h"

void UServerRow::Setup(UMainMenu* Parent, uint32 Index)
{
	ParentMenu = Parent;
	RowIndex = Index;

	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
}

void UServerRow::OnClicked()
{
	ParentMenu->SelectIndex(RowIndex);
}
