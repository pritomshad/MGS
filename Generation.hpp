#pragma once
#include <vector>
#include <stack>

extern std::pair<float, float> cell_coordinates[1000][1000];
void wall(int mazeWidth, int mazeHeight, int m_totalVisitedCells, std::vector<std::vector<int>> &m_connectedAdjList, std::pair<float, float> (&cell_coordinates)[1000][1000], std::vector<std::pair<FLOAT_PAIR, int>> &walls);
