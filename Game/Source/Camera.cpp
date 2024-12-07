#include "Framework.h"
#include "Camera.h"
#include "Game.h"

Camera::Camera(Game* pGame)
    : m_pGame( pGame )
{
}

Camera::~Camera()
{
}

vec2 Camera::convertScreenPositionToWorldPosition(vec2 screenPos)
{
    vec2 resolution = vec2((float)m_pGame->getFramework().getWindowWidth(), (float)m_pGame->getFramework().getWindowHeight());

    vec2 flippedPos = vec2( screenPos.x, resolution.y - screenPos.y );
    vec2 clipPos = flippedPos / (resolution / 2) - 1;

    vec2 viewPos = clipPos / m_zoom;

    vec2 worldPos = viewPos + m_cameraPosition;

    return worldPos;
}
