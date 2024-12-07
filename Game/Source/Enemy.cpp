#include "Framework.h"
#include "Enemy.h"

Enemy::Enemy(Game* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader, Pathfinder* pPathfinder, fw::vec2 position)
    : m_pGame(pGame)
    , m_pMesh(pMesh)
    , m_pShader(pShader)
    , m_position(position)
    , m_targetPosition(vec2(0.0f, 0.0f))
    , m_speed(2.5f)
    , m_IsAlive(true)
{
}

void Enemy::update(float deltaTime)
{
    if (!m_IsAlive)
        return;

    // Move towards the target position
    fw::vec2 direction = m_targetPosition - m_position;
    if (direction.length() > 0.1f)
    {
        direction.normalize();
        m_position += direction * m_speed * deltaTime;
    }
    else
    {
        // Reached the target position
        m_IsAlive = false;
    }
}

void Enemy::draw()
{
    if (!m_IsAlive)
        return;

    m_pMesh->draw(m_pShader, m_position, m_pTexture);
}
