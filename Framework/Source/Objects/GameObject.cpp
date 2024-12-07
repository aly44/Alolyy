#include "Core/CoreHeaders.h"

#include "GameObject.h"
#include "Mesh.h"

fw::GameObject::GameObject(GameCore* pGameCore, Mesh* pMesh, ShaderProgram* pShader)
    : m_pGameCore( pGameCore )
    , m_pMesh( pMesh )
    , m_pShader( pShader )
{
}

fw::GameObject::~GameObject()
{
}

void fw::GameObject::update(float deltaTime)
{
}

void fw::GameObject::draw()
{
    m_pMesh->draw( m_pShader, m_position, nullptr );
}
