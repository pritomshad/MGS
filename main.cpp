#include <iostream>
#include <stack>
#include <cmath>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

int mazeWidth = 80;
int mazeHeight = 50;

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

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(1300, 900), "SFML window");
    window.setFramerateLimit(80);

    sf::RectangleShape mazeCells[mazeWidth][mazeHeight];
    std::vector<sf::RectangleShape> mazeWalls;
    std::pair<float, float> cell_coordinates[mazeWidth][mazeHeight];


    for (int i = 0; i < mazeWidth; i++) 
    {
        for (int j = 0; j < mazeHeight; j++)
        {
            cell_coordinates[i][j] = {i*12, j*12};
        }
    }

    for (int i = 0; i < mazeWidth; i++)
    {
        for (int j = 0; j < mazeHeight; j++)
        {
            sf::RectangleShape cell;
            cell.setPosition(cell_coordinates[i][j].first, cell_coordinates[i][j].second);
            cell.setFillColor(sf::Color::White);
            cell.setSize(sf::Vector2f(10.f, 10.f));
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
    int nWalls = walls.size();
    int itr = 0;
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

        
 
/*        for (int i = 0; i < mazeWidth; i++)
        {
            for (int j = 0; j < mazeHeight; j++)
            {
                window.draw(mazeCells[i][j]);
            }
        }
*/


        if (itr < nWalls)
        {
            int randomCell = walls[itr].second;
            float wx = walls[itr].first.first;
            float wy = walls[itr].first.second;
            if (randomCell == 0) //right
            {
                sf::RectangleShape Fill;
                Fill.setPosition(wx - 2, wy);
                Fill.setSize(sf::Vector2f(2.f, 10.f));
                Fill.setFillColor(sf::Color::White);
                window.draw(drawWhiteSquare(10.f, wx, wy));
                window.draw(Fill);
                //std::cout << "right" << std::endl;
            }
            if (randomCell == 1) //up
            {
                sf::RectangleShape Fill;
                Fill.setPosition(wx, wy + 10);
                Fill.setSize(sf::Vector2f(10.f, 2.f));
                Fill.setFillColor(sf::Color::White);
                window.draw(drawWhiteSquare(10.f, wx, wy));
                window.draw(Fill);
                //std::cout << "up" << std::endl;
            }
            if (randomCell == 2) //left
            {
                sf::RectangleShape Fill;
                Fill.setPosition(wx + 10, wy);
                Fill.setSize(sf::Vector2f(2.f, 10.f));
                Fill.setFillColor(sf::Color::White);
                window.draw(drawWhiteSquare(10.f, wx, wy));
                window.draw(Fill);
                //std::cout << "left" << std::endl;
            }
            if (randomCell == 3) //down
            {
                sf::RectangleShape Fill;
                Fill.setPosition(wx, wy - 2);
                Fill.setSize(sf::Vector2f(10.f, 2.f));
                Fill.setFillColor(sf::Color::White);
                window.draw(drawWhiteSquare(10.f, wx, wy));
                window.draw(Fill);
                //std::cout << "down" << std::endl;
            }
            itr++;
        }
        /*for (auto &e : mazeWalls)
        {
            window.draw(e);
        }*/
 
        // Update the window
        window.display();
    }
    window.clear();



    return 0;
}
