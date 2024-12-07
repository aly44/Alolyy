#pragma once
#include "Framework.h"
#include "Enemy.h"
#include "Pathfinder.h"

class Camera;
class Tilemap;
class Enemy;
class Pthfinder;


class Game : public fw::GameCore
{
public:
    Game(fw::FWCore& framework);
    ~Game();

    virtual void update(float deltaTime) override;
    virtual void draw() override;

    virtual void onKeyPressed(int key) override {}
    virtual void onKeyReleased(int key) override {}
    virtual void onMouseMove(float x, float y) override;
    virtual void onMouseButtonDown(int mouseId, float x, float y) {}

    Camera* m_pCamera;

    fw::ShaderProgram* m_pShader;
    fw::Mesh* m_pMesh;
    fw::Texture* m_pTextures[7];

    Tilemap* m_pTilemap;
    Pathfinder* m_pPathfinder;

    fw::Texture* m_pEnemyTexture = nullptr; // Texture for enemy sprites
    std::vector<Enemy*> m_Enemies; // Vector to store enemy objects

};
