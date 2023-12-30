#include <iostream>
#include <stack>
#include <queue>
#include <list>
#include <cmath>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

int mazeWidth;
int mazeHeight;
int scale = 2;
std::vector<int> shortestPath;

bool BFS(std::vector< std::vector<int> > &adj, int src, int dest)
{
    int v = mazeWidth * mazeHeight;
    bool bfs_visited[v];
    std::list<int> queue;
    int prev[v];
    int dist[v];
    bool pathFound = false;

    for (int i = 0; i < v; i++)
    {
        bfs_visited[i] = false;
        prev[i] = -1;
        dist[i] = INT32_MAX;
    }

    bfs_visited[src] = true;
    dist[src] = 0;
    queue.push_back(src);

    while (!queue.empty())
    {
        int u = queue.front();
        queue.pop_front();
        for (int i = 0; i < adj[u].size(); i++) {
            if (bfs_visited[adj[u][i]] == false) {
                bfs_visited[adj[u][i]] = true;
                dist[adj[u][i]] = dist[u] + 1;
                prev[adj[u][i]] = u;
                queue.push_back(adj[u][i]);
 
                // We stop BFS when we find
                // destination.
                if (adj[u][i] == dest)
                {
                    pathFound = true;
                    break;
                }
            }
        }
        if (pathFound)
            break;
    }
    if (pathFound)
    {
        int crawl = dest;
        shortestPath.push_back(crawl);
        while (prev[crawl] != -1) {
            shortestPath.push_back(prev[crawl]);
            crawl = prev[crawl];
        }
    }
    return pathFound;

}

sf::RectangleShape drawWhiteSquare(float a, float x, float y)
{
    sf::RectangleShape ss;
    ss.setPosition(x, y);
    ss.setSize(sf::Vector2f(a, a));
    ss.setFillColor(sf::Color::White);
    return ss;
}

int xyToInt(int x, int y)
{
    return mazeWidth * y + x;
}

std::pair<int, int> intToXy(int val)
{
    return std::make_pair(val % mazeWidth, int(floor(val / mazeWidth)));
}

int m_totalVisitedCells;
std::stack<std::pair<int, int>> m_stack;

int main(int argc, char* argv[])
{
    mazeWidth = std::stoi(argv[1]);
    mazeHeight = std::stoi(argv[2]);
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Maze Generation and Solver");
    window.setFramerateLimit(120);

    sf::RectangleShape mazeCells[mazeWidth][mazeHeight];
    std::vector<sf::RectangleShape> mazeWalls;
    std::pair<float, float> cell_coordinates[mazeWidth][mazeHeight];


    for (int i = 0; i < mazeWidth; i++) 
    {
        for (int j = 0; j < mazeHeight; j++)
        {
            cell_coordinates[i][j] = {i*12/scale, j*12/scale};
        }
    }

    for (int i = 0; i < mazeWidth; i++)
    {
        for (int j = 0; j < mazeHeight; j++)
        {
            sf::RectangleShape cell;
            cell.setPosition(cell_coordinates[i][j].first, cell_coordinates[i][j].second);
            cell.setFillColor(sf::Color::White);
            cell.setSize(sf::Vector2f(10.f/scale, 10.f/scale));
            mazeCells[i][j] = cell;
        }
    }

//wall-------------------------------------------------------------------------------------------------------    
    int m_visited[mazeWidth][mazeHeight];
    std::vector<std::pair<std::pair<float, float>, int>> walls;
    std::vector<std::vector<int>> m_connectedAdjList(mazeWidth * mazeHeight);
    for (int i = 0; i < mazeWidth; i++)
    {
        for (int j = 0; j < mazeHeight; j++)
        {
            m_visited[i][j] = 0;
        }
    }
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
            
            int currentCell = xyToInt(m_stack.top().first, m_stack.top().second);
            int neighbourCell = xyToInt(randomCellx, randomCelly);

            m_stack.push({randomCellx, randomCelly});
            m_visited[randomCellx][randomCelly] = 1;
            m_totalVisitedCells++;
            m_connectedAdjList[currentCell].push_back(neighbourCell);
            m_connectedAdjList[neighbourCell].push_back(currentCell);
            float cx = cell_coordinates[randomCellx][randomCelly].first;
            float cy = cell_coordinates[randomCellx][randomCelly].second;
            walls.push_back({std::make_pair(cx, cy), randomCell});
/*
            if (randomCell == 0) //right
            {
                sf::RectangleShape Fill;
                Fill.setPosition(cell_coordinates[m_stack.top().first][m_stack.top().second].first + 10, 
                                    cell_coordinates[m_stack.top().first][m_stack.top().second].second);
                Fill.setSize(sf::Vector2f(2.f, 10.f));
                Fill.setFillColor(sf::Color::White);
                mazeWalls.push_back(Fill);
            }
            if (randomCell == 1) //up
            {
                sf::RectangleShape Fill;
                Fill.setPosition(cell_coordinates[m_stack.top().first][m_stack.top().second].first, 
                                    cell_coordinates[m_stack.top().first][m_stack.top().second].second - 2);
                Fill.setSize(sf::Vector2f(10.f, 2.f));
                Fill.setFillColor(sf::Color::White);
                mazeWalls.push_back(Fill);
            }
            if (randomCell == 2) //left
            {
                sf::RectangleShape Fill;
                Fill.setPosition(cell_coordinates[m_stack.top().first][m_stack.top().second].first - 2, 
                                    cell_coordinates[m_stack.top().first][m_stack.top().second].second);
                Fill.setSize(sf::Vector2f(2.f, 10.f));
                Fill.setFillColor(sf::Color::White);
                mazeWalls.push_back(Fill);
            }
            if (randomCell == 3) //down
            {
                sf::RectangleShape Fill;
                Fill.setPosition(cell_coordinates[m_stack.top().first][m_stack.top().second].first, 
                                    cell_coordinates[m_stack.top().first][m_stack.top().second].second + 10);
                Fill.setSize(sf::Vector2f(10.f, 2.f));
                Fill.setFillColor(sf::Color::White);
                mazeWalls.push_back(Fill);
            }
*/
        }
        else
        {
            m_stack.pop();
        }

    }
