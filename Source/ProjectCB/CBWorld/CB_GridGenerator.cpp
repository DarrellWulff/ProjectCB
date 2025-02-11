#include "CB_GridGenerator.h"
#include "Net/UnrealNetwork.h"

const float ACB_GridGenerator::START_SECONDS = 1.0f;
const float ACB_GridGenerator::UPDATE_INTERVAL = 7.0f;
const float ACB_GridGenerator::FALL_TIME = 2.0f;

const int ACB_GridGenerator::STAGE_WIDTH = 4;
const int ACB_GridGenerator::STAGE_LENGTH = 6;

const float ACB_GridGenerator::BOX_SIZE = 250.0f;
const float ACB_GridGenerator::WIDTH_OFFSET = -ACB_GridGenerator::BOX_SIZE * (ACB_GridGenerator::STAGE_WIDTH - 1);
const float ACB_GridGenerator::LENGTH_OFFSET = -ACB_GridGenerator::BOX_SIZE * (ACB_GridGenerator::STAGE_LENGTH - 1);

const float ACB_GridGenerator::BALL_SPAWN_START_SECONDS = ACB_GridGenerator::START_SECONDS / 2.0f;
const float ACB_GridGenerator::BALL_SPAWN_UPDATE_INTERVAL = ACB_GridGenerator::UPDATE_INTERVAL / 2.0f;

void ACB_GridGenerator::deleteBoxes_Implementation()
{
    for (int index = 0; index < this->m_numOfFallingBoxes; index++) 
    {
        if (this->m_fallingBoxes.IsValidIndex(index) && (m_fallingBoxes[index]))
        {
            this->m_fallingBoxes[index]->Destroy();
            //this->m_fallingBoxes[index]->SetActorEnableCollision(false);
            //this->m_fallingBoxes[index]->BoxMesh->SetVisibility(false);
        }
    }
        

    this->m_fallingBoxes.Empty();
    this->m_numOfFallingBoxes = 0;
}

ACB_GridBox* ACB_GridGenerator::spawnBox(int lengthPos, int widthPos)
{
    FActorSpawnParameters spawnParams;

    FVector location = FVector(ACB_GridGenerator::LENGTH_OFFSET + (2 * lengthPos * ACB_GridGenerator::BOX_SIZE),
        ACB_GridGenerator::WIDTH_OFFSET + (2 * widthPos * ACB_GridGenerator::BOX_SIZE), 0.0f);
    FRotator rotation = FRotator(0.0f, 0.0f, 0.0f);

    return GetWorld()->SpawnActor<ACB_GridBox>(this->BoxClass, location, rotation, spawnParams);
}

void ACB_GridGenerator::spawnBall_Implementation(int lengthPos, int widthPos)
{
    FActorSpawnParameters spawnParams;

    FVector location = FVector(ACB_GridGenerator::LENGTH_OFFSET + (2 * lengthPos * ACB_GridGenerator::BOX_SIZE),
        ACB_GridGenerator::WIDTH_OFFSET + (2 * widthPos * ACB_GridGenerator::BOX_SIZE), 500.0f);
    FRotator rotation = FRotator(0.0f, 0.0f, 0.0f);

    ACB_DodgeballProjectile* ball = GetWorld()->SpawnActor<ACB_DodgeballProjectile>(this->DodgeballClass,
        location, rotation, spawnParams);

    ball->SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
}

void ACB_GridGenerator::spawnBalls_Implementation()
{
    if (this->m_numOfBoxes > 0)
    {
        int boxPos = FMath::RandRange(0, this->m_numOfBoxes - 1);

        int boxID = this->m_deletableBoxes[boxPos];

        int length1 = boxID / ACB_GridGenerator::STAGE_WIDTH;
        int width1 = boxID - (length1 * ACB_GridGenerator::STAGE_WIDTH);

        spawnBall(length1, width1);

        if (length1 != (ACB_GridGenerator::STAGE_LENGTH / 2))
        {
            int length2 = ACB_GridGenerator::STAGE_LENGTH - length1 - 1;
            int width2 = ACB_GridGenerator::STAGE_WIDTH - width1 - 1;

            spawnBall(length2, width2);
        }
    }
    else
    {
        GetWorldTimerManager().ClearTimer(this->m_ballHandle);
    }
}

