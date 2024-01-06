#include "Globals.hpp"
#include "Generation.hpp"

void wall(int mazeWidth, int mazeHeight, int m_totalVisitedCells, std::vector<std::vector<int>> &m_connectedAdjList, std::pair<float, float> (&cell_coordinates)[1000][1000], std::vector<std::pair<FLOAT_PAIR, int>> &walls)
{
    srand(time(NULL));
    std::vector <std::vector <int>> m_visited(mazeWidth, std::vector<int> (mazeHeight, 0));
    std::stack<std::pair<int, int>> m_stack;
    m_stack.push({0,0});
    m_visited[0][0] = 1;
    m_totalVisitedCells++;
    while(!m_stack.empty())
    {
        std::vector<int> neighbours;
        
        //right neighbour - 0 - {m_stack.top().first + 1, m_stack.top().second}
        if (m_stack.top().first + 1 < mazeWidth && m_visited[m_stack.top().first + 1][m_stack.top().second] != 1)
        {
            neighbours.push_back(0);
        }
        //up neighbour - 1 {m_stack.top().first, m_stack.top().second - 1}
        if (m_stack.top().second - 1 >= 0 && m_visited[m_stack.top().first][m_stack.top().second - 1] != 1)
        {
            neighbours.push_back(1);
        }
        //left neighbour - 2 {m_stack.top().first - 1, m_stack.top().second}
        if (m_stack.top().first - 1 >= 0 && m_visited[m_stack.top().first - 1][m_stack.top().second] != 1)
        {
            neighbours.push_back(2);
        }
        //down neighbour - 3 {m_stack.top().first, m_stack.top().second + 1}
        if (m_stack.top().second + 1 < mazeHeight && m_visited[m_stack.top().first][m_stack.top().second + 1] != 1)
        {
            neighbours.push_back(3);
        }

        if (!neighbours.empty())
        {
            int randomCell = neighbours[rand() % neighbours.size()];
            int randomCellx;
            int randomCelly;
            if (randomCell == 0)
            {
                randomCellx = m_stack.top().first + 1;
                randomCelly = m_stack.top().second;
            }
            if (randomCell == 1)
            {
                randomCellx = m_stack.top().first;
                randomCelly = m_stack.top().second - 1;
            }
            if (randomCell == 2)
            {
                randomCellx = m_stack.top().first - 1;
                randomCelly = m_stack.top().second;
            }
            if (randomCell == 3)
            {
                randomCellx = m_stack.top().first;
                randomCelly = m_stack.top().second + 1;
            }


/*---------------------------------added by PRITOMASH---------------------------------------*/
            //to link data with Solver algorithm

            int currentCell = xyToInt(m_stack.top().first, m_stack.top().second, mazeWidth);
            int neighbourCell = xyToInt(randomCellx, randomCelly, mazeWidth);
            m_connectedAdjList[currentCell].push_back(neighbourCell);
            m_connectedAdjList[neighbourCell].push_back(currentCell);
            float cx = (cell_coordinates[randomCellx][randomCelly]).first;
            float cy = (cell_coordinates[randomCellx][randomCelly]).second;

            walls.push_back({std::make_pair(cx, cy), randomCell});

/*---------------------------------end of PRITOMASH edit-----------------------------------*/
            m_stack.push({randomCellx, randomCelly});
            m_visited[randomCellx][randomCelly] = 1;
            m_totalVisitedCells++;


        }
        else
        {
            m_stack.pop();
        }

    }
}
