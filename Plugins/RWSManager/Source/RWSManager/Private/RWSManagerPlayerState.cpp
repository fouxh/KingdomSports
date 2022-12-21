// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#include "RWSManagerPlayerState.h"
#include "RWSManagerGameInstance.h"


#include "RWSManagerDefines.h"
#include "RWSManager.h"
#include "RWSFunctionLibrary.h"

#include "RWSManagerSettings.h"

#include "RWSUtilsUrlBuilder.h"

#include "Dom/JsonObject.h"
#include "JsonObjectConverter.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

#include "Engine.h"

void ARWSManagerPlayerState::BeginPlay()
{
	Super::BeginPlay();
	URWSManagerGameInstance* GI = Cast<URWSManagerGameInstance>(GetGameInstance());

	if (GI->Unique_Player_Matchmaking_ID != "") {
			Unique_Player_Matchmaking_ID = GI->Unique_Player_Matchmaking_ID;
			//TODO: This is just a temp work around for player queue joined.
			RWSPlayerQueue("", "", "", "", "Left", "update");
	}
}

void ARWSManagerPlayerState::RWSJoinSession(const FString& Address, const FOnFindSessionUpdate& MessageUpdateCallback)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (Address != "")
		{
			//Clear timers, and reset all data.
			ARWSManagerPlayerState::RWSCancelFindSession();
			const URWSManagerSettings* Settings = FRWSManager::Get().GetSettings();
			MessageUpdateCallback.ExecuteIfBound(Settings->MatchmakingJoiningMessage);
			RWSPlayerQueue(SMapName, SGameMode, SGameRegion, SGameType, "Joining", "update");
			UE_LOG(LogRWSManager, Warning, TEXT("Joining Server: %s"), *Address);
			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		}
	}
}

//Create Request and send it to complete to check for errors or success then send to blueprint
//Create Session RWS
void ARWSManagerPlayerState::RWSServerBrowser(const FString& MapName, const FString& GameMode, const FString& GameRegion, const FString& ServerName, const FOnAuthSuccessServerBrowser& SuccessCallback, const FOnPlayerStateAuthError& ErrorCallback)
{
		TSharedPtr<FJsonObject> RequestDataJson = MakeShareable(new FJsonObject());
		RequestDataJson->SetStringField(TEXT("MapName"), MapName);
		RequestDataJson->SetStringField(TEXT("GameMode"), GameMode);
		RequestDataJson->SetStringField(TEXT("GameRegion"), GameRegion);
		RequestDataJson->SetStringField(TEXT("ServerName"), ServerName);

		FString PostContent;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PostContent);
		FJsonSerializer::Serialize(RequestDataJson.ToSharedRef(), Writer);

		// Generate endpoint url
		const URWSManagerSettings* Settings = FRWSManager::Get().GetSettings();
		const FString Endpoint = TEXT("https://reversewinterstudios.com/game/api/v1/server/find_session");
		const FString Url = RWSUtilsUrlBuilder(Endpoint)
			.AddStringQueryParam(TEXT("game_id"), Settings->ProjectID)
			.AddStringQueryParam(TEXT("login_project_id"), Settings->LoginID)
			.AddStringQueryParam(TEXT("type"), "server_browser")
			.Build();

		TSharedRef<IHttpRequest> HttpRequest = CreateHttpRequest(Url, ERWSHttpRequestVerb::VERB_POST, PostContent);
		HttpRequest->OnProcessRequestComplete().BindUObject(this, &ARWSManagerPlayerState::RWSServerBrowser_HttpRequestComplete, SuccessCallback, ErrorCallback);
		HttpRequest->ProcessRequest();
}

