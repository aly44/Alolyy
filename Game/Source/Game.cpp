#include "Framework.h"
#include "Game.h"
#include "Tilemap.h"
#include "Camera.h"
#include "Enemy.h"

using fw::vec2;
using fw::ivec2;

Game::Game(fw::FWCore& framework)
    : fw::GameCore(framework)
{

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_pCamera = new Camera(this);
    m_pShader = new fw::ShaderProgram("Data/Shaders/basic.vert", "Data/Shaders/basic.frag");

    std::vector<fw::VertexFormat> attribs = {
        { {0.0f, 0.0f}, 255, 255, 255, 255, {0, 0} },
        { {0.0f, 1.0f}, 255, 255, 255, 255, {0, 1} },
        { {1.0f, 0.0f}, 255, 255, 255, 255, {1, 0} },
        { {1.0f, 1.0f}, 255, 255, 255, 255, {1, 1} },
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
        ".S............E....."
        "...................."
        "...................."
        "...................."
        "...................."
        "...................."
        "...................."
        "...................."
        "...................."
        "...................."
        "...................."
        "...................."
        "...................."
        "...................."
        "...................."
        "...................."
        "....................";

    m_pTilemap = new Tilemap(this, tilemap1, { 20, 17 }, vec2(2, 2));
    m_pEnemyTexture = new fw::Texture("Data/Textures/player_down1.png");

    // Create and add an enemy
    fw::vec2 startTile = m_pTilemap->findStartPos();
    fw::vec2 endTile = m_pTilemap->findEndPos();

    if (startTile != fw::vec2(-1, -1) && endTile != fw::vec2(-1, -1))
    {
        fw::vec2 startWorldPos = fw::vec2(startTile.x * m_pTilemap->m_tileSize.x,
            startTile.y * m_pTilemap->m_tileSize.y);
        fw::vec2 endWorldPos = fw::vec2(endTile.x * m_pTilemap->m_tileSize.x,
            endTile.y * m_pTilemap->m_tileSize.y);

        Enemy* newEnemy = new Enemy(this, m_pMesh, m_pShader, nullptr, startWorldPos);
        newEnemy->m_pTexture = m_pEnemyTexture;
        newEnemy->m_targetPosition = endWorldPos; // Set the target position
        m_Enemies.push_back(newEnemy);
    }
}

Game::~Game()
{
    delete m_pTilemap;
    delete m_pEnemyTexture;

    for (Enemy* enemy : m_Enemies)
        delete enemy;
}

void Game::update(float deltaTime)
{
    for (Enemy* enemy : m_Enemies)
    {
        if (enemy->m_IsAlive)
            enemy->update(deltaTime);
    }
}

void Game::draw()
{
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_pTilemap->draw(this);

    for (Enemy* enemy : m_Enemies)
    {
        if (enemy->m_IsAlive)
            enemy->draw();
    }

    m_framework.swapBuffers();
}

void Game::onMouseMove(float x, float y)
{
    vec2 worldPos = m_pCamera->convertScreenPositionToWorldPosition(vec2(x, y));
    fw::outputMessage("World Pos: %0.2f, %0.2f\n", worldPos.x, worldPos.y);
}
