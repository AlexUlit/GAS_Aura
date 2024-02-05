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
}
