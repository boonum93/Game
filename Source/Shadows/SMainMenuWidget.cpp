#include "Shadows.h"
#include "SMainMenuWidget.h"
#include "ShadowsGameSession.h"

#define LOCTEXT_NAMESPACE "SMainMenuWidget"

void SMainMenuWidget::Construct(const FArguments& InArgs)
{
	OwnerPlayerController = InArgs._OwnerPlayerController;

	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		.Padding(300)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			[
				// Main Menu
				SAssignNew(MainMenu, SVerticalBox)
				+ SVerticalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				[
					// "Main Menu" text block
					SNew(STextBlock)
					.Text(FText::FromString("Main Menu"))
					.Font(FSlateFontInfo("Veranda", 64))
				]
				+ SVerticalBox::Slot()
				[
					// Solo Button
					SNew(SButton)
					.OnClicked(this, &SMainMenuWidget::OnSoloButtonClicked)
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Solo"))
						.Font(FSlateFontInfo("Veranda", 64))
					]
				]
				+ SVerticalBox::Slot()
				[
					// Online Button
					SNew(SButton)
					.OnClicked(this, &SMainMenuWidget::OnOnlineButtonClicked)
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Online"))
						.Font(FSlateFontInfo("Veranda", 64))
					]
				]
			]
			+ SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			[
				// Online Menu
				SAssignNew(OnlineMenu, SVerticalBox)
				+ SVerticalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				[
					// "Online" text block
					SNew(STextBlock)
					.Text(FText::FromString("Online"))
					.Font(FSlateFontInfo("Veranda", 64))
				]
				+ SVerticalBox::Slot()
				[
					// Host Button
					SNew(SButton)
					.OnClicked(this, &SMainMenuWidget::OnHostButtonClicked)
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Host"))
						.Font(FSlateFontInfo("Veranda", 64))
					]
				]
				+SVerticalBox::Slot()
				[
					// Find Sessions Button
					SNew(SButton)
					.OnClicked(this, &SMainMenuWidget::OnFindSessionsButtonClicked)
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Find Sessions"))
						.Font(FSlateFontInfo("Veranda", 64))
					]
				]
				+ SVerticalBox::Slot()
				[
					// Main Menu Button
					SNew(SButton)
					.OnClicked(this, &SMainMenuWidget::OnMainMenuButtonClicked)
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Main Menu"))
						.Font(FSlateFontInfo("Veranda", 64))
					]
				]
			]
			+ SOverlay::Slot()
			[
				// Lobby Menu
				SAssignNew(LobbyMenu, SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						[
							SAssignNew(PlayerListView, SListView<TSharedPtr<FOnlinePlayerData>>)
							.ItemHeight(50)
							.ListItemsSource(&PlayersData)
							.OnGenerateRow(this, &SMainMenuWidget::OnGenerateListViewWidget)
							.HeaderRow
							(
							SNew(SHeaderRow)
							+ SHeaderRow::Column("PlayerID").DefaultLabel(NSLOCTEXT("PlayersData", "PlayerIdColumn", "Player ID"))
							.HAlignHeader(HAlign_Right)
							.HAlignCell(HAlign_Right)
							)
						]
					]
				]
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						// "Lobby" text block
						SNew(STextBlock)
						.Text(FText::FromString("Lobby"))
						.Font(FSlateFontInfo("Veranda", 64))
					]
					+ SVerticalBox::Slot()
					[
						// Start Button
						SNew(SButton)
						.OnClicked(this, &SMainMenuWidget::OnStartButtonClicked)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Center)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Start"))
							.Font(FSlateFontInfo("Veranda", 64))
						]
					]
					+ SVerticalBox::Slot()
					[
						// Leave Button
						SNew(SButton)
						.OnClicked(this, &SMainMenuWidget::OnLeaveButtonClicked)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Center)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Leave"))
							.Font(FSlateFontInfo("Veranda", 64))
						]
					]
				]
			]
			+ SOverlay::Slot()
			[
				// Sessions Menu
				SAssignNew(SessionsMenu, SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						[
							SNew(SListView<TSharedPtr<FOnlineSessionData>>)
							.ItemHeight(50)
							.ListItemsSource(&SessionsData)
							.OnGenerateRow(this, &SMainMenuWidget::OnGenerateSessionListItem)
							.HeaderRow
							(
							SNew(SHeaderRow)
							+ SHeaderRow::Column("HostID").DefaultLabel(NSLOCTEXT("SessionsData", "HostIdColumn", "Host ID"))
							+ SHeaderRow::Column("NumPlayers").DefaultLabel(NSLOCTEXT("SessionsData", "NumPlayersColumn", "# Players"))
							+ SHeaderRow::Column("MaxPlayers").DefaultLabel(NSLOCTEXT("SessionsData", "MaxPlayersColumn", "Max Players"))
							+ SHeaderRow::Column("Ping").DefaultLabel(NSLOCTEXT("SessionsData", "PingColumn", "Ping (ms)"))
							.HAlignHeader(HAlign_Right)
							.HAlignCell(HAlign_Right)
							)
						]
					]
				]
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						// "Sessions" text block
						SNew(STextBlock)
						.Text(FText::FromString("Sessions"))
						.Font(FSlateFontInfo("Veranda", 64))
					]
					+ SVerticalBox::Slot()
					[
						// Refresh Button
						SNew(SButton)
						.OnClicked(this, &SMainMenuWidget::OnRefreshButtonClicked)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Center)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Refresh"))
							.Font(FSlateFontInfo("Veranda", 64))
						]
					]
					+ SVerticalBox::Slot()
					[
						// Back Button
						SNew(SButton)
						.OnClicked(this, &SMainMenuWidget::OnBackButtonClicked)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Center)
						[
							SNew(STextBlock)
							.Text(FText::FromString("Back"))
							.Font(FSlateFontInfo("Veranda", 64))
						]
					]
				]
			]
			
		];

	// Show only the Main Menu at startup
	MainMenu->SetVisibility(EVisibility::Visible);
	OnlineMenu->SetVisibility(EVisibility::Hidden);
	LobbyMenu->SetVisibility(EVisibility::Hidden);
	SessionsMenu->SetVisibility(EVisibility::Hidden);

	// Bind ListOnlineSessions to the OnFindSessionsComplete event
	UWorld* World = OwnerPlayerController->GetWorld();
	if (World)
	{
		AGameMode* GameMode = World->GetAuthGameMode();
		if (GameMode)
		{
			AShadowsGameSession* ShadowsGameSession = Cast<AShadowsGameSession>(GameMode->GameSession);
			if (ShadowsGameSession)
			{
				ShadowsGameSession->OnFindSessionsComplete().AddRaw(this, &SMainMenuWidget::ListOnlineSessions);
			}
		}
	}

}

void SMainMenuWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	// TODO: Call this when player joins or leaves session to improve performance
	if (LobbyMenu->GetVisibility() == EVisibility::Visible)
	{
		UpdatePlayerList();
	}
	
}

void SMainMenuWidget::UpdatePlayerList()
{
	UWorld* World = OwnerPlayerController->GetWorld();
	if (World)
	{
		AGameMode* GameMode = World->GetAuthGameMode();
		if (GameMode)
		{
			AGameState* GameState = GameMode->GetGameState<AGameState>();
			if (GameState)
			{
				PlayersData.Empty();
				// Gain access to all players currently in the lobby
				TArray<APlayerState*> PlayerArray = GameState->PlayerArray;
				for (auto& Player : PlayerArray)
				{
					TSharedPtr<FOnlinePlayerData> NewPlayerData = MakeShareable(new FOnlinePlayerData());
					
					NewPlayerData->PlayerId = FString::FromInt(Player->PlayerId);

					PlayersData.Add(NewPlayerData);
				}
			}
		}
	}
}

void SMainMenuWidget::ListOnlineSessions(TSharedPtr<FOnlineSessionSearch> SessionSearch, bool bWasSuccessful)
{
	// TODO: Incorporate some sort of status to notify the user when finding sessions
	// is still in progress and when it is complete.

	if (bWasSuccessful)
	{
		SessionsData.Empty();
		// TODO: If there are no current sessions, then display text notifying player no sessions were found
		for (auto& SearchResult : SessionSearch->SearchResults)
		{
			TSharedPtr<FOnlineSessionData> NewSessionData = MakeShareable(new FOnlineSessionData);

			NewSessionData->HostId = SearchResult.Session.OwningUserId;
			NewSessionData->NumPlayers = SearchResult.Session.NumOpenPublicConnections;
			NewSessionData->MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			NewSessionData->PingInMs = SearchResult.PingInMs;

			SessionsData.Add(NewSessionData);
		}
	}
}

