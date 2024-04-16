// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"
void UMenuWidget::Setup()
{
    this->AddToViewport();

    UWorld *World = GetWorld();
    if (World == nullptr)
        return;
    APlayerController *PlayerController = World->GetFirstPlayerController();
    if (PlayerController == nullptr)
        return;

    // UI 입력 가능하게 설정하는 기능

    // UI인풋에 관한 모드를 생성함
    FInputModeUIOnly InputModeData;
    // 조작하고자 하는 위젯의 포커스를 둠
    InputModeData.SetWidgetToFocus(this->TakeWidget());
    // 마우스가 화면에 잠기지 않게 설정(화면 밖으로 이동하거나 하는 것이 가능)
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    // 만든 인풋 모드로 컨트롤러의 인풋 모드를 변경
    PlayerController->SetInputMode(InputModeData);

    // 커서 보이기
    PlayerController->bShowMouseCursor = true;
}

void UMenuWidget::Teardown()
{
    this->RemoveFromViewport();

    UWorld *World = GetWorld();
    if (World == nullptr)
        return;
    APlayerController *PlayerController = World->GetFirstPlayerController();
    if (PlayerController == nullptr)
        return;

    FInputModeGameOnly InputModeData;
    PlayerController->SetInputMode(InputModeData);

    PlayerController->bShowMouseCursor = false;
}

void UMenuWidget::SetMenuInterface(IMenuInterface *InstanceMenuInterface)
{
    this->MenuInterface = InstanceMenuInterface;
}
