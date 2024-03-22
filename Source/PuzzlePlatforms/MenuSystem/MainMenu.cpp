// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"

bool UMainMenu::Initialize()
{
	bool bSuccess = Super::Initialize();
	if(!bSuccess) return false;	

	if(!ensure(Host != nullptr)) return false;
	Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface* Interface)
{
	this->MenuInterface = Interface;
}

void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}
}


