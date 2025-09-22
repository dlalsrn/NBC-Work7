#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Drone.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class NBC_WORK7_API ADrone : public APawn
{
	GENERATED_BODY()

public:
	ADrone();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	FVector GetVelocity() const;
	bool GetIsGround() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone")
	TObjectPtr<USphereComponent> SphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone")
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	float RotateSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	float Gravity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	float MaxFallSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	FVector CurrentVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	bool bIsGround;

private:
	float SpeedX;
	float SpeedY;
	float InterpSpeed;
};
