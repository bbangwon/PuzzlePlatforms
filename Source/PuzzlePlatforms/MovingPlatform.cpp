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
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//서버에서만 실행
	if (HasAuthority())
	{
		FVector Location = GetActorLocation();
		Location += FVector(Speed * DeltaSeconds, 0, 0);
		SetActorLocation(Location);
	}	
}
