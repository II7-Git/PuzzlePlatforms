// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzlePlatformsGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 *
 */

// 게임 모드의 함수는 서버 측에서 동작한다.
UCLASS()
class PUZZLEPLATFORMS_API ALobbyGameMode : public APuzzlePlatformsGameMode
{
	GENERATED_BODY()

public:
	void PostLogin(APlayerController *NewPlayer) override;

	void Logout(AController *Exiting) override;

private:
	uint32 NumberOfPlayers = 0;
};
