// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzlePlatformsGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 플레이어가 게임에 들어오고 나가는 것을 관리하고
 * 게임 시작을 관리하는 클래스
 */
UCLASS()
class PUZZLEPLATFORMS_API ALobbyGameMode : public APuzzlePlatformsGameMode
{
	GENERATED_BODY()
	
public:
	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(AController* Exiting) override;	

private:
	uint32 NumberOfPlayers = 0;
	FTimerHandle GameStartTimer;

	void StartGame();

};
