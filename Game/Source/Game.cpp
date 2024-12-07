#include "Framework.h"
#include "Game.h"
#include "Tilemap.h"
#include "Camera.h"

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

    char tilemap1[10 * 7 + 1] =
        ".S........"
        ".........."
        ".........."
        "...XX....."
        ".........."
        "........E."
        "....XX....";

    m_pTilemap = new Tilemap( this, tilemap1, { 10,7 }, vec2(2, 2) );


    // Load enemy texture
    m_pEnemyTexture = new fw::Texture("Data/Textures/player_down1.png");

    // Create and add an enemy
    Enemy* newEnemy = new Enemy(this, m_pMesh, m_pShader, nullptr, vec2(2, 2));
    newEnemy->m_pTexture = m_pEnemyTexture; // Assign texture to the enemy
    m_Enemies.push_back(newEnemy);
}

Game::~Game()
{
}

void Game::update(float deltaTime)
{
    for (Enemy* enemy : m_Enemies)
    {
        if (enemy->m_IsAlive) // Update only active enemies
        {
            enemy->update(deltaTime);
        }
    }
}


void Game::draw()
{
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw tilemap
    m_pTilemap->draw(this);

    // Draw enemies
    for (Enemy* enemy : m_Enemies)
    {
        if (enemy->m_IsAlive) // Draw only active enemies
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