TSharedRef<ITableRow> SMainMenuWidget::OnGenerateListViewWidget(TSharedPtr<FOnlinePlayerData> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	class SOnlinePlayerRowWidget : public SMultiColumnTableRow<TSharedPtr<FOnlinePlayerData>>
	{
	public:
		SLATE_BEGIN_ARGS(SOnlinePlayerRowWidget){}
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable, TSharedPtr<FOnlinePlayerData> InItem)
		{
			Item = InItem;
			SMultiColumnTableRow<TSharedPtr<FOnlinePlayerData>>::Construct(FSuperRowType::FArguments(), InOwnerTable);
		}

		TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName)
		{
			FText ItemText = FText::GetEmpty();
			if (ColumnName == "PlayerID")
			{
				ItemText = FText::FromString(Item->PlayerId);
			}
			return SNew(STextBlock)
				.Text(ItemText)
				.Font(FSlateFontInfo("Veranda", 32));
		}
		TSharedPtr<FOnlinePlayerData> Item;
	};
	return SNew(SOnlinePlayerRowWidget, OwnerTable, InItem);
}

TSharedRef<ITableRow> SMainMenuWidget::OnGenerateSessionListItem(TSharedPtr<FOnlineSessionData> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	class SOnlineSessionRowWidget : public SMultiColumnTableRow < TSharedPtr<FOnlineSessionData> >
	{
	public:
		SLATE_BEGIN_ARGS(SOnlineSessionRowWidget){}
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable, TSharedPtr<FOnlineSessionData> InItem)
		{
			Item = InItem;
			SMultiColumnTableRow<TSharedPtr<FOnlineSessionData>>::Construct(FSuperRowType::FArguments(), InOwnerTable);
		}

		TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName)
		{
			FText ItemText = FText::GetEmpty();
			if (ColumnName == "Host ID")
			{
				ItemText = FText::FromString(Item->HostId->ToString());
			}
			else if (ColumnName == "# Players")
			{
				ItemText = FText::FromString(FString::FromInt(Item->NumPlayers));
			}
			else if (ColumnName == "Max Players")
			{
				ItemText = FText::FromString(FString::FromInt(Item->MaxPlayers));
			}
			else if (ColumnName == "Ping (ms)")
			{
				ItemText = FText::FromString(FString::FromInt(Item->PingInMs));
			}
			return SNew(STextBlock)
				.Text(ItemText)
				.Font(FSlateFontInfo("Veranda", 32));
		}
		TSharedPtr<FOnlineSessionData> Item;
	};
	return SNew(SOnlineSessionRowWidget, OwnerTable, InItem);
}

FReply SMainMenuWidget::OnSoloButtonClicked()
{
	return FReply::Handled();
}

FReply SMainMenuWidget::OnOnlineButtonClicked()
{
	// Go to the online menu by hiding the other menus and showing the online menu
	MainMenu->SetVisibility(EVisibility::Hidden);
	OnlineMenu->SetVisibility(EVisibility::Visible);

	return FReply::Handled();
}

FReply SMainMenuWidget::OnHostButtonClicked()
{
	if (HostOnlineSession())
	{
		OnlineMenu->SetVisibility(EVisibility::Hidden);
		LobbyMenu->SetVisibility(EVisibility::Visible);
	}
	else
	{
		// TODO: Show the host session failed text block that will be part of the Online Menu
		;
	}

	return FReply::Handled();
}

FReply SMainMenuWidget::OnFindSessionsButtonClicked()
{
	if (FindOnlineSessions())
	{
		// TODO: Incorporate some sort of status to notify the user when finding sessions
		// is still in progress and when it is complete.
		OnlineMenu->SetVisibility(EVisibility::Hidden);
		SessionsMenu->SetVisibility(EVisibility::Visible);
	}
	return FReply::Handled();
}

