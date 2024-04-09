// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"

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
    UMainMenu *Menu = CreateWidget<UMainMenu>(this, MenuClass);

    if (Menu == nullptr)
        return;

    Menu->AddToViewport();

    APlayerController *PlayerController = GetFirstLocalPlayerController();
    if (PlayerController == nullptr)
        return;

    // UI 입력 가능하게 설정하는 기능

    // UI인풋에 관한 모드를 생성함
    FInputModeUIOnly InputModeData;
    // 조작하고자 하는 위젯의 포커스를 둠
    InputModeData.SetWidgetToFocus(Menu->TakeWidget());
    // 마우스가 화면에 잠기지 않게 설정(화면 밖으로 이동하거나 하는 것이 가능)
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    // 만든 인풋 모드로 컨트롤러의 인풋 모드를 변경
    PlayerController->SetInputMode(InputModeData);

    // 커서 보이기
    PlayerController->bShowMouseCursor = true;

    // 종속성 주입
    Menu->SetMenuInterface(this);
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