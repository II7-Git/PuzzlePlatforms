// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
public:
	UMainMenu(const FObjectInitializer &ObjectInitializer);

	void SetServerList(TArray<FString> ServerNames);

	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize();

private:
	TSubclassOf<class UUserWidget> ServerRowClass;

	// BindWidget : 해당 서브 위젯과 변수의 이름을 바탕으로 자동으로 매칭해서 바인딩 해주는 옵션(따라서 Button 서브 위젯의 이름도 Host여야 같은 UButton Host변수에 매칭된다.)
	UPROPERTY(meta = (BindWidget))
	class UButton *HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton *JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton *QuitButton;

	UPROPERTY(meta = (BindWidget))
	class UButton *CancelJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton *ConfirmJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher *MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget *JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget *MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget *ServerList;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void LoadMainMenu();

	UFUNCTION()
	void QuitPressed();

	TOptional<uint32> SelectedIndex;
};