FReply SMainMenuWidget::OnMainMenuButtonClicked()
{
	// Show the Main Menu and hide the Online Menu
	OnlineMenu->SetVisibility(EVisibility::Hidden);
	MainMenu->SetVisibility(EVisibility::Visible);

	return FReply::Handled();
}

FReply SMainMenuWidget::OnStartButtonClicked()
{
	StartOnlineSession();
	return FReply::Handled();
}

FReply SMainMenuWidget::OnLeaveButtonClicked()
{
	// Destroy the current session if you are the host. Otherwise just leave the session.
	// TODO: Implement leaving the session when not hosting.
	DestroyOnlineSession();
	
	// Return to the Online Menu
	LobbyMenu->SetVisibility(EVisibility::Hidden);
	OnlineMenu->SetVisibility(EVisibility::Visible);

	return FReply::Handled();
}

FReply SMainMenuWidget::OnRefreshButtonClicked()
{
	if (FindOnlineSessions())
	{
		// TODO: Incorporate some sort of status to notify the user when finding sessions
		// is still in progress and when it is complete.
	}

	return FReply::Handled();
}

FReply SMainMenuWidget::OnBackButtonClicked()
{
	SessionsMenu->SetVisibility(EVisibility::Hidden);
	OnlineMenu->SetVisibility(EVisibility::Visible);
	return FReply::Handled();
}

bool SMainMenuWidget::HostOnlineSession()
{
	UWorld* World = OwnerPlayerController->GetWorld();
	if (World)
	{
		AGameMode* GameMode = World->GetAuthGameMode();
		if (GameMode)
		{
			UGameInstance* GameInstance = GameMode->GetGameInstance();
			if (GameInstance)
			{
				ULocalPlayer* LocalPlayer = GameInstance->GetFirstGamePlayer();
				if (LocalPlayer)
				{
					AShadowsGameSession* ShadowsGameSession = Cast<AShadowsGameSession>(GameMode->GameSession);
					if (ShadowsGameSession)
					{
						return ShadowsGameSession->HostSession(LocalPlayer->GetUniqueNetIdFromCachedControllerId(), /*SessionName*/"TempSessionName", false, true, 4);
					}
				}
			}
		}
	}

	return false;
}

bool SMainMenuWidget::StartOnlineSession()
{
	UWorld* World = OwnerPlayerController->GetWorld();
	if (World)
	{
		AGameMode* GameMode = World->GetAuthGameMode();
		if (GameMode)
		{
			UGameInstance* GameInstance = GameMode->GetGameInstance();
			if (GameInstance)
			{
				AShadowsGameSession* ShadowsGameSession = Cast<AShadowsGameSession>(GameMode->GameSession);
				if (ShadowsGameSession)
				{
					ShadowsGameSession->StartGame();
					return true;
				}
			}
		}
	}

	return false;
}

bool SMainMenuWidget::FindOnlineSessions()
{
	UWorld* World = OwnerPlayerController->GetWorld();
	if (World)
	{
		AGameMode* GameMode = World->GetAuthGameMode();
		if (GameMode)
		{
			UGameInstance* GameInstance = GameMode->GetGameInstance();
			if (GameInstance)
			{
				ULocalPlayer* LocalPlayer = GameInstance->GetFirstGamePlayer();
				if (LocalPlayer)
				{
					AShadowsGameSession* ShadowsGameSession = Cast<AShadowsGameSession>(GameMode->GameSession);
					if (ShadowsGameSession)
					{
						ShadowsGameSession->FindSessions(LocalPlayer->GetUniqueNetIdFromCachedControllerId(), "", false, false);
						return true;
					}
				}
			}
		}
	}

	return false;
}

void SMainMenuWidget::DestroyOnlineSession()
{
	UWorld* World = OwnerPlayerController->GetWorld();
	if (World)
	{
		AGameMode* GameMode = World->GetAuthGameMode();
		if (GameMode)
		{
			AShadowsGameSession* ShadowsGameSession = Cast<AShadowsGameSession>(GameMode->GameSession);
			if (ShadowsGameSession)
			{
				ShadowsGameSession->DestroySession();
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE