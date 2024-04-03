// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "PlatformTrigger.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer &ObjectInitializer)
{
    ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
    if (!ensure(MenuBPClass.Class != nullptr))
        return;

    MenuClass = MenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
    UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
    if (MenuClass == nullptr)
        return;
    UUserWidget *Menu = CreateWidget<UUserWidget>(this, MenuClass);

    if (Menu == nullptr)
        return;

    Menu->AddToViewport();
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