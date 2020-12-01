// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootGameCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "MyPlayerState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h" 
#include "Pickups/Pickup_Gun.h"

//////////////////////////////////////////////////////////////////////////
// AShootGameCharacter

AShootGameCharacter::AShootGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	PlayerScore = 0.0f;
	xcount = 0;
	bIsAI = false;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AShootGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AShootGameCharacter::HandleFire);
	PlayerInputComponent->BindAction<FSetIsFreeViewDelegate>("FreeView", IE_Pressed, this, &AShootGameCharacter::SetIsFreeView, true);
	PlayerInputComponent->BindAction<FSetIsFreeViewDelegate>("FreeView", IE_Released, this, &AShootGameCharacter::SetIsFreeView, false);

	PlayerInputComponent->BindAction("Pick", IE_Pressed, this, &AShootGameCharacter::PickupThing); 
	PlayerInputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &AShootGameCharacter::ChangeWeapon);
	PlayerInputComponent->BindAction("Relife", IE_Pressed, this, &AShootGameCharacter::Relife);


	//PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &AShootGameCharacter::HandleMeleeDown);
	//PlayerInputComponent->BindAction("Melee", IE_Released, this, &AShootGameCharacter::HandleMeleeUp);

	FString tmpx = "Melee";
	tmpx = "Skill0"; PlayerInputComponent->BindAction<FStringDelegate>("Melee", IE_Pressed, this, &AShootGameCharacter::SetSkillNameWrapper, tmpx);
	tmpx = "Skill1"; PlayerInputComponent->BindAction<FStringDelegate>("Skill1", IE_Pressed, this, &AShootGameCharacter::SetSkillNameWrapper, tmpx);
	tmpx = "Skill2"; PlayerInputComponent->BindAction<FStringDelegate>("Skill2", IE_Pressed, this, &AShootGameCharacter::SetSkillNameWrapper, tmpx);
	tmpx = "Skill3"; PlayerInputComponent->BindAction<FStringDelegate>("Skill3", IE_Pressed, this, &AShootGameCharacter::SetSkillNameWrapper, tmpx);
	tmpx = "None"; PlayerInputComponent->BindAction<FStringDelegate>("Melee", IE_Released, this, &AShootGameCharacter::SetSkillNameWrapper, tmpx);
	tmpx = "None"; PlayerInputComponent->BindAction<FStringDelegate>("Skill1", IE_Released, this, &AShootGameCharacter::SetSkillNameWrapper, tmpx);
	tmpx = "None"; PlayerInputComponent->BindAction<FStringDelegate>("Skill2", IE_Released, this, &AShootGameCharacter::SetSkillNameWrapper, tmpx);
	tmpx = "None"; PlayerInputComponent->BindAction<FStringDelegate>("Skill3", IE_Released, this, &AShootGameCharacter::SetSkillNameWrapper, tmpx);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShootGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShootGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShootGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShootGameCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AShootGameCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AShootGameCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AShootGameCharacter::OnResetVR);


	//UE_LOG(LogClass, Log, TEXT("mjhai i'am %s, is locally controlled?%s"), *GetName(), (IsAI ? "yes" : "no"));
}


FVector AShootGameCharacter::GetFaceDirection()
{
	FVector ans = FollowCamera->GetComponentLocation() - RootComponent->GetComponentLocation();
	return ans;
}

void AShootGameCharacter::OnResetVR()
{
UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AShootGameCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AShootGameCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}


void AShootGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShootGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AShootGameCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AShootGameCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


AMyPlayerState* AShootGameCharacter::GetMyPlayerState() {
	if (MyPlayerState != nullptr) {
		return MyPlayerState;
	}
	else { 
		APlayerState* x = GetPlayerState();
		if (x != nullptr) { 
			MyPlayerState = Cast<AMyPlayerState>(x);
			if (MyPlayerState == nullptr) { 
				UE_LOG(LogClass, Log, TEXT("mphy myplayerstate is nullptr"));
			} 
		}
		else {
			UE_LOG(LogClass, Log, TEXT("mphy getplayerstate return nullptr"));
		}
		return MyPlayerState;
	}
}
void AShootGameCharacter::BeginPlay() {
	Super::BeginPlay();


	//MyPlayerState = Cast<AMyPlayerState>(GetPlayerState());

	//HoldWeapon();



	//GetMesh()->OnComponentHit.AddDynamic(this, &AShootGameCharacter::OnMyHit);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AShootGameCharacter::DefaultTimer, GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation(), true);

}

