#include "Drone.h"
#include <Components/SphereComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <EnhancedInputComponent.h>
#include "MyPlayerController.h"

ADrone::ADrone()
	: Gravity(-980.0f),
	MaxFallSpeed(-300.0f),
	InterpSpeed(4.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	if (SphereComp)
	{
		SetRootComponent(SphereComp);
		SphereComp->SetSimulatePhysics(false);

		SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
		if (SkeletalMeshComp)
		{
			SkeletalMeshComp->SetupAttachment(SphereComp);
			SkeletalMeshComp->SetSimulatePhysics(false);

			bUseControllerRotationPitch = false;
			bUseControllerRotationRoll = false;
			bUseControllerRotationYaw = false;
		}

		SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
		if (SpringArmComp)
		{
			SpringArmComp->SetupAttachment(SphereComp);
			SpringArmComp->bUsePawnControlRotation = false;

			CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
			if (CameraComp)
			{
				CameraComp->SetupAttachment(SpringArmComp);
			}
		}
	}
}

void ADrone::BeginPlay()
{
	Super::BeginPlay();
}

void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// CurrentSpeed to TargetSpeed 보간
	CurrentVelocity.X = FMath::FInterpTo(CurrentVelocity.X,
		SpeedX * MoveSpeed * (bIsGround ? 1.f : 0.7f),
		GetWorld()->GetDeltaSeconds(),
		InterpSpeed);

	CurrentVelocity.Y = FMath::FInterpTo(CurrentVelocity.Y,
		SpeedY * MoveSpeed * (bIsGround ? 1.f : 0.7f),
		GetWorld()->GetDeltaSeconds(),
		InterpSpeed);

	// 중력
	CurrentVelocity.Z += Gravity * DeltaTime;
	CurrentVelocity.Z = FMath::Max(CurrentVelocity.Z, MaxFallSpeed);

	// LineTrace
	FHitResult Result;
	FVector Start = GetActorLocation();
	FVector End = GetActorLocation() + FVector(0.f, 0.f, -1.f) * 100.f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bIsGround = GetWorld()->LineTraceSingleByChannel(
		Result,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	// 땅에 닿으면 Z축 속도 0
	if (bIsGround && CurrentVelocity.Z <= 0.0f)
	{
		CurrentVelocity.Z = 0;
	}

	AddActorWorldOffset(FVector(0.f, 0.f, CurrentVelocity.Z) * DeltaTime, true);
	
	// LineTrace 출력
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.f);
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ADrone::Move);
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Completed, this, &ADrone::Move);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ADrone::Look);
			}
		}
	}
}

void ADrone::Move(const FInputActionValue& Value)
{
	FVector MoveInput = Value.Get<FVector>();

	// Target Speed
	SpeedX = MoveInput.X;
	SpeedY = MoveInput.Y;

	if (!MoveInput.IsNearlyZero())
	{
		FVector Offset(MoveInput * MoveSpeed * GetWorld()->GetDeltaSeconds() * (bIsGround ? 1.f : 0.7f));
		AddActorLocalOffset(Offset, true);
	}
}

void ADrone::Look(const FInputActionValue& Value)
{
	FVector LookInput = Value.Get<FVector>();

	if (!LookInput.IsNearlyZero())
	{
		FRotator ActorRotation(LookInput.Y * RotateSpeed * GetWorld()->GetDeltaSeconds(), LookInput.X * RotateSpeed * GetWorld()->GetDeltaSeconds(), LookInput.Z * RotateSpeed * GetWorld()->GetDeltaSeconds());
		AddActorLocalRotation(ActorRotation);
	}
}

FVector ADrone::GetVelocity() const
{
	return CurrentVelocity;
}

bool ADrone::GetIsGround() const
{
	return bIsGround;
}

