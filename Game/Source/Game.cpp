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
    m_pTextures[0] = new fw::Texture( "Data/Textures/ground_01.png" );
    m_pTextures[1] = new fw::Texture( "Data/Textures/ground_05.png" );
    m_pTextures[2] = new fw::Texture( "Data/Textures/ground_06.png" );

    char tilemap1[10 * 7 + 1] =
        ".........."
        ".........."
        ".........."
        "...XX....."
        ".........."
        ".........."
        "....XX....";

    m_pTilemap = new Tilemap( this, tilemap1, { 10,7 }, vec2(2, 2) );
}

Game::~Game()
{
}

void Game::update(float deltaTime)
{
}

void Game::draw()
{
    glClearColor( 0.0f, 0.0f, 0.2f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    m_pTilemap->draw( this );

    m_framework.swapBuffers();
}

void Game::onMouseMove(float x, float y)
{
    vec2 worldPos = m_pCamera->convertScreenPositionToWorldPosition( vec2(x,y) );

    fw::outputMessage( "World Pos: %0.2f, %0.2f\n", worldPos.x, worldPos.y );
}
