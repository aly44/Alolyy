#pragma once

using fw::vec2;
using fw::ivec2;

class Game;

class Tilemap
{
public:
	enum TileType
	{
		Green,
		Brown,
		Gray,
		Gem,
		Wizard,
		Start,
		End,
		NumTypes,
	};

	struct TileProperties
	{

		fw::Texture* m_pTexture;
		bool m_walkable;

	};

public:
	Tilemap(Game* pGame, char* layout, ivec2 mapSize, vec2 tileSize);
	~Tilemap();

	void draw(Game* pGame);

	bool isWalkable(int x, int y) const;
	void setTileWalkable(int x, int y, bool walkable);

	vec2 findStartPos();
	vec2 findEndPos();

	void getWorldPos(ivec2);
	void getTilePos(vec2);

	TileType* m_layout;
	TileProperties m_properties[TileType::NumTypes];

	vec2 m_tileSize;
	ivec2 m_mapSize;
};



