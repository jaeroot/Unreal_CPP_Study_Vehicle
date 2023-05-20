// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleBase.h"

#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AVehicleBase::AVehicleBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMeshC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshC"));
	RootComponent = BodyMeshC;
	if (UStaticMesh* CarMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/Assets/Meshes/CarUpper.CarUpper'"))))
	{
		BodyMeshC->SetStaticMesh(CarMesh);
	}

	AC_Engine = CreateDefaultSubobject<UAudioComponent>(TEXT("AC_Engine"));
	AC_Engine->SetupAttachment(BodyMeshC);
	AC_Boost = CreateDefaultSubobject<UAudioComponent>(TEXT("AC_Boost"));
	AC_Boost->SetupAttachment(BodyMeshC);
	AC_Boost->SetAutoActivate(false);
	
	ArrowC_FR = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowC_FR"));
	ArrowC_FR->SetupAttachment(BodyMeshC, FName("WheelFR"));
	ArrowC_FR->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	ArrowC_FL = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowC_FL"));
	ArrowC_FL->SetupAttachment(BodyMeshC, FName("WheelFL"));
	ArrowC_FL->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	ArrowC_RR = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowC_RR"));
	ArrowC_RR->SetupAttachment(BodyMeshC, FName("WheelRR"));
	ArrowC_RR->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	ArrowC_RL = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowC_RL"));
	ArrowC_RL->SetupAttachment(BodyMeshC, FName("WheelRL"));
	ArrowC_RL->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	WheelScene_FR = CreateDefaultSubobject<USceneComponent>(TEXT("WheelScene_FR"));
	WheelScene_FR->SetupAttachment(ArrowC_FR);
	WheelScene_FL = CreateDefaultSubobject<USceneComponent>(TEXT("WheelScene_FL"));
	WheelScene_FL->SetupAttachment(ArrowC_FL);
	WheelScene_RR = CreateDefaultSubobject<USceneComponent>(TEXT("WheelScene_RR"));
	WheelScene_RR->SetupAttachment(ArrowC_RR);
	WheelScene_RL = CreateDefaultSubobject<USceneComponent>(TEXT("WheelScene_RL"));
	WheelScene_RL->SetupAttachment(ArrowC_RL);

	SpringArmC = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmC"));
	SpringArmC->SetupAttachment(BodyMeshC);
	SpringArmC->TargetArmLength = 600.0f;
	SpringArmC->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));
	SpringArmC->SetRelativeRotation(FRotator(-10.0f, 0.0f, 0.0f));

	CameraC = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraC"));
	CameraC->SetupAttachment(SpringArmC);
	
	BodyMeshC->SetSimulatePhysics(true);
	BodyMeshC->SetMassOverrideInKg(NAME_None, 1153.0f);
}

// Called when the game starts or when spawned
void AVehicleBase::BeginPlay()
{
	Super::BeginPlay();

	WheelArrowComponentHolder = {ArrowC_FR, ArrowC_FL, ArrowC_RR, ArrowC_RL};
	WheelSceneComponentHolder = {WheelScene_FR, WheelScene_FL, WheelScene_RR, WheelScene_RL};
	MinLength = RestLength - SpringTravelLength;
	MaxLength = RestLength + SpringTravelLength;

	const FName TraceTag("MyTraceTag");
	LineTraceCollisionQuery.TraceTag = TraceTag;
	LineTraceCollisionQuery.bDebugQuery = true;
	LineTraceCollisionQuery.AddIgnoredActor(this);
	
	if (SW_Engine)
	{
		AC_Engine->SetSound(SW_Engine);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Undefined Engine sound"));
	}
	
	if (SW_Boost)
	{
		AC_Boost->SetSound(SW_Boost);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Undefined boost sound"));
	}
}

void AVehicleBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AVehicleBase::UpdateVehicleForce(int WheelArrowIndex, float DeltaTime)
{
	if (!WheelArrowComponentHolder.IsValidIndex(WheelArrowIndex))
	{
		return;
	}
	
	auto WheelArrowC = WheelArrowComponentHolder[WheelArrowIndex];
	FHitResult OutHit;
	FVector StartTraceLoc = WheelArrowC->GetComponentLocation();
	FVector EndTraceLoc = WheelArrowC->GetForwardVector() * (MaxLength + WheelRadius) + StartTraceLoc;	
	bool bResult = GetWorld()->LineTraceSingleByChannel(OutHit, StartTraceLoc, EndTraceLoc, ECC_Camera, LineTraceCollisionQuery, FCollisionResponseParams());
	float PreviousSpringLength = SpringLength[WheelArrowIndex];

	FVector WorldLinearVelocity = BodyMeshC->GetPhysicsLinearVelocity();
	FVector LocalLinearVelocity = UKismetMathLibrary::InverseTransformDirection(BodyMeshC->GetComponentTransform(), WorldLinearVelocity);
	float CurrentSteeringAngle = UKismetMathLibrary::MapRangeClamped(RightAxisValue, -1, 1, MaxSteeringAngle * (-1), MaxSteeringAngle);

# if ENABLE_DRAW_DEBUG
	FColor drawcolor = bResult ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), StartTraceLoc, EndTraceLoc, drawcolor, false, 5.0f);
