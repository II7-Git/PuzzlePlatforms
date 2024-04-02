// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer &ObjectInitializer)
{
    UE_LOG(LogTemp, Warning, TEXT("GameInstance Constructor"));
}

void UPuzzlePlatformsGameInstance::Init()
{
    UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));
}

void UPuzzlePlatformsGameInstance::Host()
{
    UEngine *Engine = GetEngine();

    if (Engine == nullptr)
        return;

    Engine->AddOnScreenDebugMessage(0, 10, FColor::Green, TEXT("Hosting"));

    UWorld *World = GetWorld();

    if (World == nullptr)
        return;

    World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString &Address)
{
    UEngine *Engine = GetEngine();

    if (Engine == nullptr)
        return;

    Engine->AddOnScreenDebugMessage(0, 20, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

    APlayerController *PlayerController = GetFirstLocalPlayerController();

    if (PlayerController == nullptr)
        return;

    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}