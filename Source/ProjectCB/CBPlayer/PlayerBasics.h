#pragma once

#include "CoreMinimal.h"
#include <math.h>
#include "../CBMoves/Movement/Movement.h"
#include "../CBMoves/Movement/CameraMovement.h"
#include "PlayerBasics.generated.h"

USTRUCT()
struct PROJECTCB_API FPlayerBasics
{
	GENERATED_BODY()

public:

	static const FVector2D MAX_MAP_POSITION;
	static const float MAP_RESPAWN_POSITION_OFFSET;
	static const float MIN_MAP_POSITION_Z;

	static const float PLAYER_RADIUS;
	static const float PLAYER_HEIGHT;

	static const float PLAYER_SPAWN_POSITION_Z;

	static const float PLAYER_BASE_GRAVITY;
	static const float PLAYER_FAST_GRAVITY;

	static const float PLAYER_START_WORLD_LOCATION_Z;
	static const float WORLD_LOCATION_PROPORTION_Z; // TODO add to camera

	static const float LAUNCH_SPEED;
	static const float LAUNCH_MOBILITY;
	static const float LAUNCH_HEIGHT;

	enum PlayerState { PLAYER_ALIVE = 0, PLAYER_GRABBED, PLAYER_GHOST };
	enum DodgeState { DODGE_OFF = 0, DODGE_STARTUP, DODGE_DUCK, DODGE_IDLE, DODGE_JUMP, DODGE_COOLDOWN };
	enum ThrowState { THROW_OFF = 0, CATCH_STARTUP, THROW_STARTUP, CATCH_AIM, THROW_AIM, CATCH_COOLDOWN, THROW_COOLDOWN };

private:

	bool m_grounded;
	PlayerState m_playerState;

	FVector checkAliveBounds(FVector playerPosition);
	FVector checkGhostBounds(FVector playerPosition);

	void makeGhost();
	
public:

	DodgeState m_dodgeState; // TODO make private
	ThrowState m_throwState; // TODO make private

	UPROPERTY()
	float m_currentWorldLocationZ;
	
	UPROPERTY()
	float m_currentMobility;
	UPROPERTY()
	float m_previousMobility;

	float m_currentRadius;

	float m_currentHeight;
	float m_previousHeight;

	static const short RESET_COLLISION_FRAMES;

	float m_currentScaleAmountX = 0.0;
	float m_currentScaleAmountY = 0.0;

//////

	
	Movement m_movement;
	
	CameraMovement m_cameraMovement;
	
	//a reference to the current level so we can spawn objects from code
	UWorld* m_gameWorldRef;

	AActor* m_playerRef;

	UPROPERTY()
	TSubclassOf<class ACB_DodgeballProjectile> dodgeballClassRef;

	USkeletalMeshComponent* m_playerSkeletalMeshComponent;
	USkeletalMesh* m_playerModel;
	USkeletalMesh* m_ghostModel;

	//ANIM Helpers
	bool m_throwing = false;
	bool m_ducking = false;

	FVector m_rootPosition; //TODO remove temporary fix!

/////
	
	FRotator m_inputRotation;
	
	FVector m_velocity; // TODO remove?

	float m_jumpZVelocity;

	bool m_shouldJump;
	bool m_fellOff;

	FPlayerBasics();

	float getJumpVelocity(float height);

	float getAnimationPoint(float x);

	void updateAttributes();

	void updateGroundState(bool grounded);

	bool isGrounded();

	PlayerState getPlayerState();

	FVector checkPlayerBounds(FVector playerPosition);

	FVector getInputDirection();

	void makeAlive();

	void makeGrabbed();

	void launchPlayer(FVector direction, FRotator rotation);
};
