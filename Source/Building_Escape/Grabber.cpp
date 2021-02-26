// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/DefaultPawn.h"

#define  OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty."));
	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
	// check for physicshandle component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle)
	{
		UE_LOG(LogTemp, Log, TEXT("PhysicsHandle component found"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle component not found in %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::GrabRelease);
	}
}


void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber pressed"));	
	// try and reach any actors with physics body collision channel set.
	GetFirstPhysicsBodyInReach();

	// if we hit something then attach physics handle
	// todo physics handle
}

void UGrabber::GrabRelease()
{
	// todo remove physics handle
	UE_LOG(LogTemp, Warning, TEXT("Grabber released"));
}



// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// if the physics handle is attached
		// move the object we are holding
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get the players viewpoint
	FVector PlayerViewPointLocation = GetWorld()->GetFirstPlayerController()->GetFocalLocation();
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );

	// logging out
	//UE_LOG(LogTemp, Warning, TEXT("Location : %s"), *PlayerViewPointLocation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Rotation : %s"), *PlayerViewPointRotation.ToString());

	// draw a line from player showing the reach.
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine(
        GetWorld(),
        PlayerViewPointLocation,
        LineTraceEnd,
        FColor(0 , 255, 0),
        false,
        -1,
        0,
        5);
	

	// Ray-cast out to a certain distance (Reach)
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	bool HitResult = GetWorld()->LineTraceSingleByObjectType(
        OUT Hit,
        PlayerViewPointLocation,
        LineTraceEnd,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParams
    );
	// See what it hits
	AActor* ActorHit = Hit.GetActor();
	if(HitResult)
	{
		UE_LOG(LogTemp, Warning, TEXT("got hit %s"), *ActorHit->GetName());
	}

	return Hit;

	
}


