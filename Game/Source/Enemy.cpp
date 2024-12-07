#include "Framework.h"
#include "Enemy.h"
#include "Game.h"
#include "Pathfinder.h"


Enemy::Enemy(Game* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader, Pathfinder* pPathfinder, fw::vec2 position)
: m_pGame(pGame),
  m_speed(2.5f),
  m_currentWaypointIndex(0),
  m_pPathfinder(pPathfinder),
  m_position(position)
{
}

Enemy::~Enemy()
{
}

void Enemy::update(float deltaTime)
{
    // Update the current animation

    if (m_waypoints.empty() || m_currentWaypointIndex >= m_waypoints.size())
    {
        m_IsAlive = false; // Stop enemy if the path is exhausted
        return;
    }

    fw::vec2 targetTile = m_waypoints[m_currentWaypointIndex];
    fw::vec2 targetWorldPosition = fw::vec2(targetTile.x * 2.0f, targetTile.y * 2.0f); // Adjust to match TileToWorld scale

    fw::vec2 direction = targetWorldPosition - m_position;
    float distance = direction.length();

    if (distance > 0.1f) // Move towards the target tile
    {
        direction.normalize();
        m_position += direction * m_speed * deltaTime;
    }
    else
    {
        m_currentWaypointIndex++; // Move to the next tile in the path
    }

    //Check this later

    // Play walk animation while moving
    //m_animationPlayer.play("Idle");
}

void Enemy::draw()
{
}

void Enemy::GetPath()
{
    int sx = 0;
    int sy = 0;
    int ex = 0;
    int ey = 0;

    if (m_pPathfinder->FindPath(sx, sy, ex, ey))
    {
        m_tileWaypoints = m_pPathfinder->GetPath(ex, ey);
    }
}
