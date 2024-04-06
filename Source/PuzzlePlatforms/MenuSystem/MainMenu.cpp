// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "ServerRow.h"

//#include "Blueprint/WidgetBlueprintLibrary.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool bSuccess = Super::Initialize();
	if(!bSuccess) return false;	

	UE_LOG(LogTemp, Warning, TEXT("MainMenu Initialze"));

	if(!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);	

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

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
		MenuInterface->Host();
	}
}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FString& ServerName : ServerNames)
	{
		UServerRow* ServerRow = CreateWidget<UServerRow>(ServerList, ServerRowClass);
		if (ServerRow != nullptr)
		{
			ServerRow->ServerName->SetText(FText::FromString(ServerName));
			ServerRow->Setup(this, i++);

			ServerList->AddChild(ServerRow);
		}
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); i++)
	{
		UServerRow* Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->Selected = SelectedIndex.IsSet() && SelectedIndex.GetValue() == i;
		}
	}
}


void UMainMenu::JoinServer()
{
	if (MenuInterface != nullptr)
	{	
		if (SelectedIndex.IsSet())
		{
			uint32 SelectedIndexValue = SelectedIndex.GetValue();
			SelectedIndex.Reset();

			UE_LOG(LogTemp, Warning, TEXT("Selected index %d"), SelectedIndexValue);
			MenuInterface->Join(SelectedIndexValue);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Selected index not set"));
		}
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(JoinMenu);

	//JoinMenu 가 열리면 서버 리스트를 갱신한다.
	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshServerList();
	}

	//테스트
	//SetServerList({ "Test1", "Test2" });
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitGame()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->QuitGame();
	}
}




