// Ufuk Pasaalioglu All Rights Reserved


#include "DataAssets/StartUpData/DataAsset_HeroAIStartUpData.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"

void UDataAsset_HeroAIStartUpData::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	if (!HeroAICombatAbilities.IsEmpty()) {
		for (const TSubclassOf <UWarriorHeroGameplayAbility>& AbilityClass : HeroAICombatAbilities) {
			if (!AbilityClass) continue;

			FGameplayAbilitySpec AbilitySpec(AbilityClass);
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;
			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}
