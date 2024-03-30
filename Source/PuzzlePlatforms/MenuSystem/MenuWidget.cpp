// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* Interface)
{
	this->MenuInterface = Interface;
}

void UMenuWidget::Setup()
{
	AddToViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);

	//이 함수 하나로 처리 할수도 있음 
	//UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, this);

	//마우스 커서를 보이게 한다.
	PlayerController->bShowMouseCursor = true;
}

void UMenuWidget::Teardown()
{
	RemoveFromParent();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);

	//이 함수 하나로 처리 할수도 있음 
	//UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController, this);

	PlayerController->bShowMouseCursor = false;
}


