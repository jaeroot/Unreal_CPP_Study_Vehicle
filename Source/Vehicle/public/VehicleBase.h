// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VehicleBase.generated.h"

class UArrowComponent;
class USpringArmComponent;
class UCameraComponent;
class UAudioComponent;

UCLASS()
class VEHICLE_API AVehicleBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVehicleBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float GetVehicleSpeed() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

private:
	void UpdateVehicleForce(int WheelArrowIndex, float DeltaTime);
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void HandbrakePressed();
	void HandbrakeReleased();
	
	void OnBoostPressed();
	void OnBoostReleased();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BodyMeshC;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent* ArrowC_FR;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent* ArrowC_FL;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent* ArrowC_RR;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent* ArrowC_RL;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* WheelScene_FR;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* WheelScene_FL;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* WheelScene_RR;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* WheelScene_RL;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAudioComponent* AC_Engine;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAudioComponent* AC_Boost;
	


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RestLength = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpringTravelLength = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float WheelRadius = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpringForceConst = 50000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamperForceConst = 5000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ForwardForceConst = 100000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxSteeringAngle = 30;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FrictionConst = 500;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BrakeConst = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BoostForceConst = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundWave* SW_Engine;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundWave* SW_Boost;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bBrakeApplied = false;


private:
	TArray<UArrowComponent*> WheelArrowComponentHolder;
	TArray<USceneComponent*> WheelSceneComponentHolder;
	float MinLength;
	float MaxLength;
	FCollisionQueryParams LineTraceCollisionQuery;
	float SpringLength[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	float ForwardAxisValue;
	float RightAxisValue;
	float bBoost = false;
};
