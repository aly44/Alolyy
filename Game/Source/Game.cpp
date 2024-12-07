#include "Framework.h"
#include "Game.h"
#include "Tilemap.h"
#include "Camera.h"
#include "Pathfinder.h"

using fw::vec2;
using fw::ivec2;

Game::Game(fw::FWCore& framework)
    : fw::GameCore( framework )
{
    m_pCamera = new Camera( this );
    m_pShader = new fw::ShaderProgram( "Data/Shaders/basic.vert", "Data/Shaders/basic.frag" );

    std::vector<fw::VertexFormat> attribs = {
        { { 0.0f,  0.0f},  255,255,255,255,  {0,0} },
        { { 0.0f,  1.0f},  255,255,255,255,  {0,1} },
        { { 1.0f,  0.0f},  255,255,255,255,  {1,0} },
        { { 1.0f,  1.0f},  255,255,255,255,  {1,1} },
    };
    m_pMesh = new fw::Mesh( attribs, GL_TRIANGLE_STRIP );
    m_pTextures[0] = new fw::Texture( "Data/Textures/ground_05.png" );
    m_pTextures[1] = new fw::Texture( "Data/Textures/ground_01.png" );
    m_pTextures[2] = new fw::Texture( "Data/Textures/ground_01.png" );
    m_pTextures[3] = new fw::Texture( "Data/Textures/ground_01.png" );
    m_pTextures[4] = new fw::Texture( "Data/Textures/ground_01.png" );
    m_pTextures[5] = new fw::Texture( "Data/Textures/ground_06.png" );
    m_pTextures[6] = new fw::Texture( "Data/Textures/ground_06.png" );

    char tilemap1[20 * 17 + 1] =
        ".S.................."
        "...................."
        "...................."
        "...XX..............."
        "...................."
        "........E..........."
        "....XX.............."
        "....XX.............."
        "....XX.............."
        "....XX.............."
        "....XX.............."
        "....XX.............."
        "....XX.............."
        "....XX.............."
        "....XX.............."
        "....XX.............."
        "....XX..............";

    m_pTilemap = new Tilemap( this, tilemap1, { 20,17 }, vec2(2, 2) );


    // Load enemy texture

    // Create and add an enemy
    Enemy* newEnemy = new Enemy(this, m_pMesh, m_pShader, nullptr, vec2(2, 2));
    newEnemy->m_pTexture = m_pEnemyTexture; // Assign texture to the enemy
    m_Enemies.push_back(newEnemy);
    m_pEnemyTexture = new fw::Texture("Data/Textures/player_down1.png");

    m_pPathfinder = new Pathfinder(m_pTilemap, m_pTilemap->m_mapSize.x, m_pTilemap->m_mapSize.y);

}

Game::~Game()
{
}

void Game::update(float deltaTime)
{
    static bool enemySpawned = false; // Track if the enemy has already been spawned

    if (!enemySpawned)
    {
        // Get start and end tile positions
        fw::vec2 startTile = m_pTilemap->findStartPos();
        fw::vec2 endTile = m_pTilemap->findEndPos();

        if (startTile != fw::vec2(-1, -1) && endTile != fw::vec2(-1, -1)) // Ensure start and end tiles exist
        {
            // Convert start and end positions to tile coordinates
            int startX = static_cast<int>(startTile.x);
            int startY = static_cast<int>(startTile.y);
            int endX = static_cast<int>(endTile.x);
            int endY = static_cast<int>(endTile.y);

            // Use Pathfinder to calculate the path
            if (m_pPathfinder->FindPath(startX, startY, endX, endY))
            {
                std::vector<int> pathIndices = m_pPathfinder->GetPath(endX, endY);
                std::vector<fw::vec2> waypoints;

                // Convert path indices to tile positions
                for (int index : pathIndices)
                {
                    int tileX = index % m_pTilemap->m_mapSize.x;
                    int tileY = index / m_pTilemap->m_mapSize.x;
                    waypoints.emplace_back(fw::vec2(tileX, tileY));
                }

                // Create the enemy at the start position
                fw::vec2 startWorldPos = fw::vec2(startTile.x * m_pTilemap->m_tileSize.x,
                    startTile.y * m_pTilemap->m_tileSize.y);
                Enemy* newEnemy = new Enemy(this, m_pMesh, m_pShader, m_pPathfinder, startWorldPos);
                newEnemy->m_waypoints = waypoints; // Assign the calculated path
                newEnemy->m_currentWaypointIndex = 0; // Start at the first waypoint
                newEnemy->m_pTexture = m_pEnemyTexture;

                m_Enemies.push_back(newEnemy); // Add enemy to the list
                enemySpawned = true; // Prevent spawning multiple enemies
            }
        }
    }

    // Update all enemies
    for (Enemy* enemy : m_Enemies)
    {
        if (enemy->m_IsAlive)
        {
            enemy->update(deltaTime);
        }
    }
}



void Game::draw()
{
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the tilemap
    m_pTilemap->draw(this);

    // Draw all enemies
    for (Enemy* enemy : m_Enemies)
    {
        if (enemy->m_IsAlive)
        {
            enemy->draw();
        }
    }

    m_framework.swapBuffers();
}



void Game::onMouseMove(float x, float y)
{
    vec2 worldPos = m_pCamera->convertScreenPositionToWorldPosition( vec2(x,y) );

    fw::outputMessage( "World Pos: %0.2f, %0.2f\n", worldPos.x, worldPos.y );
}
