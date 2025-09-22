#include "MyGameMode.h"
#include "Drone.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = ADrone::StaticClass();
}
