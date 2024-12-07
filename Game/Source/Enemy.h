#pragma once

#include "Framework.h"

using fw::vec2;

class Game;
class Pathfinder;
class Texture;
class Mesh;
class ShaderProgram;

class Enemy
{

public:
    Enemy(Game* pGame, fw::Mesh* pMesh, fw::ShaderProgram* pShader, Pathfinder* pPathfinder, fw::vec2 position);
    virtual ~Enemy();

    void update(float deltaTime) ;
    void draw() ;

    void GetPath();

    std::vector<fw::vec2> m_waypoints;
    int m_currentWaypointIndex = 0;
    bool m_IsAlive;


    Game* m_pGame = nullptr;

    fw::Texture* m_pTexture = nullptr;
    fw::Mesh* m_pMesh = nullptr;
    fw::ShaderProgram* m_pShader = nullptr;


    fw::vec2 m_uvOffset = { 0.0f, 0.0f };
    fw::vec2 m_uvScale = { 1.0f, 1.0f };

    fw::vec2 m_position;
    fw::vec2 m_velocity = { 0.0f, 0.0f };
    float m_speed = 2.5f;

    std::vector<int> m_tileWaypoints;

    //fw::vec2 m_targetWaypoint;         

    Pathfinder* m_pPathfinder;

};
