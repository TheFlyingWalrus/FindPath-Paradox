#include <iostream>
#include <vector>

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
    int mapSize = nMapWidth * nMapHeight;

    std::vector<int> heuristicMap;
    heuristicMap.resize(mapSize);
    std::vector<int> costMap;
    costMap.resize(mapSize);
    std::vector<int> cameFrom;
    cameFrom.resize(mapSize);
    std::vector<bool> closed;
    closed.resize(mapSize);

    for(int i = 0; i < mapSize; i++)
    {
        costMap[i] = -1;
        closed[i] = false;
    }

    int currentIndex = startIndex;
    costMap[currentIndex] = 0;
    open.push_back(currentIndex);

    while( currentIndex != targetIndex )
    {
        if( open.empty() )
            return -1;

        auto lowestCost = (open.begin())++;
        for( auto it = lowestCost; it != open.end(); ++it)
        {
            if( costMap[*lowestCost] + heuristicMap[*lowestCost] > costMap[*it] + heuristicMap[*it] )
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
        {
            neighbours.push_back(currentIndex + nMapWidth);
        }
        if( currentIndex % nMapWidth != 0 )
            neighbours.push_back(currentIndex - 1);
        if( currentIndex % nMapWidth != nMapWidth - 1 )
            neighbours.push_back(currentIndex + 1);

        for( int n : neighbours )
        {
            if (pMap[n] == 1 && !closed[n] )
            {
                int newCost = costMap[currentIndex] + 1;

                if( costMap[n] == -1 )
                {
                    costMap[n] = newCost;
                    heuristicMap[n] = heuristic(n, targetIndex, nMapWidth);
                    cameFrom[n] = currentIndex;
                    open.push_back(n);
                }else if ( costMap[n] > newCost )
                {
                    costMap[n] = newCost;
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