void ARWSManagerPlayerState::RWSServerBrowser_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded,
	FOnAuthSuccessServerBrowser SuccessCallback, FOnPlayerStateAuthError ErrorCallback)
{
	TSharedPtr<FJsonObject> JsonObject;
	RWSHttpRequestError OutError;
	if (RWSUtilsHttpRequestHelper::ParseResponseAsJson(HttpRequest, HttpResponse, bSucceeded, JsonObject, OutError))
	{
		static const FString ServerIpContent = TEXT("content");
		if (JsonObject->HasTypedField<EJson::String>(ServerIpContent))
		{
			const FString Content = JsonObject.Get()->GetStringField(ServerIpContent);
			const FString ServersObject = JsonObject.Get()->GetStringField(TEXT("servers"));
			//Check why we are not getting the array back in blueprint.
			FJsonObjectConverter::JsonArrayStringToUStruct(ServersObject, &ServerArray, 0, 0);
			SuccessCallback.ExecuteIfBound(ServerArray);
			UE_LOG(LogRWSManager, Warning, TEXT("%s"), *Content);			
			return;
		}
		else
		{
			TSharedPtr<FJsonObject> ErrorObject = JsonObject.Get()->GetObjectField(TEXT("error"));
			OutError.description = ErrorObject.Get()->GetStringField(TEXT("description"));
		}
	}

	HandleRequestError(OutError, ErrorCallback);
}

void ARWSManagerPlayerState::RWSGetRegions(const FOnAuthSuccessRegions& SuccessCallback, const FOnPlayerStateAuthError& ErrorCallback)
{
	TSharedPtr<FJsonObject> RequestDataJson = MakeShareable(new FJsonObject());

	FString PostContent;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PostContent);
	FJsonSerializer::Serialize(RequestDataJson.ToSharedRef(), Writer);

	// Generate endpoint url
	const URWSManagerSettings* Settings = FRWSManager::Get().GetSettings();
	const FString Endpoint = TEXT("https://reversewinterstudios.com/game/api/v1/server/get_region_list");
	const FString Url = RWSUtilsUrlBuilder(Endpoint)
		.AddStringQueryParam(TEXT("game_id"), Settings->ProjectID)
		.AddStringQueryParam(TEXT("login_project_id"), Settings->LoginID)
		.Build();

	TSharedRef<IHttpRequest> HttpRequest = CreateHttpRequest(Url, ERWSHttpRequestVerb::VERB_POST, PostContent);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ARWSManagerPlayerState::RWSGetRegions_HttpRequestComplete, SuccessCallback, ErrorCallback);
	HttpRequest->ProcessRequest();
}

void ARWSManagerPlayerState::RWSGetRegions_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, FOnAuthSuccessRegions SuccessCallback, FOnPlayerStateAuthError ErrorCallback)
{
	TSharedPtr<FJsonObject> JsonObject;
	RWSHttpRequestError OutError;

	if (RWSUtilsHttpRequestHelper::ParseResponseAsJson(HttpRequest, HttpResponse, bSucceeded, JsonObject, OutError))
	{
		static const FString content = TEXT("content");
		if (JsonObject->HasTypedField<EJson::String>(content))
		{
			TArray<FRegions> JsonArray;
			const FString Content = JsonObject.Get()->GetStringField(content);
			const FString RegionObject = JsonObject.Get()->GetStringField(TEXT("regions"));
			//Check why we are not getting the array back in blueprint.
			FJsonObjectConverter::JsonArrayStringToUStruct(RegionObject, &JsonArray, 0, 0);
			SuccessCallback.ExecuteIfBound(JsonArray);
			UE_LOG(LogRWSManager, Warning, TEXT("%s"), *Content);
			return;
		}
		TSharedPtr<FJsonObject> ErrorObject = JsonObject.Get()->GetObjectField(TEXT("error"));
		OutError.description = ErrorObject.Get()->GetStringField(TEXT("description"));
	}

	HandleRequestError(OutError, ErrorCallback);
}

