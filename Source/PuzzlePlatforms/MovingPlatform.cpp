// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
    PrimaryActorTick.bCanEverTick = true;

    SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 자신이 서버인지 확인하는 메소드 , True면 서버
    if (HasAuthority())
    {
        FVector Location = GetActorLocation();
        Location += FVector(Speed * DeltaTime, 0, 0);

        SetActorLocation(Location);
    }
}
