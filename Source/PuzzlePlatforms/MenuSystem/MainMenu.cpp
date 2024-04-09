// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"

bool UMainMenu::Initialize()
{
    bool Success = Super::Initialize();

    if (!Success)
        return false;

    // TODO: Setup
    if (Host == nullptr || Join == nullptr)
        return false;

    Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
    Join->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

    return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface *InstanceMenuInterface)
{
    this->MenuInterface = InstanceMenuInterface;
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
}
