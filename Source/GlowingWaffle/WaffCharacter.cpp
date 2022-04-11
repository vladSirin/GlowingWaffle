// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffCharacter.h"
#include "Camera\CameraComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "WaffInteractionComponent.h"


// Sets default values
AWaffCharacter::AWaffCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Construct the Camera and spring arms, setting up attachment.
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractComp = CreateDefaultSubobject<UWaffInteractionComponent>(TEXT("InteractComp"));

	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AWaffCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWaffCharacter::MoveForward(float value)
{
	FRotator ControllerRotation = GetControlRotation();

	ControllerRotation.Pitch = 0;
	ControllerRotation.Roll = 0;

	AddMovementInput(ControllerRotation.Vector(), value);
}


void AWaffCharacter::MoveRight(float value)
{
	FRotator ControllerRotation = GetControlRotation();

	ControllerRotation.Pitch = 0;
	ControllerRotation.Roll = 0;

	AddMovementInput(FRotationMatrix(ControllerRotation).GetScaledAxis(EAxis::Y), value);
}


void AWaffCharacter::PrimaryAttack()
{
	// Using animation montage
	PlayAnimMontage(AttackAnimMontage, 1.0f, NAME_None);
	GetWorldTimerManager().SetTimer(AttackTimerHandler, this, &AWaffCharacter::PrimaryAttack_TimeElapsed, 0.2f, false);


}

void AWaffCharacter::PrimaryAttack_TimeElapsed()
{

	// use the hand socket location as the spawn location
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	// Combine with the ControlRotation, so he actor will spawn at the hand position and facing the camera direction.
	FTransform Spawn_TM = FTransform(GetControlRotation(), HandLocation);

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn the projectile actor at the hand of the character
	GetWorld()->SpawnActor<AActor>(MagicProjectile, Spawn_TM, SpawnParam);
}



void AWaffCharacter::PrimaryInteract()
{
	InteractComp->PrimaryInteract();
}

// This is entirely optional, it draws two arrows to visualize rotations of the player
void AWaffCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}


// Called to bind functionality to input
void AWaffCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AWaffCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWaffCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AWaffCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AWaffCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AWaffCharacter::PrimaryAttack);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AWaffCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AWaffCharacter::PrimaryInteract);

}

