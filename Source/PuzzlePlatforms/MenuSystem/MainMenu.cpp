// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

//#include "Blueprint/WidgetBlueprintLibrary.h"

bool UMainMenu::Initialize()
{
	bool bSuccess = Super::Initialize();
	if(!bSuccess) return false;	

	UE_LOG(LogTemp, Warning, TEXT("MainMenu Initialze"));

	if(!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);	

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if(!ensure(ConfirmJoinMenuButton != nullptr)) return false;
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(CancelJoinMenuButton != nullptr)) return false;
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);	

	return true;
}

//AddToViewport() 함수가 호출되면 호출된다.
//void UMainMenu::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//	UE_LOG(LogTemp, Warning, TEXT("MainMenu NativeConstruct"));
//}

//RemoveFromViewport() 함수가 호출되면 호출된다.
//void UMainMenu::NativeDestruct()
//{
//	Super::NativeDestruct();
//
//	UE_LOG(LogTemp, Warning, TEXT("MainMenu NativeDestruct"));
//}


void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		Teardown();
		MenuInterface->Host();
	}
}

void UMainMenu::JoinServer()
{
	if (MenuInterface != nullptr)
	{
		Teardown();
		if (!ensure(IPAddressField != nullptr)) return;
		const FString& Address = IPAddressField->GetText().ToString();

		MenuInterface->Join(Address);
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}


