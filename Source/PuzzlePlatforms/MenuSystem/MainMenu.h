// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface *InstanceMenuInterface);

protected:
	virtual bool Initialize();

private:
	// BindWidget : 해당 서브 위젯과 변수의 이름을 바탕으로 자동으로 매칭해서 바인딩 해주는 옵션(따라서 Button 서브 위젯의 이름도 Host여야 같은 UButton Host변수에 매칭된다.)
	UPROPERTY(meta = (BindWidget))
	class UButton *Host;

	UPROPERTY(meta = (BindWidget))
	class UButton *Join;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	IMenuInterface *MenuInterface;
};
