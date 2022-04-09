// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffCharacter.h"
#include "Camera\CameraComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


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

}

