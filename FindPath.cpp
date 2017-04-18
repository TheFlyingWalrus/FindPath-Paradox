#include <iostream>
#include <vector>
#include <cstdlib>

inline int heuristic(int fromIndex, int targetIndex, int w)
{
    return std::abs(fromIndex / w - targetIndex / w) + std::abs(fromIndex % w - targetIndex % w);
}

int FindPath(const int nStartX, const int nStartY,
             const int nTargetX, const int nTargetY,
             const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
             int* pOutBuffer, const int nOutBufferSize)
{
    if( nStartX == nTargetX && nStartY == nTargetY )
    {
        return 0;
    }

    std::vector<int> open;

    int startIndex = nStartX + nStartY * nMapWidth;
    int targetIndex = nTargetX + nTargetY * nMapWidth;
    const int mapSize = nMapWidth * nMapHeight;

    std::vector<bool> closed;
    closed.resize(mapSize);
    std::vector<int> cameFrom;
    cameFrom.resize(mapSize);

    struct Node
    {
      int gcost;
      int hcost;
    };
    
    std::vector<Node> nodes;
    nodes.resize(mapSize);

    for(int i = 0; i < mapSize; i++)
    {
        nodes[i].gcost = -1;
        closed[i] = false;
    }

    int currentIndex = startIndex;
    nodes[startIndex].gcost = 0;
    open.push_back(currentIndex);

    while( currentIndex != targetIndex )
    {
        if( open.empty() )
            return -1;

        auto lowestCost = (open.begin())++;
        for( auto it = lowestCost; it != open.end(); ++it)
        {
            if( nodes[*lowestCost].gcost + nodes[*lowestCost].hcost < nodes[*it].gcost + nodes[*it].hcost )
            {
                lowestCost = it;
            }
        }

        currentIndex = *lowestCost;
        open.erase(lowestCost);
        closed[currentIndex] = true;

        std::vector<int> neighbours;

        if( currentIndex >= nMapWidth )
            neighbours.push_back(currentIndex - nMapWidth);
        if( currentIndex + nMapWidth < mapSize )
            neighbours.push_back(currentIndex + nMapWidth);
        if( currentIndex != 0 )
            neighbours.push_back(currentIndex - 1);
        if( currentIndex != mapSize - 1 )
            neighbours.push_back(currentIndex + 1);

        for( int n : neighbours )
        {
            if (pMap[n] == 1 && !closed[n] )
            {
	        int newCost = nodes[currentIndex].gcost + 1;
                if( nodes[n].gcost == -1 )
                {
                    nodes[n].gcost = newCost;
                    nodes[n].hcost = heuristic(n, targetIndex, nMapWidth);
                    cameFrom[n] = currentIndex;
                    open.push_back(n);
                }else if ( nodes[n].gcost > newCost )
                {
                    nodes[n].gcost = newCost;
                    cameFrom[n] = currentIndex;
                }
            }
        }
    }

    currentIndex = targetIndex;
    std::vector<int> path;

    while( currentIndex != startIndex )
    {
        path.push_back(currentIndex);
        currentIndex = cameFrom[currentIndex];
    }

    if( path.size() <= nOutBufferSize )
    {
        for( size_t i = 0; i < path.size(); i++)
            pOutBuffer[i] = path[path.size() - i - 1];
    }

    return path.size();
}