void AShootGameCharacter::ShootTarget()
{
	if (MyWeaponClass != nullptr) {
		MyWeaponClass->WeaponFire();
	}
}


void AShootGameCharacter::HandleFire()
{

	if (GetLocalRole() == ROLE_Authority)
	{
		ClientHandleFire();
	}
	else
	{
		ServerHandleFire();
	} 
}

void  AShootGameCharacter::ServerHandleFire_Implementation() {
	HandleFire();
}
void  AShootGameCharacter::ClientHandleFire_Implementation() {
	ShootTarget();
}
void AShootGameCharacter::HandleMeleeDown()
{
	bMyMeleeDown = true;
}
void AShootGameCharacter::HandleMeleeUp()
{
	bMyMeleeDown = false;
}

bool  AShootGameCharacter::IsMelee() {
	return bMyMeleeDown;
}

void AShootGameCharacter::ServerChangeWeapon_Implementation() {
	ChangeWeapon();
}
void AShootGameCharacter::ClientChangeWeapon_Implementation() {
	if (MyWeaponClass != nullptr && MyWeaponClass->IsActorInitialized()) {
		MyWeaponClass->Destroy(); 
		MyWeaponClass = nullptr;
	}
	if (MyPlayerState != nullptr) {

		auto wds = MyPlayerState->GetWeaponData();
		int len = wds.Num();
		if (len > 0) {
			int32 i = MyPlayerState->GetNowWeaponIndex();
			i = (i + 1) % len; 
			if (i >= 0 && i < wds.Num()) {
				FWeaponData wd = wds[i];

				UWorld* const World = GetWorld();
				if (World != nullptr) {
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					SpawnParams.Instigator = GetInstigator();
					FVector SpawnLoc = GetMesh()->GetSocketLocation("ring_01_r");
					FRotator SpawnRot = UKismetMathLibrary::MakeRotator(0, 0, 0);
					UClass* clas = wd.WeaponStaticClass;
					if (clas == nullptr) {
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "clas - false");
					}
					else {
						MyWeaponClass = World->SpawnActor<AWeapon>(clas, SpawnLoc, SpawnRot, SpawnParams);
						if (MyWeaponClass) {
							MyWeaponClass->AttachToComponent(this->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), "ring_02_r_socket");
							MyWeaponClass->SetWeaponPlayer(this);
						}
					}
				}

				MyPlayerState->SetNowWeaponIndex(i);
			}
			else {
				FString msg = FString::Printf(TEXT("i=%d,len=%d "), i, wds.Num());
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, msg);

			}
			
		}
	}
}
void AShootGameCharacter::UpdateKillCount(int x)
{
	if (GetMyPlayerState() != nullptr) {
		GetMyPlayerState()->UpdateKillCount(x);
	}
}
void AShootGameCharacter::UpdateBlood(float x)
{ 
	UE_LOG(LogClass, Log, TEXT("mmul update blood called")); 
	if (GetMyPlayerState() != nullptr) {
		UE_LOG(LogClass, Log, TEXT("mmul update blood called 2"));
		ECharLifeType pres = GetMyPlayerState()->GetLifeState();
		GetMyPlayerState()->SetBlood(GetMyPlayerState()->GetBlood() + x);
		auto afters = GetMyPlayerState()->GetLifeState();
		if (pres == ECharLifeType::CLT_ALIVE && afters == ECharLifeType::CLT_DEAD) {
			// dead
			if (GetLocalRole() == ROLE_Authority) {
				ClientBeDead();
			}
		}
	} 
}
void AShootGameCharacter::ChangeWeapon()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ClientChangeWeapon();
	}
	else
	{
		ServerChangeWeapon();
	}
}
void AShootGameCharacter::ClientBeDead_Implementation()
{
	SetSkillName("Skill3");
	//this->GetMesh()->SetAllBodiesSimulatePhysics(true);
	this->GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("pelvis"), true);
	//this->GetMesh()->SetAllBodiesPhysicsBlendWeight(1.0);
	this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
}
void AShootGameCharacter::HoldWeapon() {
	if (MyWeapon != NULL) {
		UWorld* const World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			FVector SpawnLoc = GetMesh()->GetSocketLocation("ring_01_r");
			FRotator SpawnRot;
			SpawnRot.Yaw = rz;
			SpawnRot.Pitch = ry;
			SpawnRot.Roll = rx;

			SpawnRot = UKismetMathLibrary::MakeRotator(0, 0, 0);

			MyWeaponClass = World->SpawnActor<AWeapon>(MyWeapon, SpawnLoc, SpawnRot, SpawnParams);
			 
			MyWeaponClass->AttachToComponent(this->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), "ring_02_r_socket");
			MyWeaponClass->SetWeaponPlayer(this);
		}
	}
}


