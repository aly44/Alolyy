#pragma once

#include "Framework.h"

using fw::vec2;

class Game;
class Pathfinder;

class Enemy
{
public:
    Enemy(Game* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader, Pathfinder* pPathfinder, fw::vec2 position);
    virtual ~Enemy() = default;

    void update(float deltaTime);
    void draw();

    bool m_IsAlive = true;             // Enemy alive status
    fw::vec2 m_position;              // Current position of the enemy
    fw::vec2 m_targetPosition;        // Target position (end position)

    Game* m_pGame = nullptr;           // Pointer to the game instance
    fw::Mesh* m_pMesh = nullptr;       // Mesh for rendering the enemy
    fw::ShaderProgram* m_pShader = nullptr; // Shader for rendering
    fw::Texture* m_pTexture = nullptr; // Texture for the enemy
    float m_speed = 2.5f;              // Movement speed of the enemy
};
