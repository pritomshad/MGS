#pragma once

extern std::pair<float, float> cell_coordinates[1000][1000];
bool BFS(std::vector< std::vector<int> > &adj, std::pair<float, float> (&cell_coordinates)[1000][1000], int src, int dest, int mazeWidth, int mazeHeight, std::vector<int> &shortestPath, std::list<int> &queue, int *prev, bool *bfs_visited, float scale, std::vector<sf::RectangleShape> &bfs_show);