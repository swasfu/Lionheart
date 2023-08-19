#include "registry/registry.h"

EntityID Registry::RegisterEntity()
{
	return entityCounter_++;
}