float AShootGameCharacter::GetPlayerScore()
{
	return PlayerScore; 
}

void AShootGameCharacter::UpdateScore(float x) {
	PlayerScore += x; 
	UE_LOG(LogClass, Log, TEXT("Now player score:%f"), PlayerScore);
	
	
	AMyPlayerState* mps = Cast<AMyPlayerState>(this->GetPlayerState());
	if (mps != nullptr) {
		mps->UpdateScore(x);
	}
}


void AShootGameCharacter::SetMyAnimYaw(float x) {
	MyAnimYaw = x;
}
FRotator AShootGameCharacter::GetMyAnimRotator() {
	FRotator ans;

	// 是服务器 或 是本地控制
	if (bIsAI) {
		ans = UKismetMathLibrary::MakeRotator(0, -21, 0);
	} else if (GetLocalRole() == ROLE_Authority || IsLocallyControlled()) {
		ans = MyAnimRotator;
	}
	else {
		ans = FakeAnimRotator;
	}
	return ans;
}

void AShootGameCharacter::SetIsFreeView(bool x)
{
	bIsFreeView = x;
}

void AShootGameCharacter::SetSkillName_Implementation(const FString& x)
{
	MySkillName = x;
}

void AShootGameCharacter::SetAnimRotator(FRotator x)
{ 
	MyAnimRotator = x; 


	if (GetLocalRole() == ROLE_Authority && FakeAnimRotator != MyAnimRotator) {
		FakeAnimRotator = UKismetMathLibrary::MakeRotator(MyAnimRotator.Roll, MyAnimRotator.Pitch + 90, MyAnimRotator.Yaw + 90);
		//UE_LOG(LogClass, Log, TEXT("mtest i'am server, i'am set fakeanimrotator to animrotator, animrotator(pitch,yaw,roll)=(%f,%f,%f),fakeanimrotator(pitch,yaw,roll)=(%f,%f,%f)"), MyAnimRotator.Pitch, MyAnimRotator.Yaw, MyAnimRotator.Roll, FakeAnimRotator.Pitch, FakeAnimRotator.Yaw, FakeAnimRotator.Roll);

	}
	/* 
	xcount++;
	if (xcount == 30) {
		xcount = 0;
		if (GetWorld() && GetWorld()->IsServer() && FakeAnimRotator != MyAnimRotator) { 
			FakeAnimRotator = MyAnimRotator;
			UE_LOG(LogClass, Log, TEXT("mtest i'am server, i'am set fakeanimrotator to animrotator, animrotator(pitch,yaw,roll)=(%f,%f,%f),fakeanimrotator(pitch,yaw,roll)=(%f,%f,%f)"), MyAnimRotator.Pitch, MyAnimRotator.Yaw, MyAnimRotator.Roll, FakeAnimRotator.Pitch, FakeAnimRotator.Yaw, FakeAnimRotator.Roll);

		}
	}
	*/
} 

