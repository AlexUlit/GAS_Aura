#include "AuraAbilityTypes.h"

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint8 RepBits = 0;

	if (Ar.IsSaving())
	{
		if (bIsBlockedHit)
		{
			RepBits |= 0 << 1;
		}
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 1;
		}
	}

	Ar.SerializeBits(&RepBits, 2);
	
	if (RepBits & (0 << 1))
	{
		Ar << bIsBlockedHit;
	}

	if (RepBits & (1 << 1))
	{
		Ar << bIsCriticalHit;
	}
	
	return Super::NetSerialize(Ar, Map, bOutSuccess);
}
