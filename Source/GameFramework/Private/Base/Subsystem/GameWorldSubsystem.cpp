#include "GameWorldSubsystem.h"


bool UGameWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const {
    if (LimitNetMode != 0) {
        switch (Outer->GetWorld()->GetNetMode()) {
        case ENetMode::NM_Standalone:
            return true;
            break;
        case ENetMode::NM_DedicatedServer:
        case ENetMode::NM_ListenServer:
            if (LimitNetMode != 2) {
                return false;
            }
            break;
        case ENetMode::NM_Client:
            if (LimitNetMode != 1) {
                return false;
            }
            break;
        default:
            break;
        }
       
    }
    return true;
}

void UGameWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
    Super::Initialize(Collection);

    Init();
}

void UGameWorldSubsystem::Init_Implementation() {

}