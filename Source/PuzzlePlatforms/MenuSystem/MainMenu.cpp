// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "UObject/ConstructorHelpers.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer &ObjectInitializer)
{
    ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));

    if (!ensure(ServerRowBPClass.Class != nullptr))
        return;

    ServerRowClass = ServerRowBPClass.Class;
}

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
void UMainMenu::SetServerList(TArray<FString> ServerNames)
{

    UWorld *World = this->GetWorld();
    if (World == nullptr || ServerRowClass == nullptr)
        return;

    // 새로 서버 리스트를 설치하기 위해 기존 리스트 제거
    ServerList->ClearChildren();

    for (const FString &ServerName : ServerNames)
    {
        UServerRow *Row = CreateWidget<UServerRow>(World, ServerRowClass);

        if (Row == nullptr)
            return;

        Row->ServerName->SetText(FText::FromString(ServerName));

        ServerList->AddChild(Row);
    }
}
void UMainMenu::JoinServer()
{
    if (MenuInterface != nullptr)
    {
        // if (IPAddressField == nullptr)
        //     return;
        // const FString &Address = IPAddressField->GetText().ToString();
        MenuInterface->Join("");
    }
}

void UMainMenu::OpenJoinMenu()
{
    if (JoinMenu == nullptr)
        return;

    MenuSwitcher->SetActiveWidget(JoinMenu);

    if (MenuInterface != nullptr)
    {
        MenuInterface->RefreshServerList();
    }
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