//Create Request and send it to complete to check for errors or success then send to blueprint
//Create Session RWS
void ARWSManagerPlayerState::RWSFindSession(const FString& MapName, const FString& GameMode, const FString& GameRegion, const FString& GameType, const FOnFindSession& SuccessCallback, const FOnPlayerStateAuthError& ErrorCallback, const FOnFindSessionUpdate& MessageUpdateCallback)
{
	if (!CancelFindSession) {
		if (!GetWorldTimerManager().IsTimerActive(TimerHandle)) {
			//Binding the function with specific values
			TimerDel.BindUObject(this, &ARWSManagerPlayerState::MatchmakingMessage, MessageUpdateCallback);
			//Calling MatchmakingMessage for displaying matchmaking countdown.
			GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 0.95, true, 0);
		}

		const bool IsInHosted = false;
		TSharedPtr<FJsonObject> RequestDataJson = MakeShareable(new FJsonObject());
		RequestDataJson->SetStringField(TEXT("MapName"), MapName);
		RequestDataJson->SetStringField(TEXT("GameMode"), GameMode);
		RequestDataJson->SetStringField(TEXT("GameRegion"), GameRegion);
		RequestDataJson->SetStringField(TEXT("GameType"), GameType);
		RequestDataJson->SetBoolField(TEXT("IsHosted"), IsInHosted);

		//Get Project Version
		FString ProjectVersion;
		GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), ProjectVersion, GGameIni);
		if (ProjectVersion != "") { RequestDataJson->SetStringField(TEXT("server_version"), ProjectVersion); }

		SIsHosted = IsInHosted;
		SMapName = MapName;
		SGameType = GameType;
		SGameRegion = GameRegion;
		SGameMode = GameMode;

		FString PostContent;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PostContent);
		FJsonSerializer::Serialize(RequestDataJson.ToSharedRef(), Writer);

		// Generate endpoint url
		const URWSManagerSettings* Settings = FRWSManager::Get().GetSettings();
		const FString Endpoint = TEXT("https://reversewinterstudios.com/game/api/v1/server/find_session");
		const FString Url = RWSUtilsUrlBuilder(Endpoint)
			.AddStringQueryParam(TEXT("game_id"), Settings->ProjectID)
			.AddStringQueryParam(TEXT("login_project_id"), Settings->LoginID)
			.Build();

		TSharedRef<IHttpRequest> HttpRequest = CreateHttpRequest(Url, ERWSHttpRequestVerb::VERB_POST, PostContent);
		HttpRequest->OnProcessRequestComplete().BindUObject(this, &ARWSManagerPlayerState::RWSFindSession_HttpRequestComplete, SuccessCallback, ErrorCallback, MessageUpdateCallback);
		HttpRequest->ProcessRequest();
	}
}

void ARWSManagerPlayerState::RWSFindSession_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded,
	FOnFindSession SuccessCallback, FOnPlayerStateAuthError ErrorCallback, FOnFindSessionUpdate MessageUpdateCallback)
{
	TSharedPtr<FJsonObject> JsonObject;
	RWSHttpRequestError OutError;
	if (RWSUtilsHttpRequestHelper::ParseResponseAsJson(HttpRequest, HttpResponse, bSucceeded, JsonObject, OutError))
	{
		static const FString ServerIpContent = TEXT("content");
		if (JsonObject->HasTypedField<EJson::String>(ServerIpContent))
		{
			const FString Content = JsonObject.Get()->GetStringField(ServerIpContent);
			TArray<FString> ServerList;
			Content.ParseIntoArray(ServerList, TEXT(";"), true);
			if (ServerList.Num() > 0)
			{
				if (ServerList.Num() > 0)
				{
					int32 LastIndex = ServerList.Num() - 1;
					for (int32 i = 0; i <= LastIndex; ++i)
					{
						int32 Index = FMath::RandRange(i, LastIndex);
						if (i != Index)
						{
							ServerList.Swap(i, Index);
						}
					}
					FString Server = FString(ServerList[0]);

					GetWorldTimerManager().ClearTimer(TimerHandle);

					//Set the server found for updating later.
					Player_Matchmaking_Server_Found = Server;
					SuccessCallback.ExecuteIfBound(Server);

					UE_LOG(LogRWSManager, Warning, TEXT("Found in RWS: %s"), *Content);
				}
			}
			RWSPlayerQueue(SMapName, SGameMode, SGameRegion, SGameType, "Searching", "add");
			return;
		}
		else
		{
			TSharedPtr<FJsonObject> ErrorObject = JsonObject.Get()->GetObjectField(TEXT("error"));
			OutError.description = ErrorObject.Get()->GetStringField(TEXT("description"));

			if (!GetWorldTimerManager().IsTimerActive(RetrySessionTimerHandle)) {
				//Binding the function with specific values
				TimerRetrySesssionDel.BindUObject(this, &ARWSManagerPlayerState::RWSRetryFindSession, SuccessCallback, ErrorCallback, MessageUpdateCallback, OutError);
				//Calling MatchmakingMessage for displaying matchmaking countdown.
				const URWSManagerSettings* Settings = FRWSManager::Get().GetSettings();
				GetWorldTimerManager().SetTimer(RetrySessionTimerHandle, TimerRetrySesssionDel, Settings->MatchmakingDelay, true, 0);
			}

		}
	}

	HandleRequestError(OutError, ErrorCallback);
}

