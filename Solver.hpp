#pragma once

bool BFS(std::vector< std::vector<int> > &adj, int src, int dest, int mazeWidth, int mazeHeight, std::vector<int> &shortestPath, std::list<int> &queue, int *prev, bool *bfs_visited, std::vector<int> &bfs_show);