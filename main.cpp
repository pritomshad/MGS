#include "Globals.hpp"
#include "Generation.hpp"
#include "Solver.hpp"
#define RENDER_WIDTH 1
#define RENDER_HEIGHT 1
#define FRAME_RATE 1
int mazeWidth;
int mazeHeight;

float scale = .5;
std::vector<int> shortestPath;
int m_totalVisitedCells;
std::vector<std::pair<FLOAT_PAIR, int>> walls;

std::pair<float, float> cell_coordinates[1000][1000];

int main(int argc, char* argv[])
{
    mazeWidth = std::stoi(argv[1]);
    mazeHeight = std::stoi(argv[2]);
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(RENDER_WIDTH*12*mazeWidth*scale, RENDER_HEIGHT*12*mazeHeight*scale), "Maze Generation and Solver");
    window.setFramerateLimit(FRAME_RATE);

    for (int i = 0; i < mazeWidth; i++) 
    {
        for (int j = 0; j < mazeHeight; j++)
        {
            cell_coordinates[i][j] = {i*12*scale, j*12*scale};
        }
    }


/*--------------------------------GENERATION------------------------------------*/
    int totalCells = mazeWidth * mazeHeight;
    std::vector<std::vector<int>> m_connectedAdjList(totalCells, std::vector<int>());
    wall(mazeWidth, mazeHeight, m_totalVisitedCells, m_connectedAdjList, cell_coordinates, walls);
/*-----------------------------------END---------------------------------------*/


    int src = xyToInt(std::stoi(argv[3]),std::stoi(argv[4]),mazeWidth);
    int dest = xyToInt(std::stoi(argv[5]), std::stoi(argv[6]),mazeWidth);

/*-----------------------------------SOLVE--------------------------------------*/
    //BFS(m_connectedAdjList, xyToInt(std::stoi(argv[3]),std::stoi(argv[4]),mazeWidth), xyToInt(std::stoi(argv[5]), std::stoi(argv[6]),mazeWidth), mazeWidth, mazeHeight, shortestPath);
    int v = mazeWidth * mazeHeight;
    bool bfs_visited[v];
    std::list<int> queue;
    int prev[v];
    std::vector<sf::RectangleShape> bfs_show;

    for (int i = 0; i < v; i++)
    {
        bfs_visited[i] = false;
        prev[i] = -1;
    }
    queue.push_back(src);
/*------------------------------------END---------------------------------------*/


    int nWalls = walls.size();
    int itr = 0;
    int path = 0;
    int debug = 0, debug1=0;
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

        
        //-------------------------MARK INITIAL POSITION TO GREEN----------------------
        sf::RectangleShape initialPosMarker;
        initialPosMarker.setPosition(cell_coordinates[std::stoi(argv[3])][std::stoi(argv[4])].first, cell_coordinates[std::stoi(argv[3])][std::stoi(argv[4])].second);
        initialPosMarker.setSize(sf::Vector2f(10.f*scale, 10.f*scale));
        initialPosMarker.setFillColor(sf::Color::Green);
        window.draw(initialPosMarker);


        //-------------------------MARK FINAL POSITION TO RED----------------------
        sf::RectangleShape destPosMarker;
        destPosMarker.setPosition(cell_coordinates[std::stoi(argv[5])][std::stoi(argv[6])].first, cell_coordinates[std::stoi(argv[5])][std::stoi(argv[6])].second);
        destPosMarker.setSize(sf::Vector2f(10.f*scale, 10.f*scale));
        destPosMarker.setFillColor(sf::Color::Red);
        window.draw(destPosMarker);


/*---------------------------------GENERATION RENDER---------------------------------------*/
        while (itr < nWalls)
        //if (itr < nWalls)
        {
            int randomCell = walls[itr].second;
            float wx = walls[itr].first.first;
            float wy = walls[itr].first.second;
            if (randomCell == 0) //right
            {
                sf::RectangleShape Fill;
                Fill.setPosition(wx - 2*scale, wy);
                Fill.setSize(sf::Vector2f(2.f*scale, 10.f*scale));
                Fill.setFillColor(sf::Color::White);
                window.draw(drawWhiteSquare(10.f*scale, wx, wy));
                window.draw(Fill);
            }
            if (randomCell == 1) //up
            {
                sf::RectangleShape Fill;
                Fill.setPosition(wx, wy + 10*scale);
                Fill.setSize(sf::Vector2f(10.f*scale, 2.f*scale));
                Fill.setFillColor(sf::Color::White);
                window.draw(drawWhiteSquare(10.f*scale, wx, wy));
                window.draw(Fill);
            }
            if (randomCell == 2) //left
            {
                sf::RectangleShape Fill;
                Fill.setPosition(wx + 10*scale, wy);
                Fill.setSize(sf::Vector2f(2.f*scale, 10.f*scale));
                Fill.setFillColor(sf::Color::White);
                window.draw(drawWhiteSquare(10.f*scale, wx, wy));
                window.draw(Fill);
            }
            if (randomCell == 3) //down
            {
                sf::RectangleShape Fill;
                Fill.setPosition(wx, wy - 2*scale);
                Fill.setSize(sf::Vector2f(10.f*scale, 2.f*scale));
                Fill.setFillColor(sf::Color::White);
                window.draw(drawWhiteSquare(10.f*scale, wx, wy));
                window.draw(Fill);
            }
            itr++;
        }
