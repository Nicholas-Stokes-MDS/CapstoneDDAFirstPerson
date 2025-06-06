// Fill out your copyright notice in the Description page of Project Settings.


#include "DDA.h"

// Sets default values
ADDA::ADDA()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADDA::BeginPlay()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Hello from C++!"));
    Super::BeginPlay();
    
}

void ADDA::RecalculateHealthShortfallProbability()
{
    if (EncounterHistory.Num() == 0)
    {
        HealthShortfallProbability = 0.0f;
        return;
    }

    float Sum = 0.0f;
    for (const FEncounterRecord& Rec : EncounterHistory)
    {
        Sum += Rec.DamageTaken;
    }

    float Mean = Sum / EncounterHistory.Num();

    float Variance = 0.0f;
    for (const FEncounterRecord& Rec : EncounterHistory)
    {
        float Diff = Rec.DamageTaken - Mean;
        Variance += Diff * Diff;
    }

    float StdDev = FMath::Sqrt(Variance / EncounterHistory.Num());

    // Calculate total heal from packs (capped at 100 HP max)
    float PotentialHeal = HealthPacks * 40.0f;
    float EffectiveHealth = FMath::Min(PlayerHealth + PotentialHeal, 100.0f);

    if (StdDev <= KINDA_SMALL_NUMBER)
    {
        HealthShortfallProbability = (EffectiveHealth < Mean) ? 1.0f : 0.0f;
        return;
    }

    float Z = (Mean - EffectiveHealth) / (StdDev * FMath::Sqrt(2.0f));
    HealthShortfallProbability = 0.5f * (1.0f + std::erf(Z));
}

// Called every frame
void ADDA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADDA::RecordEncounter(float DamageTaken)
{
	EncounterHistory.Add(FEncounterRecord(DamageTaken));
	RecalculateHealthShortfallProbability();
}

void ADDA::UpdatePlayerStatus(float NewHealth, int32 NewAmmo, int32 NewHealthPacks)
{
	PlayerHealth = NewHealth;
	PlayerAmmo = NewAmmo;
	HealthPacks = NewHealthPacks;
}

int32 ADDA::GetRecommendedHealthPacks()
{
    // Scale based on shortfall probability (0 to 1)
    // You can tweak the max drop amount here
    const int32 MaxPacksPerCrate = 3;

    // Simple linear scaling
    return FMath::Clamp(FMath::RoundToInt(HealthShortfallProbability * MaxPacksPerCrate), 0, MaxPacksPerCrate);
}