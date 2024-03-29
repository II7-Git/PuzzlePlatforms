// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
    PrimaryActorTick.bCanEverTick = true;

    SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        SetReplicates(true);
        SetReplicateMovement(true);
    }

    GlobalStartLocation = GetActorLocation();
    GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (ActiveTriggers > 0)
    {
        // 자신이 서버인지 확인하는 메소드 , True면 서버
        if (HasAuthority())
        {
            FVector Location = GetActorLocation();

            // 이동해야할 최대 거리
            float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
            // 현재까지 이동한 거리
            float JourneyTravelled = (Location - GlobalStartLocation).Size();

            // 이동 거리가 목표 지점을 넘어섰으면 되돌아가게 설정
            // 시작 벡터와 목표 벡터를 교환
            if (JourneyTravelled >= JourneyLength)
            {
                FVector Swap = GlobalStartLocation;
                GlobalStartLocation = GlobalTargetLocation;
                GlobalTargetLocation = Swap;
            }
            // 현재 위치에서 타겟 위치까지의 벡터
            FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
            Location += Speed * DeltaTime * Direction;

            SetActorLocation(Location);
        }
    }
}

void AMovingPlatform::AddActiveTrigger()
{
    ActiveTriggers++;
}
void AMovingPlatform::RemoveActiveTrigger()
{
    if (ActiveTriggers > 0)
        ActiveTriggers--;
}