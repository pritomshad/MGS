#include "Globals.hpp"
#include "Solver.hpp"
#include <vector>

bool BFS(std::vector< std::vector<int> > &adj, std::pair<float, float> (&cell_coordinates)[1000][1000], int src, int dest, int mazeWidth, int mazeHeight, std::vector<int> &shortestPath, std::list<int> &queue, int *prev, bool *bfs_visited, float scale, std::vector<sf::RectangleShape> &bfs_show)
{
    if (!queue.empty())
    {
        int u = queue.front();
        queue.pop_front();
        for (int i = 0; i < adj[u].size(); i++) {
            if (bfs_visited[adj[u][i]] == false) {

                /*render children to the screen*/


                int cx = intToXy(adj[u][i], mazeWidth).first;
                int cy = intToXy(adj[u][i], mazeWidth).second;

                sf::RectangleShape Fill;
                Fill.setPosition(cell_coordinates[cx][cy].first, cell_coordinates[cx][cy].second);
                Fill.setSize(sf::Vector2f(10.f*scale, 10.f*scale));
                Fill.setFillColor(sf::Color::Blue);
                bfs_show.push_back(Fill);
                


                /*end of render children*/

                bfs_visited[adj[u][i]] = true;
                prev[adj[u][i]] = u;
                queue.push_back(adj[u][i]);

                // We stop BFS when we find
                // destination.
                if (adj[u][i] == dest)
                {
                    //std::cout << "FOUND" << std::endl;
                    int crawl = dest;
                    shortestPath.push_back(crawl);
                    while (prev[crawl] != src) {
                        //std::cout << "crawl: " << crawl << std::endl;
                        shortestPath.push_back(prev[crawl]);
                        crawl = prev[crawl];
                    }
                    //std::cout << shortestPath.size() << std::endl;
                    return true;
                }
            }
        }
    }

    return false;

}
