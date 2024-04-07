// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "TimerManager.h"
#include "PuzzlePlatformsGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;

	UE_LOG(LogTemp, Warning, TEXT("PostLogin - NumberOfPlayers : %d"), NumberOfPlayers);

	if (NumberOfPlayers >= 2)
	{
		if (GameStartTimer.IsValid()) return;

		UE_LOG(LogTemp, Warning, TEXT("Game Starting Timer Start..."));
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 10);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;

	UE_LOG(LogTemp, Warning, TEXT("Logout - NumberOfPlayers : %d"), NumberOfPlayers);
}

void ALobbyGameMode::StartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Starting!"));	
	GetWorldTimerManager().ClearTimer(GameStartTimer);

	UWorld* World = GetWorld();
	if(!ensure(World != nullptr)) return;

	UPuzzlePlatformsGameInstance* GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	if (!ensure(GameInstance != nullptr)) return;

	GameInstance->StartSession();

	bUseSeamlessTravel = true;	
	World->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
}
