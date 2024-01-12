#include "Globals.hpp"
#include "Generation.hpp"
#include "Solver.hpp"
#define RENDER_WIDTH 1
#define RENDER_HEIGHT 1
#define FRAME_RATE 24
int mazeWidth;
int mazeHeight;

float scale;
std::vector<int> shortestPath;
std::vector<int> shortestPath2;
int m_totalVisitedCells;
std::vector<std::pair<FLOAT_PAIR, int>> walls;
int speed = FRAME_RATE;

std::pair<float, float> cell_coordinates[1000][1000];

int main(int argc, char* argv[])
{
    mazeWidth = std::stoi(argv[1]);
    mazeHeight = std::stoi(argv[2]);
    scale = std::stof(argv[7]);
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(RENDER_WIDTH*12*mazeWidth*scale, RENDER_HEIGHT*12*mazeHeight*scale), "Maze Generation and Solver : A Song of Fire and Ice");
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
    std::vector<int> bfs_show;

    for (int i = 0; i < v; i++)
    {
        bfs_visited[i] = false;
        prev[i] = -1;
    }
    queue.push_back(src);

    bool bfs_visited2[v];
    std::list<int> queue2;
    int prev2[v];
    std::vector<int> bfs_show2;

    for (int i = 0; i < v; i++)
    {
        bfs_visited2[i] = false;
        prev2[i] = -1;
    }
    queue2.push_back(dest);
