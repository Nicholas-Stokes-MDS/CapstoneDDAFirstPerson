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

    // artificial value
    RecordEncounter(30.f);

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

    FString Msg = FString::Printf(TEXT("Shortfall Probability: %.2f%%"), HealthShortfallProbability * 100.0f);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Msg);

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

void ADDA::NumCratesEnemiesLeft(int32 Crates, int32 Enemies)
{
    CratesLeft = Crates;
    EnemiesLeft = Enemies;
}

void ADDA::UpdatePlayerStatus(float NewHealth, int32 NewAmmo, int32 NewHealthPacks)
{
	PlayerHealth = NewHealth;
	PlayerAmmo = NewAmmo;
	HealthPacks = NewHealthPacks;
}

int32 ADDA::GetRecommendedHealthPacks()
{
    if (EncounterHistory.Num() < 1)
    {
        return 1;
    }

    if (HealthShortfallProbability > 0.9f)
    {
        return 3;
    }
    if (HealthShortfallProbability > 0.6f) 
    {
        return 2;
    }
    if (HealthShortfallProbability > 0.3f)
    {
        return 1;
    }
    return 0;
}

void ADDA::Recalculate()
{
    RecalculateHealthShortfallProbability();
}


int32 ADDA::GetRecommendedAmmo()
{
    float RecommendedAmmo = ((EnemiesLeft * 5) - PlayerAmmo + 2 ) / CratesLeft;
    int32 RoundedUp = FMath::CeilToInt(RecommendedAmmo);
    return RoundedUp;
}