/*---------------------------------END OF GENERATION RENDER---------------------------------------*/

        if (itr >= nWalls && shortestPath.size() == 0)
        {
            window.setFramerateLimit(10);
            BFS(m_connectedAdjList, cell_coordinates, src, dest, mazeWidth, mazeHeight, shortestPath, queue, prev, bfs_visited, scale, bfs_show);
            for (int i=0; i<bfs_show.size(); i++)
            {
                window.draw(bfs_show[i]);
            }

            //std::cout << "BFS running: " << debug++ << " path:" << path << " ss:"<< shortestPath.size() << std::endl;
        }

/*---------------------------------SOLVE RENDER---------------------------------------*/
        if (itr >= nWalls && path < shortestPath.size())
        {
            window.setFramerateLimit(10);
            //std::cout << "FOUND PATH running " << debug1++ << std::endl;
            if (path == 0)
            {
                int fx;
                int fy;
                fx = intToXy(shortestPath[0], mazeWidth).first;
                fy = intToXy(shortestPath[0], mazeWidth).second;

                sf::RectangleShape Cell;
                Cell.setPosition(cell_coordinates[fx][fy].first, cell_coordinates[fx][fy].second);
                Cell.setSize(sf::Vector2f(10.f*scale, 10.f*scale));
                Cell.setFillColor(sf::Color::Red);

                window.draw(Cell);
            }
            else
            {
                int fx;
                int fy;
                fx = intToXy(shortestPath[path], mazeWidth).first;
                fy = intToXy(shortestPath[path], mazeWidth).second;

                sf::RectangleShape Cell;
                if (shortestPath[path] - 1 == shortestPath[path-1]) //right
                {
                    Cell.setPosition(cell_coordinates[fx][fy].first - 9*scale, cell_coordinates[fx][fy].second + 3*scale);
                    Cell.setSize(sf::Vector2f(16.f*scale, 4.f*scale));
                    Cell.setFillColor(sf::Color::Red);
                }
                if (shortestPath[path] + 1 == shortestPath[path-1]) //left
                {
                    Cell.setPosition(cell_coordinates[fx][fy].first + 3*scale, cell_coordinates[fx][fy].second + 3*scale);
                    Cell.setSize(sf::Vector2f(16.f*scale, 4.f*scale));
                    Cell.setFillColor(sf::Color::Red);
                }
                if (shortestPath[path] - shortestPath[path-1] < -1 ) //up
                {
                    Cell.setPosition(cell_coordinates[fx][fy].first + 3*scale, cell_coordinates[fx][fy].second + 3*scale);
                    Cell.setSize(sf::Vector2f(4.f*scale, 16.f*scale));
                    Cell.setFillColor(sf::Color::Red);
                }
                if (shortestPath[path] - shortestPath[path-1] > 1 ) //down
                {
                    Cell.setPosition(cell_coordinates[fx][fy].first + 3*scale, cell_coordinates[fx][fy].second - 9*scale);
                    Cell.setSize(sf::Vector2f(4.f*scale, 16.f*scale));
                    Cell.setFillColor(sf::Color::Red);
                }
                
                window.draw(Cell);
                
            }
            path++;
        }
/*---------------------------------END OF SOLVE RENDER---------------------------------------*/

        window.display(); // Update the window
        for (int i=0; i<bfs_show.size(); i++)
            {
                window.setFramerateLimit(400);
                bfs_show[i].setFillColor(sf::Color(150,150,255));
                window.draw(bfs_show[i]);
            }
            bfs_show = std::vector<sf::RectangleShape> ();

    }
    window.clear();



    return 0;
}