/*------------------------------------END---------------------------------------*/


    int nWalls = walls.size();
    int itr = 0;
    int path = 0;
    int debug = 0, debug1=0;
    bool alreadyDisplayed = false;
    bool path_drawn = false;

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            speed = speed+6;
            if (speed > 360)
                speed = 360;
            std::cout << speed << std::endl;
            window.setFramerateLimit(speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            speed = speed-6;
            if (speed < 3)
                speed = 3;
            std::cout << speed << std::endl;
            window.setFramerateLimit(speed);
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
        //while (itr < nWalls)
        if (itr < nWalls)
        {
            int randomCell = walls[itr].second;
            float wx = walls[itr].first.first;
            float wy = walls[itr].first.second;
            sf::RectangleShape Fill;
            window.draw(drawWhiteSquare(10.f*scale, wx, wy));
            if (randomCell == 0) //right
            {
                Fill.setPosition(wx - 2*scale, wy);
                Fill.setSize(sf::Vector2f(2.f*scale, 10.f*scale));
                Fill.setFillColor(sf::Color(234,235,204));
            }
            if (randomCell == 1) //up
            {
                Fill.setPosition(wx, wy + 10*scale);
                Fill.setSize(sf::Vector2f(10.f*scale, 2.f*scale));
                Fill.setFillColor(sf::Color(234,235,204));
            }
            if (randomCell == 2) //left
            {
                Fill.setPosition(wx + 10*scale, wy);
                Fill.setSize(sf::Vector2f(2.f*scale, 10.f*scale));
                Fill.setFillColor(sf::Color(234,235,204));
            }
            if (randomCell == 3) //down
            {
                Fill.setPosition(wx, wy - 2*scale);
                Fill.setSize(sf::Vector2f(10.f*scale, 2.f*scale));
                Fill.setFillColor(sf::Color(234,235,204));
            }
            window.draw(Fill);
            itr++;
        }
/*---------------------------------END OF GENERATION RENDER---------------------------------------*/

        if (itr >= nWalls && shortestPath.size() == 0 && !path_drawn)
        {
            //window.setFramerateLimit(120);
            BFS(m_connectedAdjList, cell_coordinates, src, dest, mazeWidth, mazeHeight, shortestPath, queue, prev, bfs_visited, scale, bfs_show);
            for (int i=0; i<bfs_show.size(); i++)
            {
                
                int cx = intToXy(bfs_show[i], mazeWidth).first;
                int cy = intToXy(bfs_show[i], mazeWidth).second;

                sf::RectangleShape Fill;
                Fill.setPosition(cell_coordinates[cx][cy].first, cell_coordinates[cx][cy].second);
                Fill.setSize(sf::Vector2f(10.f*scale, 10.f*scale));
                Fill.setFillColor(sf::Color::Blue);
                window.draw(Fill);
            }

            //std::cout << "BFS running: " << debug++ << " path:" << path << " ss:"<< shortestPath.size() << std::endl;
        }
        if (itr >= nWalls && shortestPath2.size() == 0 && !path_drawn)
        {
            //window.setFramerateLimit(120);
            BFS(m_connectedAdjList, cell_coordinates, dest, src, mazeWidth, mazeHeight, shortestPath2, queue2, prev2, bfs_visited2, scale, bfs_show2);
            for (int i=0; i<bfs_show2.size(); i++)
            {
                if (bfs_visited[bfs_show2[i]])
                {
                    int crawl = bfs_show2[i];
                    shortestPath.push_back(crawl);
                    while (prev[crawl] != src) {
                        //std::cout << "crawl: " << crawl << std::endl;
                        shortestPath.push_back(prev[crawl]);
                        crawl = prev[crawl];
                    }
                    crawl = bfs_show2[i];
                    while (prev2[crawl] != dest) {
                        //std::cout << "crawl: " << crawl << std::endl;
                        shortestPath.push_back(prev2[crawl]);
                        crawl = prev2[crawl];
                    }
                }

                int cx = intToXy(bfs_show2[i], mazeWidth).first;
                int cy = intToXy(bfs_show2[i], mazeWidth).second;

                sf::RectangleShape Fill;
                Fill.setPosition(cell_coordinates[cx][cy].first, cell_coordinates[cx][cy].second);
                Fill.setSize(sf::Vector2f(10.f*scale, 10.f*scale));
                Fill.setFillColor(sf::Color::Red);
                window.draw(Fill);
            }

            //std::cout << "BFS running: " << debug++ << " path:" << path << " ss:"<< shortestPath.size() << std::endl;
        }

/*---------------------------------SOLVE RENDER---------------------------------------*/
        if (itr >= nWalls)
        {
            if (shortestPath2.size() > 0)
                shortestPath = shortestPath2;
        }

        if (itr >= nWalls && path < shortestPath.size())
        {
            path_drawn = true;
            window.setFramerateLimit(60);
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
                    Cell.setFillColor(sf::Color(41, 82, 74));
                }
                if (shortestPath[path] + 1 == shortestPath[path-1]) //left
                {
                    Cell.setPosition(cell_coordinates[fx][fy].first + 3*scale, cell_coordinates[fx][fy].second + 3*scale);
                    Cell.setSize(sf::Vector2f(16.f*scale, 4.f*scale));
                    Cell.setFillColor(sf::Color(41, 82, 74));
                }
                if (shortestPath[path] - shortestPath[path-1] < -1 ) //up
                {
                    Cell.setPosition(cell_coordinates[fx][fy].first + 3*scale, cell_coordinates[fx][fy].second + 3*scale);
                    Cell.setSize(sf::Vector2f(4.f*scale, 16.f*scale));
                    Cell.setFillColor(sf::Color(41, 82, 74));
                }
                if (shortestPath[path] - shortestPath[path-1] > 1 ) //down
                {
                    Cell.setPosition(cell_coordinates[fx][fy].first + 3*scale, cell_coordinates[fx][fy].second - 9*scale);
                    Cell.setSize(sf::Vector2f(4.f*scale, 16.f*scale));
                    Cell.setFillColor(sf::Color(41, 82, 74));
                }
                
                window.draw(Cell);
                
            }
            path++;
        }
/*---------------------------------END OF SOLVE RENDER---------------------------------------*/

        window.display(); // Update the window
        for (int i=0; i<bfs_show.size(); i++)
        {
            //window.setFramerateLimit(120);
            int cx = intToXy(bfs_show[i], mazeWidth).first;
            int cy = intToXy(bfs_show[i], mazeWidth).second;

            sf::RectangleShape Fill;
            Fill.setPosition(cell_coordinates[cx][cy].first, cell_coordinates[cx][cy].second);
            Fill.setSize(sf::Vector2f(10.f*scale, 10.f*scale));
            Fill.setFillColor(sf::Color(182, 207, 241));
            window.draw(Fill);
        }
        bfs_show = std::vector<int> ();
        for (int i=0; i<bfs_show2.size(); i++)
        {
            //window.setFramerateLimit(120);
            int cx = intToXy(bfs_show2[i], mazeWidth).first;
            int cy = intToXy(bfs_show2[i], mazeWidth).second;

            sf::RectangleShape Fill;
            Fill.setPosition(cell_coordinates[cx][cy].first, cell_coordinates[cx][cy].second);
            Fill.setSize(sf::Vector2f(10.f*scale, 10.f*scale));
            Fill.setFillColor(sf::Color(229, 192, 230));
            window.draw(Fill);
        }
        bfs_show2 = std::vector<int> ();

    }
    window.clear();



    return 0;
}