void AShootGameCharacter::SetSkillNameWrapper(FString x)
{
	SetSkillName(x);
}

FString AShootGameCharacter::GetSkillName()
{
	return MySkillName;
}

void AShootGameCharacter::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	   
	if (GetLocalRole() == ROLE_Authority || IsLocallyControlled()) { 
		FRotator r1 = this->GetControlRotation() - GetActorRotation();
		FRotator r2 = UKismetMathLibrary::RInterpTo(MyAnimRotator, r1, DeltaTime, 15.0);
		MyAnimPitch = UKismetMathLibrary::ClampAngle(r2.Pitch, -90, 90);
		MyAnimYaw = UKismetMathLibrary::ClampAngle(r2.Yaw, -90, 90);
		FRotator x = UKismetMathLibrary::MakeRotator(0, MyAnimPitch, MyAnimYaw);

		SetAnimRotator(x);
	}  
	 
}

bool AShootGameCharacter::IsFreeView() {
	return bIsFreeView;
}

void AShootGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShootGameCharacter, MySkillName);
	DOREPLIFETIME(AShootGameCharacter, FakeAnimRotator);
}

void AShootGameCharacter::OnRep_FakeAnimRotator()
{
	FakeAnimRotator = UKismetMathLibrary::MakeRotator(FakeAnimRotator.Roll, FakeAnimRotator.Pitch - 90, FakeAnimRotator.Yaw - 90);;
	//UE_LOG(LogClass, Log, TEXT("mtest i'am clientr, i'am got fakeanimrotator(pitch,yaw,roll)=(%f,%f,%f)"), FakeAnimRotator.Pitch, FakeAnimRotator.Yaw, FakeAnimRotator.Roll);
	/* 
	if (GetWorld()) {
		if (GetWorld()->IsServer()) {
			UE_LOG(LogClass, Log, TEXT("mtest i'am server,i am onrep_fakeanimrotator"));
		}
		else {  
			UE_LOG(LogClass, Log, TEXT("mtest i'am client,i am onrep_fakeanimrotator"));
		}
	}
	if (!IsLocallyControlled()) {
		UE_LOG(LogClass, Log, TEXT("mtest i'am client, i'am set animrotator to fakeanimrotator, fakeanimrotator(pitch,yaw,roll)=(%f,%f,%f)"), FakeAnimRotator.Pitch, FakeAnimRotator.Yaw, FakeAnimRotator.Roll);
		MyAnimRotator = FakeAnimRotator;
	}
	*/
}

bool AShootGameCharacter::IsAI()
{
	return bIsAI;
}
/*
void AShootGameCharacter::OnMyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	AShootGameCharacter* otherCharacter = Cast<AShootGameCharacter>(OtherActor);
	if (otherCharacter != nullptr) {
		if (IsAI()) {
			int32 maxski = 3;
			static int32 lastSkill = 0;
			// [0,maxski)
			lastSkill = (lastSkill + 1) % maxski;
			switch (lastSkill) {
			case 0:
				SetSkillNameWrapper("Skill0");
				break;
			case 1:
				SetSkillNameWrapper("Skill1");
				break;
			case 2:
				SetSkillNameWrapper("Skill2");
				break;
			}
			GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AShootGameCharacter::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), false);

		}
	}
}
*/
void  AShootGameCharacter::DefaultTimer() {
	SetSkillNameWrapper("None");

	if (MyPlayerState == nullptr) {
		GetMyPlayerState();
	}
	else {
		if (MyPlayerState->GetMyChar() == nullptr) {
			MyPlayerState->SetMyChar(this);
		}
	}
}

void AShootGameCharacter::PickupThing() { 
	if (GetLocalRole() == ROLE_Authority)
	{
		ClientPickupThing();
	}
	else
	{
		ServerPickupThing();
	}

}


void AShootGameCharacter::ServerPickupThing_Implementation() {
	PickupThing();
}

