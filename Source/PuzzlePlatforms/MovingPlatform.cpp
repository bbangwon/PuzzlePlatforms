// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	UE_LOG(LogTemp, Display, TEXT("MovingPlatform 생성자 실행"));
	
	//Tick함수를 사용하기 위해 true로 설정
	PrimaryActorTick.bCanEverTick = true;

	//모빌리티를 무버블로 설정
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("MovingPlatform BeginPlay"));

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);	
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//서버에서만 실행
	if (HasAuthority())
	{
		FVector Location = GetActorLocation();
		FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();

		float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
		float JourneyTravelled = (Location - GlobalStartLocation).Size();
		if (JourneyTravelled >= JourneyLength)
		{
			FVector Temp = GlobalStartLocation;
			GlobalStartLocation = GlobalTargetLocation;
			GlobalTargetLocation = Temp;
		}

		Location += Speed * DeltaSeconds * Direction;
		SetActorLocation(Location);
	}
}
