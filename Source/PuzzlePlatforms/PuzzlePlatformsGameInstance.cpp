// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "MenuSystem/MenuWidget.h"

#include "OnlineSubsystem.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	//생성자는 언리얼 에디터에서도 실행되지만, Init은 게임이 시작될 때 실행된다.
	
	ConstructorHelpers::FClassFinder<UMenuWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if(!ensure(MenuBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;	

	ConstructorHelpers::FClassFinder<UMenuWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if(!ensure(InGameMenuBPClass.Class != nullptr)) return;

	InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());

	//싱글턴 포인터 반환
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();	

	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found OnlineSubsystem %s"), *Subsystem->GetSubsystemName().ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem is nullptr"));
	}
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if(!ensure(MenuClass != nullptr)) return;

	MainMenu = CreateWidget<UMenuWidget>(this, MenuClass);
	if(!ensure(MainMenu != nullptr)) return;

	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	if(!ensure(InGameMenuClass != nullptr)) return;

	InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if(!ensure(InGameMenu != nullptr)) return;

	InGameMenu->Setup();
	InGameMenu->SetMenuInterface(this);
}


void UPuzzlePlatformsGameInstance::Host() const
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if(!ensure(World != nullptr)) return;

	// ?listen을 붙이면 서버로서 동작하게 된다.
	World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");	
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address) const
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if(!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::LoadMainMenu() const
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("LoadMainMenu"));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::QuitGame() const
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("QuitGame"));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}
