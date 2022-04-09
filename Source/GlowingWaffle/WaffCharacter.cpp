// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffCharacter.h"
#include "Camera\CameraComponent.h"
#include "GameFramework\SpringArmComponent.h"

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
}

// Called when the game starts or when spawned
void AWaffCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWaffCharacter::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * value);
}


// Called every frame
void AWaffCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWaffCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AWaffCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &AWaffCharacter::AddControllerYawInput);

}

