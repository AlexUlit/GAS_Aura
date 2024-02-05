// Copyright AUGames


#include "AbilitySystem/AuraAbilitySystemComponent.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer EffectTags;
	
	// put all effects tags to "EffectTags"
	EffectSpec.GetAllAssetTags(EffectTags);
	
	//Send these tags for subscribers(OverlayWidgetController)
	EffectAssetTags.Broadcast(EffectTags);
}
