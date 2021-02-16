#pragma once

#include <math.h>
#include "CoreMinimal.h"
#include "PlayerBasics.h"
#include "../CBMoves/Dodge/Dodge.h"
#include "../CBMoves/Throw/Throw.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Animation/BlendSpace1D.h"
#include "CB_PlayerCharacter.generated.h"

UCLASS()
class PROJECTCB_API ACB_PlayerCharacter : public ACharacter, public IGrabbable, public IGrabbableObject
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties

	PlayerBasics m_basics;

	ACB_PlayerCharacter();

private:

	// General
	short m_resetCollisionFrame;
	
	void playerUpdate(float deltaTime);

	void cameraUpdate();

	void adjustGravity(UCharacterMovementComponent* characterMovement);

	Dodge m_dodge = Dodge(this->m_basics);
	Throw m_throw = Throw(this->m_basics);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		USceneComponent* grabRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
		UBoxComponent* grabBox;

	UFUNCTION()
		void OnEnterGrabBox(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
			int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);

	UFUNCTION()
		void OnLeaveGrabBox(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent,
			int32 otherBodyIndex);

	//For prototyping, TODO add character model and start ik system
	UPROPERTY(EditAnywhere, Category = "Components")
		UStaticMeshComponent* staticMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
		USkeletalMeshComponent* skeletalMesh;

	UPROPERTY(EditAnywhere, Category = "Throwing")
		TSubclassOf<class ACB_DodgeballProjectile> DodgeballProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Components")
		class UCameraComponent* camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Components")
		class USpringArmComponent* cameraArm;

	//ANIM
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anims")
		UBlendSpace1D* blendspace;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveVertical(float amount);
	void MoveHorizontal(float amount);

	void RotateCamera(float amount);

	void JumpAction();
	void StopJumpAction();

	void ShootAction();
	void StopShootAction();

	void AliveAction();

	float getRadius() override;
	bool isGrabbable() override;
	void makeGrabbed() override;
	void launchRelease(FVector direction, FRotator rotation) override;
	void setGrabbed(FVector position, FRotator rotation) override;

	bool hasGrabbableObject() override;
	IGrabbableObject* getGrabbableObject() override;
	unsigned char getGrabPriority() override;

	//Animation Updating Functions
	bool onGround();
	bool onDuck();
	bool onThrowing();
	void resetThrowing();
	/// @brief Checks X and Y speed only, used for animations
	float groundMoveSpeed();
	/// @brief Check if the PlayerCharacter has a Grabbable Object, used for the catch animation
	bool onCatch();

};
