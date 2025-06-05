// Fill out your copyright notice in the Description page of Project Settings.


#include "DDA.h"

// Sets default values
ADDA::ADDA()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADDA::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADDA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADDA::Initialize(ACapstoneFirstPersonCharacter* InPlayer)
{
	Player = InPlayer;
}

void ADDA::RecordDamage(float DamageAmount)
{
	if (DamageAmount <= 0.f)
		return;

	DamageHistory.Add(DamageAmount);

	// Optional: Limit to last N entries
	if (DamageHistory.Num() > 100)
	{
		DamageHistory.RemoveAt(0);
	}
}



void ADDA::GetDamageStats(float& OutMean, float& OutStdDev) const
{
	OutMean = 0.f;
	OutStdDev = 0.f;

	int32 Count = DamageHistory.Num();
	if (Count == 0)
		return;

	// Mean
	for (float Value : DamageHistory)
	{
		OutMean += Value;
	}
	OutMean /= Count;

	// Standard deviation
	float SumSqDiff = 0.f;
	for (float Value : DamageHistory)
	{
		SumSqDiff += FMath::Square(Value - OutMean);
	}
	OutStdDev = FMath::Sqrt(SumSqDiff / Count);
}

// shows the probability that a value is less than or equal to x
float ADDA::GaussianCDF(float x, float mean, float stdDev)
{
	float z = (x - mean) / (stdDev * FMath::Sqrt(2.0f));
	return 0.5f * (1.0f + std::erf(z));
}
