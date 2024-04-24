// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"

#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer &ObjectInitializer)
{
    ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
    if (!ensure(MenuBPClass.Class != nullptr))
        return;

    MenuClass = MenuBPClass.Class;

    ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
    if (!ensure(InGameMenuBPClass.Class != nullptr))
        return;

    InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
    IOnlineSubsystem *Subsystem = IOnlineSubsystem::Get();
    if (Subsystem != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
        IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
        if (SessionInterface.IsValid())
        {
            UE_LOG(LogTemp, Warning, TEXT("Found Session Interface"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"));
    }
}

void UPuzzlePlatformsGameInstance::LoadMenuWidget()
{
    if (MenuClass == nullptr)
        return;
    Menu = CreateWidget<UMainMenu>(this, MenuClass);

    if (Menu == nullptr)
        return;

    Menu->Setup();
    // 종속성 주입
    Menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::InGameLoadMenu()
{
    if (InGameMenuClass == nullptr)
        return;
    UMenuWidget *InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);

    if (InGameMenu == nullptr)
        return;

    InGameMenu->Setup();
    // 종속성 주입
    InGameMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host()
{
    if (Menu != nullptr)
    {
        Menu->Teardown();
    }
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

    if (Menu != nullptr)
    {
        Menu->Teardown();
    }
    UEngine *Engine = GetEngine();

    if (Engine == nullptr)
        return;

    Engine->AddOnScreenDebugMessage(0, 20, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

    APlayerController *PlayerController = GetFirstLocalPlayerController();

    if (PlayerController == nullptr)
        return;

    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{

    APlayerController *PlayerController = GetFirstLocalPlayerController();

    if (PlayerController == nullptr)
        return;

    PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}