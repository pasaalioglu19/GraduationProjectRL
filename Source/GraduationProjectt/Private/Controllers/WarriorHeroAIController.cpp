// Ufuk Pasaalioglu All Rights Reserved


#include "Controllers/WarriorHeroAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "WarriorDebugHelper.h"

AWarriorHeroAIController::AWarriorHeroAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{

	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("HeroAISenseConfig_Sight");
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	AISenseConfig_Sight->SightRadius = 5000.f;
	AISenseConfig_Sight->LoseSightRadius = 0.f;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f;

	HeroAIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("HeroAIPerceptionComponent");
	HeroAIPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	HeroAIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	HeroAIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnHeroAIPerceptionUpdated);

	SetGenericTeamId(FGenericTeamId(0));
}

ETeamAttitude::Type AWarriorHeroAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);

	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());

	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() > GetGenericTeamId()) {
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;

}

void AWarriorHeroAIController::OnHeroAIPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	//Debug::Print(FString::Printf(TEXT("Seen actor is -> %s"), *Actor->GetActorNameOrLabel()), FColor::Purple);

	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent()) {
		if (!BlackboardComponent->GetValueAsObject(FName("TargetActor"))) {
			if (Stimulus.WasSuccessfullySensed() && Actor) {
				BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
			}
		}
	}
}