// Copyright AUGames

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	/*Tag to link this ability to activation by input only on game's start
	 *But if we want to change it example: LBM has fireball and we want that fire ball is moving to RBM. 
	 *We should to use "DynamicAbilityTags"
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;
};
