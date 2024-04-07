// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;

	UE_LOG(LogTemp, Warning, TEXT("PostLogin - NumberOfPlayers : %d"), NumberOfPlayers);

	if (NumberOfPlayers >= 2)
	{
		UWorld* World = GetWorld();
		if(!ensure(World != nullptr)) return;

		bUseSeamlessTravel = true;
		World->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;

	UE_LOG(LogTemp, Warning, TEXT("Logout - NumberOfPlayers : %d"), NumberOfPlayers);
}
