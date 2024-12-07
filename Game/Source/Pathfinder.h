#ifndef __PathFinder_H__
#define __PathFinder_H__

#include <vector>
#include "Tilemap.h" 

// Structure for each tile in the pathfinding gr
struct PathNode
{
    enum PathNodeStatus
    {
        Unchecked,
        Open,
        Closed,
    };

    int parentNodeIndex;
    PathNodeStatus status;

    float cost;           // G: Cost to reach this node
    float estimate;       // H: Estimated cost to reach the goal
    float finalCost;      // F: Total cost (G + H)
};

class Pathfinder
{
public:

    Pathfinder(Tilemap* pTilemap, int width, int height);
    virtual ~Pathfinder();

    void Reset();
    bool FindPath(int sx, int sy, int ex, int ey);

    std::vector<int> GetPath(int ex, int ey);


protected:
    int m_MapWidth;
    int m_MapHeight;
    Tilemap* m_pTilemap;

    PathNode* m_Nodes;
    int m_NumNodes;

    std::vector<int> m_OpenNodeIndexes;
    int m_NumOpen;


};

#endif // __PathFinder_H__