void ARWSManagerPlayerState::RWSRetryFindSession(FOnFindSession SuccessCallback, FOnPlayerStateAuthError ErrorCallback, FOnFindSessionUpdate MessageUpdateCallback, RWSHttpRequestError OutError)
{
	const URWSManagerSettings* Settings = FRWSManager::Get().GetSettings();
	if (FindTries == Settings->MatchmakingRetries)
	{
		FindTries = 0;
		ARWSManagerPlayerState::RWSPostServer(SMapName, SGameMode, SGameRegion, SGameType);
		MessageUpdateCallback.ExecuteIfBound(Settings->MatchmakingErrorMessage);
		HandleRequestError(OutError, ErrorCallback);
	}
	else
	{
		ARWSManagerPlayerState::RWSFindSession(SMapName, SGameMode, SGameRegion, SGameType, SuccessCallback, ErrorCallback, MessageUpdateCallback);
	}
	FindTries++;
}

void ARWSManagerPlayerState::RWSCancelFindSession()
{
	CancelFindSession = true;
	FindTries = 0;
	TimerSec = 0;
	TimerMin = 0;
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void ARWSManagerPlayerState::MatchmakingMessage(FOnFindSessionUpdate MessageUpdateCallback) 
{
	const URWSManagerSettings* Settings = FRWSManager::Get().GetSettings();
	FString Message = " " + Settings->MatchmakingSearchingMessage;
	FString Sec;
	if (TimerSec < 10) { Sec = "0" + FString::FromInt(TimerSec); } else { Sec = FString::FromInt(TimerSec); }

	MessageUpdateCallback.ExecuteIfBound(FString::FromInt(TimerMin) + ":" + Sec + Message);

    TimerSec++;
	if (TimerSec >= 60) {TimerMin++;TimerSec = 0;}
	return;
}

void ARWSManagerPlayerState::RWSPostServer(const FString& MapName, const FString& GameMode, const FString& GameRegion, const FString& GameType)
{
	const FOnPostServer SuccessCallback;
	const FOnPlayerStateAuthError ErrorCallback;

	TSharedPtr<FJsonObject> RequestDataJson = MakeShareable(new FJsonObject());
	RequestDataJson->SetStringField(TEXT("MapName"), MapName);
	RequestDataJson->SetStringField(TEXT("GameMode"), GameMode);
	RequestDataJson->SetStringField(TEXT("GameRegion"), GameRegion);
	RequestDataJson->SetStringField(TEXT("GameType"), GameType);

	//Get Project Version
	FString ProjectVersion;
	GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), ProjectVersion, GGameIni);
	if (ProjectVersion != "") { RequestDataJson->SetStringField(TEXT("server_version"), ProjectVersion); }

	FString PostContent;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PostContent);
	FJsonSerializer::Serialize(RequestDataJson.ToSharedRef(), Writer);

	// Generate endpoint url
	const URWSManagerSettings* Settings = FRWSManager::Get().GetSettings();
	const FString Endpoint = TEXT("https://reversewinterstudios.com/game/api/v1/server/post_host_request");
	const FString Url = RWSUtilsUrlBuilder(Endpoint)
		.AddStringQueryParam(TEXT("game_id"), Settings->ProjectID)
		.AddStringQueryParam(TEXT("login_project_id"), Settings->LoginID)
		.Build();

	TSharedRef<IHttpRequest> HttpRequest = CreateHttpRequest(Url, ERWSHttpRequestVerb::VERB_POST, PostContent);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ARWSManagerPlayerState::RWSPostServer_HttpRequestComplete, SuccessCallback, ErrorCallback);
	HttpRequest->ProcessRequest();
}

