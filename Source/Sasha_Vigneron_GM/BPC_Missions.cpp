// Fill out your copyright notice in the Description page of Project Settings.


#include "BPC_Missions.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime/Core/Public/Misc/OutputDeviceNull.h>

// Sets default values for this component's properties
UBPC_Missions::UBPC_Missions()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USoundWave> soundAsset(TEXT("SoundWave'/Game/Sounds/MissionSucces.MissionSucces'"));
	if (soundAsset.Succeeded())
	{
		missionSucccesSound = soundAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundWave> soundAsset2(TEXT("SoundWave'/Game/Sounds/MissionFail.MissionFail'"));
	if (soundAsset2.Succeeded())
	{
		missionFailedSound = soundAsset2.Object;
	}
}


// Called when the game starts
void UBPC_Missions::BeginPlay()
{
	Super::BeginPlay();

	// ...
	#pragma region Missions
	// Mission 1
	FMission mission1 = FMission();
	mission1.requiredBalance = 11000.f;
	mission1.bonus = 1000.f;

	mission1.hasToPay = false;
	mission1.hasTimeLimit = false;
	mission1.timeLimit = 0.0f;
	mission1.penalty = 0.f;
	missions.push_back(mission1);

	// Mission 2
	FMission mission2 = FMission();
	mission2.requiredBalance = 15000.0f;
	mission2.bonus = 5000.f;

	mission2.hasToPay = true;
	mission2.hasTimeLimit = true;
	mission2.timeLimit = 60.0f;
	mission2.penalty = 30000.f;
	missions.push_back(mission2);

	// Mission 3
	FMission mission3 = FMission();
	mission3.requiredBalance = 15000.0f;
	mission3.bonus = 1000.f;

	mission3.hasToPay = false;
	mission3.hasTimeLimit = false;
	mission3.timeLimit = 0.0f;
	mission3.penalty = 0.f;
	missions.push_back(mission3);

	// Mission 4
	FMission mission4 = FMission();
	mission4.requiredBalance = 25000.0f;
	mission4.bonus = 5000.f;

	mission4.hasToPay = false;
	mission4.hasTimeLimit = false;
	mission4.timeLimit = 0.0f;
	mission4.penalty = 0.f;
	missions.push_back(mission4);

	// Mission 5
	FMission mission5 = FMission();
	mission5.requiredBalance = 50000.0f;
	mission5.bonus = 20000.f;

	mission5.hasToPay = true;
	mission5.hasTimeLimit = true;
	mission5.timeLimit = 120.0f;
	mission5.penalty = 40000.f;
	missions.push_back(mission5);

	// Mission 6
	FMission mission6 = FMission();
	mission6.requiredBalance = 150000.0f;
	mission6.bonus = 20000.f;

	mission6.hasToPay = true;
	mission6.hasTimeLimit = true;
	mission6.timeLimit = 60.0f;
	mission6.penalty = 150000.0f;
	missions.push_back(mission6);
	#pragma endregion

	currMissionAccTime = 0.0f;
	activeMissionIndex = 0;
	missionCount = missions.size();

	if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Current Mission Index: %i"), activeMissionIndex));
	if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Total Mission Count: %i"), missionCount));
}

// Called every frame
void UBPC_Missions::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if game over
	if (isGameOver) return;

	// if just completed mission
	if (isMissionCompleted) 
	{
		currMissionCompletedAccTime += DeltaTime;
		if (currMissionCompletedAccTime >= missionCompletedTime) 
		{
			currMissionCompletedAccTime = 0.0f;
			isMissionCompleted = false;
			LoadNextMission();
		}
	}
	// if just failed mission
	else if (isMissionFailed) 
	{
		currMissionCompletedAccTime += DeltaTime;
		if (currMissionCompletedAccTime >= missionCompletedTime)
		{
			currMissionCompletedAccTime = 0.0f;
			isMissionFailed = false;
			LoadNextMission();
		}
	}
	// if has current mission
	else if (!completedAllMissions && activeMissionIndex < missions.size()) 
	{
		FMission currMission = missions[activeMissionIndex];

		if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Cyan, FString::Printf(TEXT("Balance: %f"), balance));

		#pragma region CheckIfWon
		if (balance >= currMission.requiredBalance)
		{
			if (currMission.hasToPay)
			{
				if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("You Paid: %i $"), (int)currMission.requiredBalance));
				RemoveBalance(currMission.requiredBalance);
			}
			CompleteMission(currMission);
			return;
		}
		#pragma endregion

		#pragma region CheckTimeLimit
		if (currMission.hasTimeLimit)
		{
			currMissionAccTime += DeltaTime;

			if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, FString::Printf(TEXT("Mission Penalty: %f"), currMission.penalty));

			if (currMissionAccTime > currMission.timeLimit)
			{
				// Mission failed
				FailMission(currMission);
			}
		}
		#pragma endregion
	}
}

void UBPC_Missions::FailMission(FMission& currMission)
{
	if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("")));
	if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("You Paid: %i $"), (int)currMission.penalty));
	if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Mission Failed!")));

	RemoveBalance(currMission.penalty); 

	

	UGameplayStatics::PlaySound2D(this, missionFailedSound);
	isMissionFailed = true;
	failedMissions++;
}
void UBPC_Missions::CompleteMission(FMission& currMission)
{
	if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("")));
	if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("You Received: %i"), (int)currMission.bonus));
	if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Mission Completed!")));

	AddBalance(currMission.bonus);

	UGameplayStatics::PlaySound2D(this, missionSucccesSound);
	isMissionCompleted = true;
}
void UBPC_Missions::LoadNextMission()
{
	currMissionAccTime = 0.0f;
	activeMissionIndex++;

	if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Current Mission Index: %i"), activeMissionIndex));
	if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Total Mission Count: %i"), missionCount));

	if (activeMissionIndex >= missionCount)
	{
		completedAllMissions = true;
		activeMissionIndex = missionCount;
		if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("All missions completed!"));
	}
}

