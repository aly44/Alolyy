#pragma once

// This header includes all files inside the framework project.
// It is meant for use by game projects ONLY.
// As new files are created, include them here so the game has easy access to them.
// It should **not** be included by any files inside the framework project.

#if GAME_PROJECT

#include "Core/CoreHeaders.h"

#include "Core/FWCore.h"
#include "Core/FWWindows.h"
#include "Core/GameCore.h"
#include "Math/vector.h"
#include "Mesh.h"
#include "Objects/GameObject.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Utility.h"

#endif // GAME_PROJECT