void ARWSManagerPlayerState::RWSPlayerQueue(const FString& MapName, const FString& GameMode, const FString& GameRegion, const FString& GameType, const FString& Status, const FString& Type)
{
	const FOnRequestSuccessPlayerState SuccessCallback;
	const FOnPlayerStateAuthError ErrorCallback;

	TSharedPtr<FJsonObject> RequestDataJson = MakeShareable(new FJsonObject());
	RequestDataJson->SetStringField(TEXT("MapName"), MapName);
	RequestDataJson->SetStringField(TEXT("GameMode"), GameMode);
	RequestDataJson->SetStringField(TEXT("GameRegion"), GameRegion);
	RequestDataJson->SetStringField(TEXT("GameType"), GameType);

	//Get Status can be 'Searching', 'Joining', 'Joined', 'Left'
	RequestDataJson->SetStringField(TEXT("status"), Status);

	//Get Project Version
	FString ProjectVersion;
	GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), ProjectVersion, GGameIni);
	if (ProjectVersion != "") { RequestDataJson->SetStringField(TEXT("server_version"), ProjectVersion); }

	//If contains a unique id then set the id
	URWSManagerGameInstance* RWSGameInstance = Cast<URWSManagerGameInstance>(GetGameInstance());
	if (RWSGameInstance->Unique_Player_Matchmaking_ID != "" && RWSGameInstance) { RequestDataJson->SetStringField(TEXT("unique_id"), RWSGameInstance->Unique_Player_Matchmaking_ID); }

	//If contains a Matchmaking Server if it does it will trade it in for a ServerID
	if (Player_Matchmaking_Server_Found != "") { RequestDataJson->SetStringField(TEXT("server_found"), Player_Matchmaking_Server_Found); }

	FString PostContent;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PostContent);
	FJsonSerializer::Serialize(RequestDataJson.ToSharedRef(), Writer);

	// Generate endpoint url
	const URWSManagerSettings* Settings = FRWSManager::Get().GetSettings();
	const FString Endpoint = TEXT("https://reversewinterstudios.com/game/api/v1/server/player_queue");
	//type query can be 'add', 'update', 'get_queue'.
	const FString Url = RWSUtilsUrlBuilder(Endpoint)
		.AddStringQueryParam(TEXT("game_id"), Settings->ProjectID)
		.AddStringQueryParam(TEXT("login_project_id"), Settings->LoginID)
		.AddStringQueryParam(TEXT("type"), Type)
		.Build();

	TSharedRef<IHttpRequest> HttpRequest = CreateHttpRequest(Url, ERWSHttpRequestVerb::VERB_POST, PostContent);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ARWSManagerPlayerState::RWSPlayerQueue_HttpRequestComplete, SuccessCallback, ErrorCallback);
	HttpRequest->ProcessRequest();
}

