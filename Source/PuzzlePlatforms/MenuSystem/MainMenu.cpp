// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
bool UMainMenu::Initialize()
{
    bool Success = Super::Initialize();

    if (!Success)
        return false;

    // TODO: Setup
    if (HostButton == nullptr || JoinButton == nullptr)
        return false;

    HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
    JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
    QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);
    CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::LoadMainMenu);
    ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

    return true;
}

void UMainMenu::HostServer()
{
    UE_LOG(LogTemp, Warning, TEXT("Hosting Now!!"));
    if (MenuInterface != nullptr)
    {
        MenuInterface->Host();
    }
}

void UMainMenu::JoinServer()
{
    if (MenuInterface != nullptr)
    {
        if (IPAddressField == nullptr)
            return;

        const FString &Address = IPAddressField->GetText().ToString();
        MenuInterface->Join(Address);
    }
}

void UMainMenu::OpenJoinMenu()
{
    if (JoinMenu == nullptr)
        return;

    MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::LoadMainMenu()
{
    if (MainMenu == nullptr)
        return;

    MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitPressed()
{

    UWorld *World = GetWorld();
    if (World == nullptr)
        return;
    APlayerController *PlayerController = World->GetFirstPlayerController();
    if (PlayerController == nullptr)
        return;

    PlayerController->ConsoleCommand("quit");
}