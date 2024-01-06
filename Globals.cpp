#include "Globals.hpp"

int xyToInt(int x, int y, int mazeWidth)
{
    return mazeWidth * y + x;
}

std::pair<int, int> intToXy(int val, int mazeWidth)
{
    return std::make_pair(val % mazeWidth, int(floor(val / mazeWidth)));
}

sf::RectangleShape drawWhiteSquare(float a, float x, float y)
{
    sf::RectangleShape ss;
    ss.setPosition(x, y);
    ss.setSize(sf::Vector2f(a, a));
    ss.setFillColor(sf::Color::White);
    return ss;
}