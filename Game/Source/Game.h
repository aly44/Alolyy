#pragma once
#include "Framework.h"

class Camera;
class Tilemap;

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
};
