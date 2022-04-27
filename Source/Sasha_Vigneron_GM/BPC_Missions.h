// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <vector>
#include <string>
#include "BPC_Missions.generated.h"

USTRUCT(BlueprintType)
struct FMission 
{
	GENERATED_BODY()

	bool hasToPay;
	float requiredBalance;
	float bonus;

	bool hasTimeLimit;
	float timeLimit;
	float penalty;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SASHA_VIGNERON_GM_API UBPC_Missions : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBPC_Missions();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missions")
	int activeMissionIndex;

	AActor* gameMode;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	USoundBase* missionSucccesSound;
	USoundBase* missionFailedSound;


	bool isGameOver = false;

	bool isDebugging = false;

	float balance;
	int missionCount;

	float currMissionAccTime;

	bool isMissionFailed;
	bool isMissionCompleted;
	float missionCompletedTime = 2.0f;
	float currMissionCompletedAccTime;

	int failedMissions = 0;

	bool completedAllMissions = false;
	std::vector<FMission> missions;

	void AddBalance(float value);
	void RemoveBalance(float value);
	void UpdateGameModeBalance();
	void LoadNextMission();


	void FailMission(FMission& currMission);
	void CompleteMission(FMission& currMission);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Missions")
	void SetGameMode(AActor* value);

	UFUNCTION(BlueprintCallable, Category = "Missions")
	void SetBalance(float value);

	UFUNCTION(BlueprintCallable, Category = "Missions")
	float GetBalance();

	UFUNCTION(BlueprintCallable, Category = "Missions")
	void SetGameOver(bool value);
	UFUNCTION(BlueprintCallable, Category = "Missions")
	void ResetGame();

public:


	UFUNCTION(BlueprintCallable, Category = "Missions")
	bool GetIsMissionCompleted();

	UFUNCTION(BlueprintCallable, Category = "Missions")
	bool GetIsMissionFailed();







	UFUNCTION(BlueprintCallable, Category = "Missions")
	FString GetCurrentMissionInfo();

	UFUNCTION(BlueprintCallable, Category = "Missions")
	FString GetCurrentMissionReward();


	UFUNCTION(BlueprintCallable, Category = "Missions")
	bool GetCurrentMissionHasTimeLimit();

	UFUNCTION(BlueprintCallable, Category = "Missions")
	FString GetCurrentMissionPenalty();

	UFUNCTION(BlueprintCallable, Category = "Missions")
	FString GetCurrentMissionTimeLeft();

};
