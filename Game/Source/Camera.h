#pragma once

using fw::vec2;
using fw::ivec2;

class Game;

class Camera
{
public:
    Camera(Game* pGame);
    ~Camera();

    vec2 convertScreenPositionToWorldPosition(vec2 screenPos);

protected:
    Game* m_pGame = nullptr;

    vec2 m_cameraPosition = vec2(10.0f, 10.0f);
    float m_aspectRatio = 1280.0f / 720.0f;
    vec2 m_zoom = vec2(1.0f / (12.0f * (m_aspectRatio)), 1.0f / 12.0f);
};