/*Adjacency list print*/
/*     for (int i = 0; i < mazeWidth * mazeHeight; i++) 
    {
        std::cout << i << ": ";
        for (auto e : m_connectedAdjList[i])
        {
            std::cout << e << " ";
        }
        std::cout << std::endl;
    }
/*print shortest path
    
    if (!shortestPath.empty())
    {
        for (auto e: shortestPath)
        {
            std::cout << "(" << intToXy(e).first << ", " << intToXy(e).second << ") <- ";
        }
        std::cout << std::endl;
    }
 */

    BFS(m_connectedAdjList, xyToInt(std::stoi(argv[3]),std::stoi(argv[4])), xyToInt(std::stoi(argv[5]), std::stoi(argv[6])));
/**************************************render operations*****************************************/
    int nWalls = walls.size();
    int itr = 0;
    int path = 0;
    bool alreadyDisplayed = false;
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        
 
        /* for (int i = 0; i < mazeWidth; i++)
        {
            for (int j = 0; j < mazeHeight; j++)
            {
                window.draw(mazeCells[i][j]);
            }
        } */
        sf::RectangleShape initialPosMarker;
        initialPosMarker.setPosition(cell_coordinates[std::stoi(argv[3])][std::stoi(argv[4])].first, cell_coordinates[std::stoi(argv[3])][std::stoi(argv[4])].second);
        initialPosMarker.setSize(sf::Vector2f(10.f/scale, 10.f/scale));
        initialPosMarker.setFillColor(sf::Color::Green);
        window.draw(initialPosMarker);

        sf::RectangleShape destPosMarker;
        destPosMarker.setPosition(cell_coordinates[std::stoi(argv[5])][std::stoi(argv[6])].first, cell_coordinates[std::stoi(argv[5])][std::stoi(argv[6])].second);
        destPosMarker.setSize(sf::Vector2f(10.f/scale, 10.f/scale));
        destPosMarker.setFillColor(sf::Color::Red);
        window.draw(destPosMarker);



        if (itr < nWalls)
        {
            int randomCell = walls[itr].second;
            float wx = walls[itr].first.first;
            float wy = walls[itr].first.second;
            if (randomCell == 0) //right
            {
                sf::RectangleShape Fill;
                Fill.setPosition(wx - 2/scale, wy);
                Fill.setSize(sf::Vector2f(2.f/scale, 10.f/scale));
                Fill.setFillColor(sf::Color::White);
                window.draw(drawWhiteSquare(10.f/scale, wx, wy));
                window.draw(Fill);
                //std::cout << "right" << std::endl;
            }
            if (randomCell == 1) //up
            {
                sf::RectangleShape Fill;
                Fill.setPosition(wx, wy + 10/scale);
                Fill.setSize(sf::Vector2f(10.f/scale, 2.f/scale));
                Fill.setFillColor(sf::Color::White);
                window.draw(drawWhiteSquare(10.f/scale, wx, wy));
                window.draw(Fill);
                //std::cout << "up" << std::endl;
            }
            if (randomCell == 2) //left
            {
                sf::RectangleShape Fill;
                Fill.setPosition(wx + 10/scale, wy);
                Fill.setSize(sf::Vector2f(2.f/scale, 10.f/scale));
                Fill.setFillColor(sf::Color::White);
                window.draw(drawWhiteSquare(10.f/scale, wx, wy));
                window.draw(Fill);
                //std::cout << "left" << std::endl;
            }
            if (randomCell == 3) //down
            {
                sf::RectangleShape Fill;
                Fill.setPosition(wx, wy - 2/scale);
                Fill.setSize(sf::Vector2f(10.f/scale, 2.f/scale));
                Fill.setFillColor(sf::Color::White);
                window.draw(drawWhiteSquare(10.f/scale, wx, wy));
                window.draw(Fill);
                //std::cout << "down" << std::endl;
            }
            itr++;
        }
        /*for (auto &e : mazeWalls)
        {
            window.draw(e);
        }*/

        if (itr >= nWalls && path < shortestPath.size())
        {
            if (path == 0)
            {
                int fx;
                int fy;
                fx = intToXy(shortestPath[0]).first;
                fy = intToXy(shortestPath[0]).second;

                sf::RectangleShape Cell;
                Cell.setPosition(cell_coordinates[fx][fy].first, cell_coordinates[fx][fy].second);
                Cell.setSize(sf::Vector2f(10.f/scale, 10.f/scale));
                Cell.setFillColor(sf::Color::Red);

                window.draw(Cell);
            }
            else
            {
                int fx;
                int fy;
                fx = intToXy(shortestPath[path]).first;
                fy = intToXy(shortestPath[path]).second;

                sf::RectangleShape Cell;
                if (shortestPath[path] - 1 == shortestPath[path-1]) //right
                {
                    Cell.setPosition(cell_coordinates[fx][fy].first - 9/scale, cell_coordinates[fx][fy].second + 3/scale);
                    Cell.setSize(sf::Vector2f(16.f/scale, 4.f/scale));
                    Cell.setFillColor(sf::Color::Blue);
                }
                if (shortestPath[path] + 1 == shortestPath[path-1]) //left
                {
                    Cell.setPosition(cell_coordinates[fx][fy].first + 3/scale, cell_coordinates[fx][fy].second + 3/scale);
                    Cell.setSize(sf::Vector2f(16.f/scale, 4.f/scale));
                    Cell.setFillColor(sf::Color::Blue);
                }
                if (shortestPath[path] - shortestPath[path-1] < -1 ) //up
                {
                    Cell.setPosition(cell_coordinates[fx][fy].first + 3/scale, cell_coordinates[fx][fy].second + 3/scale);
                    Cell.setSize(sf::Vector2f(4.f/scale, 16.f/scale));
                    Cell.setFillColor(sf::Color::Blue);
                }
                if (shortestPath[path] - shortestPath[path-1] > 1 ) //down
                {
                    Cell.setPosition(cell_coordinates[fx][fy].first + 3/scale, cell_coordinates[fx][fy].second - 9/scale);
                    Cell.setSize(sf::Vector2f(4.f/scale, 16.f/scale));
                    Cell.setFillColor(sf::Color::Blue);
                }
                
                window.draw(Cell);
                if (path == shortestPath.size()-1)
                {
                    sf::RectangleShape Cell;
                    Cell.setPosition(cell_coordinates[fx][fy].first, cell_coordinates[fx][fy].second);
                    Cell.setSize(sf::Vector2f(10.f/scale, 10.f/scale));
                    Cell.setFillColor(sf::Color::Green);
                    window.draw(Cell);
                }
            }
            path++;
        }
 
        // Update the window
        window.display();
    }
    window.clear();



    return 0;
}
