#include <assert.h>
#include <float.h>
#include <math.h>

#include "Framework.h"
#include "Tilemap.h"
#include "Pathfinder.h"

Pathfinder::Pathfinder(Tilemap* pTilemap, int width, int height)
{
    m_pTilemap = pTilemap;
    m_NumNodes = width * height;
    m_Nodes = new PathNode[m_NumNodes];
    m_MapWidth = width;
    m_MapHeight = height;

    Reset();
}

Pathfinder::~Pathfinder()
{
    delete[] m_Nodes;
}

void Pathfinder::Reset()
{
    m_NumOpen = 0;

    // Reset all nodes
    for (int i = 0; i < m_NumNodes; i++)
    {
        m_Nodes[i].parentNodeIndex = -1;
        m_Nodes[i].status = PathNode::Unchecked;

        m_Nodes[i].cost = FLT_MAX;
        m_Nodes[i].estimate = FLT_MAX;
        m_Nodes[i].finalCost = FLT_MAX;
    }
}

bool Pathfinder::FindPath(int sx, int sy, int ex, int ey)
{

    if (!m_pTilemap->isWalkable(sx, sy) || !m_pTilemap->isWalkable(ex, ey)) {
        return false;  // Start or goal is not walkable
    }

    //TODO :: // Reset all nodes (parents to -1, all 3 costs to FLT_MAX and status to unchecked)
    Reset();

    // Get the starting tile index and the destination tile index
    int startIndex = sx + sy * m_MapWidth;

    // Set starting node cost to zero,
    m_Nodes[startIndex].cost = 0;
    // mark it open, 

    m_Nodes[startIndex].status = PathNode::Open;  // Start node is open
    // and add it to the open list

    std::vector<int> openList = { startIndex };
    // Keep looping until the open list is empty
    while (!openList.empty())
    {
        // Find the node with the lowest 'F' score from open list, make it the current node
        int currentIndex = openList[0];
        for (int i = 1; i < openList.size(); i++) {
            if (m_Nodes[openList[i]].finalCost < m_Nodes[currentIndex].finalCost) {
                currentIndex = openList[i];
            }
        }
        // Remove the node from the open list
        openList.erase(std::remove(openList.begin(), openList.end(), currentIndex), openList.end());

        // Check if the node is the destination, if so return true (found a path)
        int cx = currentIndex % m_MapWidth;
        int cy = currentIndex / m_MapWidth;

        // If goal reached, path found
        if (currentIndex == ex + ey * m_MapWidth) {
            return true;
        }

        // Mark the node as closed
        m_Nodes[currentIndex].status = PathNode::Closed;  // Mark as closed

        // Build a list of valid neighbours. Skip nodes that are closed or not walkable.

       // Explore neighbors (up, down, left, right)
        int dx[4] = { -1, 1, 0, 0 };
        int dy[4] = { 0, 0, -1, 1 };

        for (int i = 0; i < 4; i++)
        {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            if (nx >= 0 && ny >= 0 && nx < m_MapWidth && ny < m_MapHeight && m_pTilemap->isWalkable(nx, ny)) {
                int neighborIndex = nx + ny * m_MapWidth;

                // Skip if the neighbor is closed
                if (m_Nodes[neighborIndex].status == PathNode::Closed) continue;

                // Calculate new cost
                float newCost = m_Nodes[currentIndex].cost + 1;  // Assume uniform cost (1 for each step)
                float newEstimate = abs(ex - nx) + abs(ey - ny);  // Manhattan distance
                float newFinalCost = newCost + newEstimate;

                // If neighbor is unchecked or new cost is lower, update it
                if (m_Nodes[neighborIndex].status == PathNode::Unchecked || newFinalCost < m_Nodes[neighborIndex].finalCost) {
                    m_Nodes[neighborIndex].cost = newCost;
                    m_Nodes[neighborIndex].estimate = newEstimate;
                    m_Nodes[neighborIndex].finalCost = newFinalCost;
                    m_Nodes[neighborIndex].parentNodeIndex = currentIndex;

                    // Add neighbor to open list if it's unchecked
                    if (m_Nodes[neighborIndex].status == PathNode::Unchecked) {
                        openList.push_back(neighborIndex);
                        m_Nodes[neighborIndex].status = PathNode::Open;
                    }
                }
            }
        }
    }

    return false;  // No path found
}

std::vector<int> Pathfinder::GetPath(int ex, int ey)
{
    return std::vector<int>();

    std::vector<int> path;
    int currentIndex = ex + ey * m_MapWidth;

    // Reconstruct the path by following parent nodes
    while (currentIndex != -1) {
        path.push_back(currentIndex);
        currentIndex = m_Nodes[currentIndex].parentNodeIndex;
    }

    std::reverse(path.begin(), path.end());  // Reverse to get the path from start to goal
    return path;

}
