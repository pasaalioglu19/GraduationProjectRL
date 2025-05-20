// Ufuk Pasaalioglu All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/WarriorHeroCharacter.h"
#include "WarriorHeroAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATIONPROJECTT_API AWarriorHeroAICharacter : public AWarriorHeroCharacter
{
	GENERATED_BODY()
	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override {}
};
