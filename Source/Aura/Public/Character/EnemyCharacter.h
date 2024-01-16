// Copyright AUGames

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class AURA_API AEnemyCharacter : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
private:
	void ChangeCustomDepthState(bool bState, USkeletalMeshComponent* TargetMesh);
};
