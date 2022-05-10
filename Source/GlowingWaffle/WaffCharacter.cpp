// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "WaffInteractionComponent.h"
#include "WaffProjectile.h"
#include "WaffAttributeComponent.h"
#include "Components/CapsuleComponent.h"
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

	// Var
	HandSocketName = "Muzzle_01";
	TimeToHit = "TimeToHit";
}

// Post Initialize
void AWaffCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttriComp->OnHealthChanged.AddDynamic(this, &AWaffCharacter::OnHealthChanged);
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


void AWaffCharacter::PrimaryAttack()
{
	// Using animation montage
	PlayAnimMontage(AttackAnimMontage, 1.0f, NAME_None);

	// Bind the timer handler delegates with specific var.
	AttackTimerDelegate.BindUFunction(this, FName("Attack_TimeElapsed"), PrimaryProjectile);
	// Call the Bound function after 0.2f sec
	GetWorldTimerManager().SetTimer(AttackTimerHandler, AttackTimerDelegate, 0.2f, false);
}

void AWaffCharacter::SecondaryAttack()
{
	// Using animation montage
	PlayAnimMontage(AttackAnimMontage, 1.0f, NAME_None);

	// Bind the timer handler function with specific var.
	AttackTimerDelegate.BindUFunction(this, FName("Attack_TimeElapsed"), SecondaryProjectile);
	// Call the Bound function after 0.2f sec
	GetWorldTimerManager().SetTimer(AttackTimerHandler, AttackTimerDelegate, 0.2f, false);
}

void AWaffCharacter::DashCast()
{
	// Using animation montage
	PlayAnimMontage(AttackAnimMontage, 1.0f, NAME_None);

	// Bind the timer handler function with specific var.
	AttackTimerDelegate.BindUFunction(this, FName("Attack_TimeElapsed"), DashProjectile);
	// Call the Bound function after 0.2f sec
	GetWorldTimerManager().SetTimer(AttackTimerHandler, AttackTimerDelegate, 0.2f, false);
}

void AWaffCharacter::Attack_TimeElapsed(TSubclassOf<AWaffProjectile> AttackProjectile)
{
	// use the hand socket location as the spawn location
	FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);

	// Do line trace to find the camera rotation hit target
	FHitResult OutHit;
	FVector Start;
	FRotator PlayerView;
	GetController()->GetPlayerViewPoint(Start, PlayerView);

	FVector End = Start + CameraComp->GetComponentRotation().Vector() * 1000;
	FCollisionQueryParams QueryParam;
	QueryParam.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParam);
	// DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f, 0, 2.0f);

	// Decide the Final rotation for the projectile to hit target
	FRotator ProjectileSpawnRotation;
	if (OutHit.GetActor())
	{
		ProjectileSpawnRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, OutHit.ImpactPoint);
	}
	else
	{
		ProjectileSpawnRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, End);
	}

	// Combine with the ControlRotation, so he actor will spawn at the hand position and facing the camera direction.
	FTransform Spawn_TM = FTransform(ProjectileSpawnRotation, HandLocation);

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParam.Instigator = this;

	//Play Muzzle Flash
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GetMesh(), HandSocketName,FVector(ForceInit),
		FRotator::ZeroRotator, FVector(1),EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::None, true);
	// Spawn the projectile actor at the hand of the character
	AActor* Projectile = GetWorld()->SpawnActor<AActor>(AttackProjectile, Spawn_TM, SpawnParam);
	GetCapsuleComponent()->IgnoreActorWhenMoving(Projectile, true);
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
}


// Console Commands
void AWaffCharacter::HealSelf(float Amount /* = 100 */) const
{
	AttriComp->ApplyHealthChange(100);
}