void UBPC_Missions::SetGameMode(AActor* value)
{
	gameMode = value;
}

void UBPC_Missions::SetBalance(float value)
{
	balance = value;
}
float UBPC_Missions::GetBalance()
{
	return balance;
}

void UBPC_Missions::SetGameOver(bool value)
{
	isGameOver = value;
}

void UBPC_Missions::ResetGame()
{
	isGameOver = false;
	activeMissionIndex = 0;
	currMissionAccTime = 0.f;
	isMissionFailed = false;
	isMissionCompleted = false;
	currMissionCompletedAccTime = 0.f;
	failedMissions = 0;
	completedAllMissions = false;
}

bool UBPC_Missions::GetIsMissionCompleted()
{
	return isMissionCompleted;
}

bool UBPC_Missions::GetIsMissionFailed()
{
	return isMissionFailed;
}

FString UBPC_Missions::GetCurrentMissionInfo()
{
#pragma region PrintMissionInfo

	// If just completed
	if (isMissionCompleted) 
	{
		FString string = FString("Mission Completed!\nYou received ");
		string.AppendInt((int)missions[activeMissionIndex].bonus);
		string.Append("$!");
		return string;
	}

	// If just failed
	if (isMissionFailed) 
	{
		FString string = FString("Mission Failed!\nYou paid ");
		string.AppendInt((int)missions[activeMissionIndex].penalty);
		string.Append("$!");
		return string;
	}

	// If missions completed
	if (activeMissionIndex == missionCount)
	{
		FString string = FString("Congratulations!\n");
		string.Append("You've completed ");
		string.AppendInt(missionCount - failedMissions);
		string.Append("/");
		string.AppendInt(missionCount);
		string.Append(" missions!");
		return string;
	}

	// Default Info
	FString string = FString();
	switch (activeMissionIndex)
	{
	case 0:
		if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Cyan, FString::Printf(TEXT("Reach %i $!"), (int)missions[activeMissionIndex].requiredBalance));
		if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Cyan, FString::Printf(TEXT("Active Mission Info:")));
		if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("")));

		string.Append("Reach: ");
		string.AppendInt((int)missions[activeMissionIndex].requiredBalance);
		string.Append("$!");
		break;
	case 1:
		if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Cyan, FString::Printf(TEXT("Electricity bill is coming, reach & pay %i $!"), (int)missions[activeMissionIndex].requiredBalance));
		if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Cyan, FString::Printf(TEXT("Active Mission Info:")));
		if (isDebugging) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("")));

		string.Append("Electricity bill is coming, reach & pay ");
		string.AppendInt((int)missions[activeMissionIndex].requiredBalance);
		string.Append("$!");
		break;
	case 2: 
		string.Append("Reach ");
		string.AppendInt((int)missions[activeMissionIndex].requiredBalance);
		string.Append("$!");
		break;
	case 3:
		string.Append("Reach ");
		string.AppendInt((int)missions[activeMissionIndex].requiredBalance);
		string.Append("$!");
		break;
	case 4:
		string.Append("Renting bill is coming, reach & pay ");
		string.AppendInt((int)missions[activeMissionIndex].requiredBalance);
		string.Append("$!");
		break;
	case 5:
		string.Append("Taxes are coming, reach & pay ");
		string.AppendInt((int)missions[activeMissionIndex].requiredBalance);
		string.Append("$!");
		break;
	default:

		break;
	}
#pragma endregion

	return string;
}

FString UBPC_Missions::GetCurrentMissionReward()
{
	// If missions completed
	if (activeMissionIndex == missionCount)
	{
		return FString("");
	}

	FString string = FString("Reward: ");
	string.AppendInt((int)missions[activeMissionIndex].bonus);
	string.Append("$");
	return string;
}

bool UBPC_Missions::GetCurrentMissionHasTimeLimit()
{
	// If missions completed
	if (activeMissionIndex == missionCount)
	{
		return false;
	}

	if (!isMissionCompleted) return missions[activeMissionIndex].hasTimeLimit;
	return false;
}

FString UBPC_Missions::GetCurrentMissionPenalty()
{
	// If missions completed
	if (activeMissionIndex == missionCount)
	{
		return FString("");
	}

	FString string = FString("Penalty: ");
	string.AppendInt((int)missions[activeMissionIndex].penalty);
	string.Append("$");
	return string;
}

FString UBPC_Missions::GetCurrentMissionTimeLeft()
{
	// If missions completed
	if (activeMissionIndex == missionCount)
	{
		return FString("");
	}

	FString string = FString("Time Remaining: ");
	string.AppendInt(int(missions[activeMissionIndex].timeLimit - currMissionAccTime));
	string.Append("s");
	return string;
}

void UBPC_Missions::AddBalance(float value)
{
	balance += value;
	UpdateGameModeBalance();
}
void UBPC_Missions::RemoveBalance(float value)
{
	balance -= value;
	UpdateGameModeBalance();
}
void UBPC_Missions::UpdateGameModeBalance()
{
	FOutputDeviceNull ar;
	const FString command = FString::Printf(TEXT("SetMissionBalance"));
	gameMode->CallFunctionByNameWithArguments(*command, ar, NULL, true);
}

