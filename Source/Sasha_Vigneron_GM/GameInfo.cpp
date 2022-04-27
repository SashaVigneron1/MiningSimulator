// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInfo.h"

// Sets default values for this component's properties
UGameInfo::UGameInfo()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGameInfo::BeginPlay()
{
	Super::BeginPlay();

	// ...
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, FString::Printf(TEXT("H: Open Rig Shop")));
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, FString::Printf(TEXT("ESCAPE: If you're looking at a computer, you will go back to freeroam.")));
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, FString::Printf(TEXT("CLICK ON COMPUTER: Get detailed information about the computer.")));
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, FString::Printf(TEXT("Controls:")));
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, FString::Printf(TEXT("")));
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, FString::Printf(TEXT("You're goal is to complete the missions, and become rich!")));
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, FString::Printf(TEXT("Goal:")));
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, FString::Printf(TEXT("")));
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, FString::Printf(TEXT("Welcome to my game!")));

}


// Called every frame
void UGameInfo::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

