// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <cmath>
#include "DDA.generated.h"

USTRUCT(BlueprintType)
struct FEncounterRecord
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DamageTaken = 0.0f;

	FEncounterRecord() {}
	FEncounterRecord(float InDamageTaken) : DamageTaken(InDamageTaken) {}
};


UCLASS()
class CAPSTONEFIRSTPERSON_API ADDA : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADDA();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<float> DamageHistory;

	UPROPERTY(VisibleAnywhere)
	TArray<FEncounterRecord> EncounterHistory;

	UPROPERTY(VisibleAnywhere)
	float PlayerHealth = 100.0f;

	UPROPERTY(VisibleAnywhere)
	int32 PlayerAmmo = 0;

	UPROPERTY(VisibleAnywhere)
	int32 CratesLeft = 0;

	UPROPERTY(VisibleAnywhere)
	int32 EnemiesLeft = 0;

	UPROPERTY(VisibleAnywhere)
	int32 HealthPacks = 0;

	void RecalculateHealthShortfallProbability();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "DDA")
	void RecordEncounter(float DamageTaken);

	UFUNCTION(BlueprintCallable, Category = "DDA")
	void NumCratesEnemiesLeft(int32 Crates, int32 Enemies);

	UFUNCTION(BlueprintCallable, Category = "DDA")
	void UpdatePlayerStatus(float NewHealth, int32 NewAmmo, int32 NewHealthPacks);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DDA")
	float HealthShortfallProbability;

	// How many health packs should the next crate contain
	UFUNCTION(BlueprintCallable, Category = "DDA")
	int32 GetRecommendedHealthPacks();


	// How many health packs should the next crate contain
	UFUNCTION(BlueprintCallable, Category = "DDA")
	int32 GetRecommendedAmmo();
};
