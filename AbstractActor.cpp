#include "AbstractActor.h"
#include <iostream>

AbstractActor::AbstractActor():_executers(std::map<int, std::shared_ptr<AbstractExecuter>>())
{
}

