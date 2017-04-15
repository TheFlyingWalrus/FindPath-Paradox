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

    // Interleaving some data for better cache-usage
    std::vector<int> data;
    data.resize(mapSize*2);
    int offsetCost = 0;
    int offsetHeuristic = 1;

    for(int i = 0; i < mapSize; i++)
    {
        data[offsetCost + i*2] = -1;
        closed[i] = false;
    }

    int currentIndex = startIndex;
    data[offsetCost + currentIndex*2] = 0;
    open.push_back(currentIndex);

    while( currentIndex != targetIndex )
    {
        if( open.empty() )
            return -1;

        auto lowestCost = (open.begin())++;
        for( auto it = lowestCost; it != open.end(); ++it)
        {
            if( data[offsetCost + *lowestCost*2] + data[offsetHeuristic + *lowestCost*2]
                > data[offsetCost + *it*2] + data[offsetHeuristic + *it*2] )
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
                int newCost = data[offsetCost + currentIndex*2] + 1;
                if( data[offsetCost + n*2] == -1 )
                {
                    data[offsetCost + n*2] = newCost;
                    data[offsetHeuristic + n*2] = heuristic(n, targetIndex, nMapWidth);
                    cameFrom[n] = currentIndex;
                    open.push_back(n);
                }else if ( data[offsetCost + n*2] > newCost )
                {
                    data[offsetCost + n*2] = newCost;
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