# endif
	
	if (OutHit.IsValidBlockingHit())
	{
		float CurrentSpringLength = OutHit.Distance - WheelRadius;
		SpringLength[WheelArrowIndex] = UKismetMathLibrary::FClamp(CurrentSpringLength, MinLength, MaxLength);
		float SpringVelocity = (PreviousSpringLength - SpringLength[WheelArrowIndex]) / DeltaTime;
		float SpringForce = (RestLength - SpringLength[WheelArrowIndex]) * SpringForceConst;
		float DamperForce = SpringVelocity * DamperForceConst;
		FVector UpwardForce = GetActorUpVector() * (SpringForce + DamperForce);
		BodyMeshC->AddForceAtLocation(UpwardForce, WheelArrowC->GetComponentLocation());
		
		if (WheelArrowIndex < 2)
		{
			WheelSceneComponentHolder[WheelArrowIndex]->SetRelativeRotation(FRotator(0.0f, 0.0f, CurrentSteeringAngle));
		}
	
		if (WorldLinearVelocity.SizeSquared() > 1)
		{
			BodyMeshC->AddTorqueInDegrees(FVector(0.0f, 0.0f, CurrentSteeringAngle), NAME_None, true);
		}
	}
	else
	{
		SpringLength[WheelArrowIndex] = MaxLength;
	}

	

	FVector FrictionVector = FVector::ZeroVector;
	if (UKismetMathLibrary::Abs(LocalLinearVelocity.Y) > 2)
	{
		FrictionVector = BodyMeshC->GetRightVector() * LocalLinearVelocity.Y * FrictionConst * (-1);
	}

	FVector VehicleForwardForce = GetActorForwardVector() * ForwardAxisValue * ForwardForceConst + FrictionVector;

	if (bBrakeApplied)
	{
		VehicleForwardForce = WorldLinearVelocity * (-1) * BrakeConst;
	}
	
	BodyMeshC->AddForce(VehicleForwardForce);

	if (bBoost)
	{
		BodyMeshC->AddImpulse(BodyMeshC->GetForwardVector() * BoostForceConst, NAME_None, true);
	}
	
	WheelSceneComponentHolder[WheelArrowIndex]->SetRelativeLocation(FVector(SpringLength[WheelArrowIndex], 0.0f, 0.0f));
	WheelSceneComponentHolder[WheelArrowIndex]->GetChildComponent(0)->AddLocalRotation(FRotator(((-1) * 360 * LocalLinearVelocity.X * DeltaTime) / (2 * 3.14 * WheelRadius), 0.0f, 0.0f));
	// WheelSceneComponentHolder[WheelArrowIndex]->AddLocalRotation(FRotator(((-1) * 360 * LocalLinearVelocity.X * DeltaTime) / (2 * 3.14 * WheelRadius), 0.0f, 0.0f));
}

void AVehicleBase::MoveForward(float Value)
{
	ForwardAxisValue = Value;
}

void AVehicleBase::MoveRight(float Value)
{
	if (BodyMeshC->GetPhysicsLinearVelocity().Size() * 0.036 > 3)
	{
		RightAxisValue = UKismetMathLibrary::FInterpTo(RightAxisValue, Value, GetWorld()->GetDeltaSeconds(), 5);
	}
	else
	{
		RightAxisValue = 0;
	}
}

void AVehicleBase::HandbrakePressed()
{
	bBrakeApplied = true;
}

void AVehicleBase::HandbrakeReleased()
{
	bBrakeApplied = false;
}

void AVehicleBase::OnBoostPressed()
{
	bBoost = true;
	AC_Boost->Activate();
}

void AVehicleBase::OnBoostReleased()
{
	bBoost = false;
	AC_Boost->Deactivate();
}

// Called every frame
void AVehicleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// UE_LOG(LogTemp, Warning, TEXT("Your Speed is %lf"), BodyMeshC->GetPhysicsLinearVelocity().Size() * 0.036);
	AC_Engine->SetPitchMultiplier(UKismetMathLibrary::MapRangeClamped(GetVehicleSpeed(), 0, 100, 1, 4));
	for (int WheelArrowIndex = 0; WheelArrowIndex < 4; WheelArrowIndex++)
	{
		UpdateVehicleForce(WheelArrowIndex, DeltaTime);
	}
}

// Called to bind functionality to input
void AVehicleBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AVehicleBase::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AVehicleBase::MoveRight);
	PlayerInputComponent->BindAction(FName("Handbrake"), EInputEvent::IE_Pressed, this, &AVehicleBase::HandbrakePressed);
	PlayerInputComponent->BindAction(FName("Handbrake"), EInputEvent::IE_Released, this, &AVehicleBase::HandbrakeReleased);
	PlayerInputComponent->BindAction(FName("Boost"), EInputEvent::IE_Pressed, this, &AVehicleBase::OnBoostPressed);
	PlayerInputComponent->BindAction(FName("Boost"), EInputEvent::IE_Released, this, &AVehicleBase::OnBoostReleased);
}

float AVehicleBase::GetVehicleSpeed() const
{
	return BodyMeshC->GetPhysicsLinearVelocity().Size() * 0.036;
}

