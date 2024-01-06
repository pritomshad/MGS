#pragma once

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
#include <cstdint>
#define FLOAT_PAIR std::pair<float, float>

int xyToInt(int, int, int mazeWidth);
std::pair<int, int> intToXy(int, int mazeWidth);
sf::RectangleShape drawWhiteSquare(float size, float corr_x, float corr_y);