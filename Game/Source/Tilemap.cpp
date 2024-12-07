#include "Framework.h"
#include "Tilemap.h"
#include "Game.h"

Tilemap::Tilemap(Game* pGame, char* layout, ivec2 mapSize, vec2 tileSize)
    :  m_mapSize(mapSize), m_tileSize(tileSize)

{
    m_layout = new TileType[mapSize.x * mapSize.y];

    for (int i = 0; i < mapSize.x * mapSize.y; i++)
    {
        if (layout[i] == '.') m_layout[i] = TileType::Green;
        if (layout[i] == 'X') m_layout[i] = TileType::Brown;
        if (layout[i] == 'O') m_layout[i] = TileType::Gray;
        if (layout[i] == 'R') m_layout[i] = TileType::Gem;
        if (layout[i] == 'W') m_layout[i] = TileType::Wizard;
        if (layout[i] == 'S') m_layout[i] = TileType::Start;
        if (layout[i] == 'E') m_layout[i] = TileType::End;
    }                         

    m_properties[TileType::Green] = { pGame->m_pTextures[0], true };
    m_properties[TileType::Brown] = { pGame->m_pTextures[1], false };
    m_properties[TileType::Gray] = { pGame->m_pTextures[2], false };
    m_properties[TileType::Gem] = { pGame->m_pTextures[3], false };
    m_properties[TileType::Wizard] = { pGame->m_pTextures[4], false };
    m_properties[TileType::Start] = { pGame->m_pTextures[5], false };
    m_properties[TileType::End] = { pGame->m_pTextures[6], false };
}

Tilemap::~Tilemap()
{
}

void Tilemap::draw(Game* pGame)
{

    for (int y = 0; y < m_mapSize.y; y++)
    {
        for (int x = 0; x < m_mapSize.x; x++)
        {
            int tileIndex = y * m_mapSize.x + x;

            TileType type = m_layout[tileIndex];

            fw::Texture* texture = m_properties[type].m_pTexture;

            pGame->m_pMesh->draw( pGame->m_pShader, vec2(x, y) * m_tileSize, texture );
        }
    }
}

bool Tilemap::isWalkable(int x, int y) const
{
    if (x < 0 || y < 0 || x >= 10 || y >= 7)
        return false;


    return m_layout[y * 10 + x] == TileType::Green;
}

void Tilemap::setTileWalkable(int x, int y, bool walkable)
{
    if (x < 0 || y < 0 || x >= 10 || y >= 7)
        return;


    if (walkable)
        m_layout[y * 10 + x] = TileType::Green;
    else
        m_layout[y * 10 + x] = TileType::Brown;
}

fw::vec2 Tilemap::findStartPos()
{
    for (int y = 0; y < m_mapSize.y; y++) // Loop through rows
    {
        for (int x = 0; x < m_mapSize.x; x++) // Loop through columns
        {
            int index = y * m_mapSize.x + x; // Calculate the index
            if (m_layout[index] == TileType::Start) // Check for Start tile
            {
                return fw::vec2((float)x, (float)y); // Return position
            }
        }
    }
    return fw::vec2(-1, -1); // Return invalid position if not found
}

vec2 Tilemap::findEndPos()
{
    for (int y = 0; y < m_mapSize.y; y++) // Loop through rows
    {
        for (int x = 0; x < m_mapSize.x; x++) // Loop through columns
        {
            int index = y * m_mapSize.x + x; // Calculate the index
            if (m_layout[index] == TileType::End) // Check for End tile
            {
                return fw::vec2((float)x, (float)y); // Return position
            }
        }
    }
    return fw::vec2(-1, -1); // Return invalid position if not found

}

void Tilemap::getWorldPos(ivec2)
{

}

void Tilemap::getTilePos(vec2)
{

}

