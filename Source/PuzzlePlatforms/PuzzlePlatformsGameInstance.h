// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(Exec)
	virtual void Host(const FString ServerName) override;

	UFUNCTION(Exec)
	virtual void Join(const uint32 Index) const override;

	virtual void LoadMainMenu() const override;
	virtual void QuitGame() const override;

	virtual void RefreshServerList() override;

	void StartSession();


private:
	TSubclassOf<class UMainMenu> MenuClass;
	TSubclassOf<class UInGameMenu> InGameMenuClass;

	class UMainMenu* MainMenu = nullptr;
	class UInGameMenu* InGameMenu = nullptr;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success) const;
	void OnDestroySessionComplete(FName SessionName, bool Success) const;
	void OnFindSessionsComplete(bool Success) const;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) const;

	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString) const;

	void CreateSession() const;

	FString DesiredServerName;

};
