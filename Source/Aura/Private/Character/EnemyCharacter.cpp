// Copyright AUGames


#include "Character/EnemyCharacter.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AEnemyCharacter::HighlightActor()
{
	ChangeCustomDepthState(true, GetMesh());
	ChangeCustomDepthState(true, Weapon);
}

void AEnemyCharacter::UnHighlightActor()
{
	ChangeCustomDepthState(false, GetMesh());
	ChangeCustomDepthState(false, Weapon); 
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AEnemyCharacter::ChangeCustomDepthState(bool bState, USkeletalMeshComponent* TargetMesh)
{
		TargetMesh->SetRenderCustomDepth(bState);
		const int32 StencilValue = bState ? CUSTOM_DEPTH_RED : 0;
		TargetMesh->SetCustomDepthStencilValue(StencilValue);
}
