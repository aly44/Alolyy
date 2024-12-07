#pragma once

#include "Math/vector.h"

namespace fw {

    class GameCore;
    class Mesh;
    class ShaderProgram;

    class GameObject
    {
    public:
        GameObject(GameCore* pGameCore, Mesh* pMesh, ShaderProgram* pShader);
        virtual ~GameObject();

        virtual void update(float deltaTime);
        virtual void draw();

        void setPosition(const vec2& pos) { m_position = pos; }
        vec2 getPosition() const { return m_position; }

    protected:
        GameCore* m_pGameCore = nullptr;
        Mesh* m_pMesh = nullptr;
        ShaderProgram* m_pShader = nullptr;

        vec2 m_position;
    };

} // namespace fw