void ACB_GridGenerator::generateGrid_Implementation()
{
    for (int length = 0; length < ACB_GridGenerator::STAGE_LENGTH; length++)
    {
        TArray<ACB_GridBox*> boxArray;

        for (int width = 0; width < ACB_GridGenerator::STAGE_WIDTH; width++)
        {
            if (length < ((ACB_GridGenerator::STAGE_LENGTH + 1) / 2))
            {
                this->m_deletableBoxes.Add((length * ACB_GridGenerator::STAGE_WIDTH) + width);
                this->m_numOfBoxes++;
            }
            
            boxArray.Add(spawnBox(length, width));
        }

        this->m_grid.Add(boxArray);
    }
}

void ACB_GridGenerator::updateGrid_Implementation()
{
    if (this->m_numOfBoxes > 0)
    {
        this->m_numOfBoxes--;

        int boxPos = FMath::RandRange(0, this->m_numOfBoxes);

        int boxID = this->m_deletableBoxes[boxPos];

        int length = boxID / ACB_GridGenerator::STAGE_WIDTH;
        int width = boxID - (length * ACB_GridGenerator::STAGE_WIDTH);

        ACB_GridBox* box1 = this->m_grid[length][width];
        box1->startFall();
        this->m_fallingBoxes.Add(box1);

        if (length != (ACB_GridGenerator::STAGE_LENGTH / 2))
        {
            ACB_GridBox* box2 = this->m_grid[ACB_GridGenerator::STAGE_LENGTH - length - 1][ACB_GridGenerator::STAGE_WIDTH - width - 1];
            box2->startFall();
            this->m_fallingBoxes.Add(box2);

            this->m_numOfFallingBoxes = 2;
        }
        else
            this->m_numOfFallingBoxes = 1;
        
        this->m_deletableBoxes.RemoveAt(boxPos);
        
    }
    else
    {
        GetWorldTimerManager().ClearTimer(this->m_timerHandle);
        GetWorldTimerManager().ClearTimer(this->m_fallHandle);
    }
}

ACB_GridGenerator::ACB_GridGenerator()
{
 	PrimaryActorTick.bCanEverTick = false;
    this->SetReplicates(true);

    this->m_numOfBoxes = 0;
    this->m_numOfFallingBoxes = 0;
}

void ACB_GridGenerator::BeginPlay()
{
    Super::BeginPlay();

    //Had to temporarily disable this since it seemed like it was causing some issues

    generateGrid();

    GetWorldTimerManager().SetTimer(this->m_ballHandle, this, &ACB_GridGenerator::spawnBalls,
        ACB_GridGenerator::BALL_SPAWN_UPDATE_INTERVAL, true, ACB_GridGenerator::BALL_SPAWN_START_SECONDS);
    
    size_t centerLength = ACB_GridGenerator::STAGE_LENGTH / 2.0f;
    size_t centerWidth = ACB_GridGenerator::STAGE_WIDTH / 2.0f;

    //spawnBall(centerLength, centerWidth);
    //spawnBall(centerLength - 1, centerWidth);
    //spawnBall(centerLength, centerWidth - 1);
    //spawnBall(centerLength - 1, centerWidth - 1);
}

void ACB_GridGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACB_GridGenerator::EnableStageRemoval_Implementation()
{
	GetWorldTimerManager().SetTimer(this->m_timerHandle, this, &ACB_GridGenerator::updateGrid,
		ACB_GridGenerator::UPDATE_INTERVAL, true, ACB_GridGenerator::START_SECONDS);

	GetWorldTimerManager().SetTimer(this->m_fallHandle, this, &ACB_GridGenerator::deleteBoxes,
		ACB_GridGenerator::FALL_TIME, true, ACB_GridGenerator::START_SECONDS);
}

void ACB_GridGenerator::DisableStageRemoval_Implementation()
{
    GetWorldTimerManager().ClearTimer(this->m_timerHandle);
    GetWorldTimerManager().ClearTimer(this->m_fallHandle);
}

void ACB_GridGenerator::DisableBallGeneration_Implementation()
{
	GetWorldTimerManager().ClearTimer(this->m_ballHandle);
}

void ACB_GridGenerator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACB_GridGenerator, m_fallingBoxes);

    DOREPLIFETIME(ACB_GridGenerator, m_timerHandle);

    DOREPLIFETIME(ACB_GridGenerator, m_fallHandle);

    DOREPLIFETIME(ACB_GridGenerator, m_ballHandle);
}



