// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <cmath>
#include "DDA.generated.h"


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
	ACapstoneFirstPersonCharacter* Player;

	UPROPERTY()
	TArray<float> DamageHistory;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Damage Tracking")
	void Initialize(ACapstoneFirstPersonCharacter* InPlayer);

	// Call this whenever the player takes damage
	UFUNCTION(BlueprintCallable, Category = "Damage Tracking")
	void RecordDamage(float DamageAmount);

	// Returns current mean and standard deviation
	UFUNCTION(BlueprintCallable, Category = "Damage Tracking")
	void GetDamageStats(float& OutMean, float& OutStdDev) const;

	UFUNCTION(BlueprintCallable, Category = "Damage Tracking")
	float GaussianCDF(float x, float mean, float stdDev);
};