void AShootGameCharacter::ClientPickupThing_Implementation() {
	UE_LOG(LogClass, Log, TEXT("mphy pick up start"));

	auto MyWorld = GetWorld();
	if (MyWorld != nullptr) {
		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
		RV_TraceParams.bTraceComplex = false;
		RV_TraceParams.bReturnPhysicalMaterial = false;

		FCollisionObjectQueryParams coqp;
		coqp.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		coqp.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		coqp.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

		FCollisionShape cs = FCollisionShape::MakeSphere(PICK_DISTANCE);

		TArray<FOverlapResult> Overlaps;
		bool everOverlaped = MyWorld->OverlapMultiByObjectType(Overlaps, GetActorLocation(), FQuat(), coqp, cs, RV_TraceParams);

		UE_LOG(LogClass, Log, TEXT("mphy pick ever overlaped:%d"), everOverlaped ? 1 : 0);
		if (everOverlaped) {

			UE_LOG(LogClass, Log, TEXT("mphy pick overlaped count"), Overlaps.Num());
			for (int i = 0; i < Overlaps.Num(); i++) {
				FOverlapResult& or = Overlaps[i];
				AActor* ac = or .GetActor();
				if (ac != nullptr) {
					APickup_Gun* pg = Cast<APickup_Gun>(ac);
					if (pg != nullptr) {
						FWeaponData wd = pg->GetWeaponData();
						UE_LOG(LogClass, Log, TEXT("mphy pick overlaped gun,info:%s"), *wd.ToString());
						pg->BePicked();
						UE_LOG(LogClass, Log, TEXT("mphy is server?%d,player state is null?%d"), GetLocalRole() == ROLE_Authority ? 1 : 0, GetMyPlayerState() == nullptr ? 1 : 0);

						if (GetMyPlayerState() != nullptr) {
							GetMyPlayerState()->GetWeaponData().AddUnique(pg->GetWeaponData());
						}
					}
				}
			}
		}
	}
}

void AShootGameCharacter::ServerBeRelife_Implementation()
{
	Relife();
}

void AShootGameCharacter::ClientBeRelife_Implementation()
{
	/* 
	if (GetMyPlayerState() != nullptr) {
		GetMyPlayerState()->SetLifeState(ECharLifeType::CLT_ALIVE);
		//this->GetMesh()->SetAllBodiesSimulatePhysics(false);
		this->GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("pelvis"), false);

		this->GetMesh()->SetAllBodiesPhysicsBlendWeight(0);
		//this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	*/
	if (GetMyPlayerState() != nullptr) {
		GetMyPlayerState()->SetLifeState(ECharLifeType::CLT_ALIVE);
	}
	this->DetachFromControllerPendingDestroy();
	this->Destroy();
	/*
	if (GetLocalRole() == ROLE_Authority) {

	}
	else {

		GetWorld()->GetFirstPlayerController()->ServerRestartPlayer();
	}
	*/
	/*
	UE_LOG(LogClass, Log, TEXT("mmul pose 1"));
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{  
		if ((*Iterator)->IsLocalPlayerController()) {
			UE_LOG(LogClass, Log, TEXT("mmul pose 0"));
			UClass* clas = DefaultCharacterClass;
			if (clas == nullptr) {
				UE_LOG(LogClass, Log, TEXT("mmul pose 4"));
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "clas - false");
			}
			else {
				//(*Iterator)->ServerRestartPlayer();
				
				UE_LOG(LogClass, Log, TEXT("mmul pose 2"));
				auto y = GetActorTransform();
				auto x = GetWorld()->SpawnActor<AShootGameCharacter>(clas, y);
				if (x) {

					(*Iterator)->Possess(x);
					//this->Destroy();
					UE_LOG(LogClass, Log, TEXT("mmul pose"));

				}
				else {
					UE_LOG(LogClass, Log, TEXT("mmul pose 3"));
				} 
			}
			break;
		}
	} 
	*/
}

void AShootGameCharacter::Relife() {
	if (GetLocalRole() == ROLE_Authority)
	{
		ClientBeRelife();
	}
	else
	{
		ServerBeRelife();
	}
}