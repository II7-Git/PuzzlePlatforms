// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("My Session Game");
const static FName SERVER_NAME_SETTING_KEY = TEXT("Server Name");

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
        SessionInterface = Subsystem->GetSessionInterface();
        if (SessionInterface.IsValid())
        {
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
            SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
            SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
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

void UPuzzlePlatformsGameInstance::Host(FString ServerName)
{
    DesiredServerName = ServerName;
    if (SessionInterface.IsValid())
    {
        auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
        if (ExistingSession != nullptr)
        {
            SessionInterface->DestroySession(SESSION_NAME);
        }
        else
        {
            CreateSession();
        }
    }
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
    if (Success)
    {
        CreateSession();
    }
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
    if (SessionInterface.IsValid())
    {
        FOnlineSessionSettings SessionSettings;
        // 로컬 네트워크를 통한 검색 허용, 같은 컴퓨터에서 매칭 가능하게 한다.

        // 서브시스템 이용안하는 테스트 시에는 랜매치 허용
        if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
        {
            SessionSettings.bIsLANMatch = true;
        }
        else
        {
            SessionSettings.bIsLANMatch = false;
        }

        // 플레이어 수 제한 (public, private 모두 있다)
        SessionSettings.NumPublicConnections = 2;
        // 온라인에서 세션을 볼 수 있게하는 옵션
        SessionSettings.bShouldAdvertise = true;

        SessionSettings.bUsesPresence = true;
        SessionSettings.Set(SERVER_NAME_SETTING_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

        SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
    }
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
    if (!Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
        return;
    }
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
void UPuzzlePlatformsGameInstance::RefreshServerList()
{
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    if (SessionSearch.IsValid())
    {
        // 로컬 내트워크 매칭 허용
        // SessionSearch->bIsLanQuery = true;
        SessionSearch->MaxSearchResults = 100;
        SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

        UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
        SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
    }
}
void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool Success)
{

    if (Success && SessionSearch.IsValid() && Menu != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Finished Find Session"));

        TArray<FServerData> ServerNames;
        for (const FOnlineSessionSearchResult &SearchResult : SessionSearch->SearchResults)
        {
            UE_LOG(LogTemp, Warning, TEXT("Found session names: %s"), *SearchResult.GetSessionIdStr());
            FServerData Data;
            Data.Name = SearchResult.GetSessionIdStr();
            Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
            Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
            Data.HostUsername = SearchResult.Session.OwningUserName;

            FString ServerName;
            if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTING_KEY, ServerName))
            {
                // UE_LOG(LogTemp, Warning, TEXT("Data Found in Settings: %s"), *ServerName);
                Data.Name = ServerName;
            }
            else
            {
                // UE_LOG(LogTemp, Warning, TEXT("Didnt get expected Data"));
                Data.Name = "Could not find name";
            }
            ServerNames.Add(Data);
        }

        Menu->SetServerList(ServerNames);
    }
}

void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{
    if (!SessionInterface.IsValid())
        return;

    if (!SessionSearch.IsValid())
        return;

    if (Menu != nullptr)
    {
        // Menu->SetServerList({});
        Menu->Teardown();
    }

    SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (!SessionInterface.IsValid())
        return;

    FString Address;
    if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not get connect String"));
        return;
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