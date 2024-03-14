// Copyright AUGames
#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	//Primary Attributes
	GameplayTags.Attribute_Primary_Strength = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Primary.Strength"),"Increases physical damage");
	
	GameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"), FString("Increases magical damage"));
	
	GameplayTags.Attribute_Primary_Resilience = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Primary.Resilience"),"Increases Armor and Armor Penetration");
	
	GameplayTags.Attribute_Primary_Vigor = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Primary.Vigor"),"Increases Health");


	//Secondary Attributes
	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduce damage taken, improves Block Chanel"));
	
	GameplayTags.Attribute_Secondary_ArmorPenetration = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"),"Ignored Percentage of enemy Armor, Increase Critical Chance");
	
	GameplayTags.Attribute_Secondary_BlockChance = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"),"Chance to cut incoming damage in half");
	
	GameplayTags.Attribute_Secondary_CriticalHitChance = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"),"Chance to double damage plus critical hit bonus");
	
	GameplayTags.Attribute_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"),"Bonus damage added when a critical hit scored");
	
	GameplayTags.Attribute_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"),"Reduce Critical Hit Chance of attacking enemies");
	
	GameplayTags.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"),"Amount of Health regenerated every 1 second");
	
	GameplayTags.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"),"Amount of Mana regenerated every 1 second");
	
	GameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"),"Max amount of Health obtainable");
	
	GameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"),"Max amount of Mana obtainable");

	//Resistance Attributes
	GameplayTags.Attribute_Resistance_Arcane = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"),"Arcane damage resistance");

	GameplayTags.Attribute_Resistance_Fire = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Resistance.Fire"),"Fire damage resistance");

	GameplayTags.Attribute_Resistance_Lightning = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"),"Lightning damage resistance");

	GameplayTags.Attribute_Resistance_Physical = UGameplayTagsManager::Get().
	AddNativeGameplayTag(FName("Attributes.Resistance.Physical"),"Physical damage resistance");

	//Input Tags
	GameplayTags.InputTag_LBM = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LBM"), "Input Tag for Left Mouse Button");
	
	GameplayTags.InputTag_RBM = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RBM"), "Input Tag for Right Mouse Button");
	
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), "Input Tag for 1 key");
	
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), "Input Tag for 2 key");
	
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), "Input Tag for 3 key");
	
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), "Input Tag for 4 key");

	//Damage
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), "Damage");
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"), "Arcane Damage Type");
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"), "Fire Damage Type");
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lighting"), "Lighting Damage Type");
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), "Physical Damage Type");
	
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Arcane, GameplayTags.Attribute_Resistance_Arcane);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Fire, GameplayTags.Attribute_Resistance_Fire);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Lightning, GameplayTags.Attribute_Resistance_Lightning);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Physical, GameplayTags.Attribute_Resistance_Physical);

	
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), "Tag granted when Hit Reacting");
}
