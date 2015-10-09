#pragma once

#include "SlateBasics.h"
#include "GameFramework/PlayerController.h"
#include "Online.h"

struct FOnlinePlayerData
{
	FString PlayerId;
};

struct FOnlineSessionData
{
	TSharedPtr<const FUniqueNetId> HostId;
	int32 NumPlayers;
	int32 MaxPlayers;
	int32 PingInMs;
};

class SMainMenuWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SMainMenuWidget){}

	/*See private declaration of OwnerHUD below.*/
	SLATE_ARGUMENT(TWeakObjectPtr<APlayerController>, OwnerPlayerController)

	SLATE_END_ARGS()

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Needed for every widget
	// Builds this widget and any of it's children
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	void UpdatePlayerList();
	void ListOnlineSessions(TSharedPtr<FOnlineSessionSearch> SessionSearch, bool bWasSuccessful);

	TSharedRef<ITableRow> OnGenerateListViewWidget(TSharedPtr<FOnlinePlayerData> InItem, const TSharedRef<STableViewBase>& OwnerTable);
	TSharedRef<ITableRow> OnGenerateSessionListItem(TSharedPtr<FOnlineSessionData> InItem, const TSharedRef<STableViewBase>& OwnerTable);

	bool HostOnlineSession();
	bool StartOnlineSession();
	bool FindOnlineSessions();
	void DestroyOnlineSession();

	// Main Menu button callbacks
	FReply OnSoloButtonClicked();
	FReply OnOnlineButtonClicked();

	// Online Menu button callbacks
	FReply OnHostButtonClicked();
	FReply OnFindSessionsButtonClicked();
	FReply OnMainMenuButtonClicked();

	// Lobby Menu button callbacks
	FReply OnStartButtonClicked();
	FReply OnLeaveButtonClicked();

	// Sessions Menu button callbacks
	FReply OnRefreshButtonClicked();
	FReply OnBackButtonClicked();
	
protected:
	TWeakObjectPtr<class APlayerController> OwnerPlayerController;

	TArray<TSharedPtr<FOnlinePlayerData>> PlayersData;
	TArray<TSharedPtr<FOnlineSessionData>> SessionsData;

	TSharedPtr<SListView<TSharedPtr<FOnlinePlayerData>>> PlayerListView;

	TSharedPtr<SVerticalBox> MainMenu;
	TSharedPtr<SVerticalBox> OnlineMenu;
	TSharedPtr<SHorizontalBox> LobbyMenu;
	TSharedPtr<SHorizontalBox> SessionsMenu;

};