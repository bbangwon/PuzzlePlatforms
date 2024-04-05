// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "MenuSystem/MenuWidget.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

const static FName SESSION_NAME = TEXT("My Session Game");

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
		SessionInterface = Subsystem->GetSessionInterface();

		//델리게이트는 여기서 설정
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);

		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);

		SessionSearch = MakeShareable(new FOnlineSessionSearch());
		if (SessionSearch.IsValid())
		{		
			SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem is nullptr"));
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool Success) const
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete Called"));
	if (Success && SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Find Session Complete"));

		for (const FOnlineSessionSearchResult& Result : SessionSearch->SearchResults)
		{
			FString SessionName = Result.GetSessionIdStr();
			uint32 HostNum = Result.Session.NumOpenPublicConnections;
			FString HostName = Result.Session.OwningUserName;

			UE_LOG(LogTemp, Warning, TEXT("SessionName: %s, HostNum: %d, HostName: %s"), *SessionName, HostNum, *HostName);
		}
	}
}

//세션 생성이 완료되면 호출
void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success) const
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

	if (MainMenu != nullptr)
	{
		MainMenu->Teardown();
	}

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	// ?listen을 붙이면 서버로서 동작하게 된다.
	World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
}	

//서버가 세션을 파괴하면 다시 세션을 생성한다.
void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) const
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not destroy session"));
		return;
	}
	else
	{
		CreateSession();
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

void UPuzzlePlatformsGameInstance::CreateSession() const
{
	if(SessionInterface == nullptr) return;

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;	//로컬 네트워크에서 사용
	SessionSettings.NumPublicConnections = 2;	//최대 2명까지 접속 가능
	SessionSettings.bShouldAdvertise = true;	//다른 플레이어에게 보여지도록 설정
	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}


void UPuzzlePlatformsGameInstance::Host() const
{
	if (SessionInterface == nullptr)
		return;
	
	//세션을 생성하기 전에 기존 세션이 있는지 확인한다.
	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (ExistingSession != nullptr)
	{ 
		SessionInterface->DestroySession(SESSION_NAME);
	}
	else
	{
		CreateSession();
	}
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
