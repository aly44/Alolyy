#include "Core/CoreHeaders.h"

#include "Core/FWCore.h"
#include "Core/GameCore.h"

namespace fw {

fw::GameCore::GameCore(FWCore& framework)
    : m_framework( framework)
{
}

GameCore::~GameCore()
{
}

} // namespace fw
