// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"

//#include "Blueprint/WidgetBlueprintLibrary.h"

bool UMainMenu::Initialize()
{
	bool bSuccess = Super::Initialize();
	if(!bSuccess) return false;	

	UE_LOG(LogTemp, Warning, TEXT("MainMenu Initialze"));

	if(!ensure(Host != nullptr)) return false;
	Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	return true;
}

//AddToViewport() �Լ��� ȣ��Ǹ� ȣ��ȴ�.
void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("MainMenu NativeConstruct"));
}

//RemoveFromViewport() �Լ��� ȣ��Ǹ� ȣ��ȴ�.
void UMainMenu::NativeDestruct()
{
	Super::NativeDestruct();

	UE_LOG(LogTemp, Warning, TEXT("MainMenu NativeDestruct"));
}

void UMainMenu::SetMenuInterface(IMenuInterface* Interface)
{
	this->MenuInterface = Interface;
}

void UMainMenu::Setup()
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

	//�� �Լ� �ϳ��� ó�� �Ҽ��� ���� 
	//UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, this);

	//���콺 Ŀ���� ���̰� �Ѵ�.
	PlayerController->bShowMouseCursor = true;

}

void UMainMenu::Teardown()
{	
	RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);

	//�� �Լ� �ϳ��� ó�� �Ҽ��� ���� 
	//UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController, this);

	PlayerController->bShowMouseCursor = false;
}

void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}
}


