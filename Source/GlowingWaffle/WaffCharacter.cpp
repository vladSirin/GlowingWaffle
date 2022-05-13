// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "WaffActionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "WaffInteractionComponent.h"
#include "WaffProjectile.h"
#include "WaffAttributeComponent.h"
#include "Kismet/GameplayStatics.h"


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

	// Create interact comp
	InteractComp = CreateDefaultSubobject<UWaffInteractionComponent>(TEXT("InteractComp"));

	// Create Attribute Comp
	AttriComp = CreateDefaultSubobject<UWaffAttributeComponent>(TEXT("AttributeComp"));

	// Basic 3C settings
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Action Comp
	ActionComponent = CreateDefaultSubobject<UWaffActionComponent>(TEXT("ActionComp"));

	// Var
	TimeToHit = "TimeToHit";
}

// Post Initialize
void AWaffCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttriComp->OnHealthChanged.AddDynamic(this, &AWaffCharacter::OnHealthChanged);

	for(TSubclassOf<UWaffAction> ActionClass : ActionClassList)
	{
		ActionComponent->AddAction(ActionClass);
	}
}

FVector AWaffCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

// Called when the game starts or when spawned
void AWaffCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Attributes
// On health change ,check if player is dead, if so, disable input.
void AWaffCharacter::OnHealthChanged(AActor* ChangeInstigator, UWaffAttributeComponent* OwingComp, float NewHealth,
                                     float Delta)
{
	if (Delta < 0.0f)
	{
		Cast<UMeshComponent>(GetMesh())->SetScalarParameterValueOnMaterials(TimeToHit, GetWorld()->TimeSeconds);
		if (NewHealth <= 0.0f)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			DisableInput(PC);
		}
	}
}

// Movement controls
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

void AWaffCharacter::StartSprint()
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void AWaffCharacter::StopSprint()
{
	ActionComponent->StopActionByName(this, "Sprint");

}


void AWaffCharacter::PrimaryAttack()
{
	ActionComponent->StartActionByName(this, "PrimaryAttack");
}

void AWaffCharacter::SecondaryAttack()
{
	ActionComponent->StartActionByName(this, "SecondaryAttack");
}

void AWaffCharacter::DashCast()
{
	ActionComponent->StartActionByName(this, "DashCast");
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
	constexpr auto DrawScale = 100.0f;
	constexpr float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	const UE::Math::TVector<double> ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0,
	                          Thickness);

	const FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f,
	                          0, Thickness);
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
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &AWaffCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("DashCast", IE_Pressed, this, &AWaffCharacter::DashCast);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AWaffCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AWaffCharacter::StopSprint);
}


// Console Commands
void AWaffCharacter::HealSelf(float Amount /* = 100 */)
{
	AttriComp->ApplyHealthChange(100, Cast<AActor>(this));
}
