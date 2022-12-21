// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#pragma once

#include "RWSUtilsDataModel.h"
#include "RWSUtilsHttpRequestHelper.h"
#include "TimerManager.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RWSManagerPlayerState.generated.h"

USTRUCT(BlueprintType)
struct FRegions
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWS|Server Manager|Data")
	FString region;
};

USTRUCT(BlueprintType)
struct FServers
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWS|Server Manager|Data")
	FString server_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWS|Server Manager|Data")
	FString ServerID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWS|Server Manager|Data")
	FString region;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWS|Server Manager|Data")
	FString MapName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWS|Server Manager|Data")
	FString Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWS|Server Manager|Data")
	FString Mode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWS|Server Manager|Data")
	FString IP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWS|Server Manager|Data")
	FString PortNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWS|Server Manager|Data")
	FString server_started;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWS|Server Manager|Data")
	FString Current_Players;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWS|Server Manager|Data")
	FString Max_Players;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWS|Server Manager|Data")
	FString ping;
};

/** Common callback for operations without any user-friendly messages from the server in case of success. */
DECLARE_DYNAMIC_DELEGATE(FOnRequestSuccessPlayerState);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnFindSession, const FString&, Address);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnPostServer, const FString&, MapName, const FString&, GameMode, const FString&, GameType);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAuthSuccessRegions, const TArray<FRegions>&, Regions);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnPlayerStateAuthError, const FString&, Code, const FString&, Description);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnFindSessionUpdate, const FString&, Message);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAuthSuccessServerBrowser, const TArray<FServers>&, Servers);

class FJsonObject;

UCLASS()
class RWSMANAGER_API ARWSManagerPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWS|Server Manager|Session")
		bool CancelFindSession;

	UFUNCTION(BlueprintCallable, Category = "RWS|Server Manager|Session", meta = (AutoCreateRefTerm = "SuccessCallback, ErrorCallback, MessageUpdateCallback"))
		void RWSFindSession(const FString& MapName, const FString& GameMode, const FString& GameRegion, const FString& GameType, const FOnFindSession& SuccessCallback, const FOnPlayerStateAuthError& ErrorCallback, const FOnFindSessionUpdate& MessageUpdateCallback);
	
	UFUNCTION(BlueprintCallable, Category = "RWS|Server Manager|Session", meta = (AutoCreateRefTerm = "SuccessCallback, ErrorCallback"))
	void RWSServerBrowser(const FString& MapName, const FString& GameMode, const FString& GameRegion, const FString& ServerName, const FOnAuthSuccessServerBrowser& SuccessCallback, const FOnPlayerStateAuthError& ErrorCallback);

	UFUNCTION(BlueprintCallable, Category = "RWS|Server Manager|Session")
	void RWSCancelFindSession();

	void RWSRetryFindSession(FOnFindSession SuccessCallback, FOnPlayerStateAuthError ErrorCallback, FOnFindSessionUpdate MessageUpdateCallback, RWSHttpRequestError OutError);

	void RWSPostServer(const FString& MapName, const FString& GameMode, const FString& GameRegion, const FString& GameType);
	void RWSPlayerQueue(const FString& MapName, const FString& GameMode, const FString& GameRegion, const FString& GameType, const FString& Status, const FString& Type);

	/** RWS Join Session
	*
	* Join a normal session by an address.
	*
	*/
	UFUNCTION(BlueprintCallable, Category = "RWS|Server Manager|Data", meta = (AutoCreateRefTerm = "MessageUpdateCallback"))
	void RWSJoinSession(const FString& Address, const FOnFindSessionUpdate& MessageUpdateCallback);

	//Get All Regions Of Server
	UFUNCTION(BlueprintCallable, Category = "RWS|Server Manager|Data", meta = (AutoCreateRefTerm = "SuccessCallback, ErrorCallback"))
		void RWSGetRegions(const FOnAuthSuccessRegions& SuccessCallback, const FOnPlayerStateAuthError& ErrorCallback);

	void RWSGetRegions_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, FOnAuthSuccessRegions SuccessCallback, FOnPlayerStateAuthError ErrorCallback);

protected:
	/** Checks RWSFindSession for any errors or if its success it will pass the information back */
	void RWSFindSession_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded,
		FOnFindSession SuccessCallback, FOnPlayerStateAuthError ErrorCallback, FOnFindSessionUpdate MessageUpdateCallback);

	/** Checks RWSServerBrowser for any errors or if its success it will pass the information back */
	void RWSServerBrowser_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, FOnAuthSuccessServerBrowser SuccessCallback, FOnPlayerStateAuthError ErrorCallback);

	/** Checks RWSFindSession for any errors or if its success it will pass the information back */
	void RWSPostServer_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded,
		FOnPostServer SuccessCallback, FOnPlayerStateAuthError ErrorCallback);

	/** Checks RWSFindSession for any errors or if its success it will pass the information back */
	void RWSPlayerQueue_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded,
		FOnRequestSuccessPlayerState SuccessCallback, FOnPlayerStateAuthError ErrorCallback);

	/** Returns true if the error occurs. */
	void HandleRequestError(RWSHttpRequestError ErrorData, FOnPlayerStateAuthError ErrorCallback);

public:
	int32 FindTries = 1;
	FString SMapName;
	bool SIsHosted;
	FString SGameType;
	FString SGameMode;
	FString SGameRegion;
	FTimerHandle handle;
	FString Unique_Player_Matchmaking_ID;
	FString Player_Matchmaking_Server_Found;

	TArray<FServers> ServerArray;


	int32 TimerSec = 0;
	int32 TimerMin = 0;
	FTimerDelegate TimerDel;
	FTimerHandle TimerHandle;
	void MatchmakingMessage(FOnFindSessionUpdate MessageUpdateCallback);

	FTimerDelegate TimerRetrySesssionDel;
	FTimerHandle RetrySessionTimerHandle;

private:
	/** Sets a JSON string array field named FieldName and value of Array. */
	void SetStringArrayField(TSharedPtr<FJsonObject> Object, const FString& FieldName, const TArray<FString>& Array) const;

	/** Create http request and add RWS API meta */
	TSharedRef<IHttpRequest> CreateHttpRequest(const FString& Url, const ERWSHttpRequestVerb Verb = ERWSHttpRequestVerb::VERB_GET,
		const FString& Content = FString(), const FString& AuthToken = FString());

	void BeginPlay() override;
};