void ARWSManagerPlayerState::RWSPlayerQueue_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded,
	FOnRequestSuccessPlayerState SuccessCallback, FOnPlayerStateAuthError ErrorCallback)
{
	TSharedPtr<FJsonObject> JsonObject;
	RWSHttpRequestError OutError;

	if (RWSUtilsHttpRequestHelper::ParseResponseAsJson(HttpRequest, HttpResponse, bSucceeded, JsonObject, OutError))
	{
		static const FString ServerIpContent = TEXT("content");
		if (JsonObject->HasTypedField<EJson::String>(ServerIpContent))
		{
			const FString Content = JsonObject.Get()->GetStringField(ServerIpContent);

			TSharedPtr<FJsonObject> ContentObject = JsonObject.Get()->GetObjectField(TEXT("player"));

			Unique_Player_Matchmaking_ID = ContentObject.Get()->GetStringField(TEXT("unique_id"));

			if (Unique_Player_Matchmaking_ID != "") {
				URWSManagerGameInstance* RWSGameInstance = Cast<URWSManagerGameInstance>(GetGameInstance());
				RWSGameInstance->Unique_Player_Matchmaking_ID = Unique_Player_Matchmaking_ID;
			}

			SuccessCallback.ExecuteIfBound();
			return;
		}
		TSharedPtr<FJsonObject> ErrorObject = JsonObject.Get()->GetObjectField(TEXT("error"));
		OutError.description = ErrorObject.Get()->GetStringField(TEXT("description"));
	}

	HandleRequestError(OutError, ErrorCallback);
}

void ARWSManagerPlayerState::RWSPostServer_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded,
	FOnPostServer SuccessCallback, FOnPlayerStateAuthError ErrorCallback)
{
	TSharedPtr<FJsonObject> JsonObject;
	RWSHttpRequestError OutError;

	if (RWSUtilsHttpRequestHelper::ParseResponseAsJson(HttpRequest, HttpResponse, bSucceeded, JsonObject, OutError))
	{
		static const FString ServerIpContent = TEXT("content");
		if (JsonObject->HasTypedField<EJson::String>(ServerIpContent))
		{
			const FString Content = JsonObject.Get()->GetStringField(ServerIpContent);

			SuccessCallback.ExecuteIfBound(SMapName, SGameMode, SGameType);
			UE_LOG(LogRWSManager, Warning, TEXT("Found in RWS: %s"), *Content);
			return;
		}
		TSharedPtr<FJsonObject> ErrorObject = JsonObject.Get()->GetObjectField(TEXT("error"));
		OutError.description = ErrorObject.Get()->GetStringField(TEXT("description"));
	}

	HandleRequestError(OutError, ErrorCallback);
}

void ARWSManagerPlayerState::SetStringArrayField(TSharedPtr<FJsonObject> Object, const FString& FieldName, const TArray<FString>& Array) const
{
	TArray<TSharedPtr<FJsonValue>> StringJsonArray;
	for (auto Item : Array)
	{
		StringJsonArray.Push(MakeShareable(new FJsonValueString(Item)));
	}

	Object->SetArrayField(FieldName, StringJsonArray);
}

void ARWSManagerPlayerState::HandleRequestError(RWSHttpRequestError ErrorData, FOnPlayerStateAuthError ErrorCallback)
{
	UE_LOG(LogRWSManager, Error, TEXT("%s: request failed - Error code: %s, Error message: %s"), *VA_FUNC_LINE, *ErrorData.code, *ErrorData.description);
	ErrorCallback.ExecuteIfBound(ErrorData.code, ErrorData.description);
}

TSharedRef<IHttpRequest> ARWSManagerPlayerState::CreateHttpRequest(const FString& Url, const ERWSHttpRequestVerb Verb, const FString& Content, const FString& AuthToken)
{
	return RWSUtilsHttpRequestHelper::CreateHttpRequest(Url, Verb, AuthToken, Content, TEXT("SERVER_MANAGER"), RWS_MANAGER_VERSION);
}