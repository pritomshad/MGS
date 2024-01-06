#include "Globals.hpp"
#include "Solver.hpp"
#include <vector>

bool BFS(std::vector< std::vector<int> > &adj, int src, int dest, int mazeWidth, int mazeHeight, std::vector<int> &shortestPath)
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
