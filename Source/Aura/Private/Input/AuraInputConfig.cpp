// Copyright AUGames

#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for(const FAuraInputActions& Pair : AbilityInputActions)
	{
		if (Pair.InputAction && Pair.InputTag.MatchesTagExact(InputTag))
		{
			return Pair.InputAction;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Not founded input action for [%s]"), *InputTag.GetTagName().ToString());
	return nullptr;
}
