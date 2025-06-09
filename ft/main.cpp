#include <SFML/Graphics.hpp>
#include <muParser.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

const int w = 800;
const int h = 600;
const int m = 5;
const float arrowS = 10.0f;

int main() 
{
    sf::RenderWindow okno(sf::VideoMode(w, h), "Graph");

    sf::Font f;
    if (!f.loadFromFile("arial.ttf"))
    {
        cerr << "Ошибка загрузки шрифта!\n";
    }

    string inputFunc;
    cout << "Введите функцию от x: ";
    getline(cin, inputFunc);

    double minX, maxX;
    cout << "Введите минимум X: ";
    cin >> minX;
    cout << "Введите максимум X: ";
    cin >> maxX;

    if (minX >= maxX)
    {
        cout << "minX должен быть меньше maxX\n";
        return 1;
    }

    double x = 0;
    mu::Parser parser;
    parser.DefineVar("x", &x);
    try {
        parser.SetExpr(inputFunc);
    }
    catch (...) {
        cout << "Ошибка при разборе функции\n";
        return 1;
    }

    double minY = 1e9, maxY = -1e9;
    vector<pair<double, double>> dots;

    for (x = minX; x <= maxX; x += 0.001)
    {
        double y;
        try {
            y = parser.Eval();
        }
        catch (...) {
            continue;
        }

        if (isnan(y) || isinf(y) || abs(y) > 1e6)
        {
            continue;
        }

        dots.push_back({x, y});
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }

    if (dots.empty()) 
    {
        cout << "нет допустимых значений\n";
        return 1;
    }

    double scaleX = (w - 2 * m) / (maxX - minX);
    double scaleY = (h - 2 * m) / (maxY - minY);

    vector<sf::Vertex> graph;
    for (auto& p : dots)
    {
        double sx = m + (p.first - minX) * scaleX;
        double sy = h - m - (p.second - minY) * scaleY;

        if (sy < 0 || sy > h)
            continue;

        graph.push_back(sf::Vertex(sf::Vector2f(sx, sy), sf::Color::Red));
    }

    sf::Text fLabel("f(x) = " + inputFunc, f, 16);
    fLabel.setPosition(10, 10);
    fLabel.setFillColor(sf::Color::Blue);

    sf::Text xLabel("X", f, 16);
    xLabel.setFillColor(sf::Color::Black);

    sf::Text yLabel("Y", f, 16);
    yLabel.setFillColor(sf::Color::Black);

    while (okno.isOpen())
    {
        sf::Event e;
        while (okno.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            okno.close();
        }

        okno.clear(sf::Color::White);

        double y0 = h - m - (0 - minY) * scaleY;
        if (y0 >= 0 && y0 <= h)
        {
            sf::Vertex xAxis[] = {
                sf::Vertex(sf::Vector2f(0, y0), sf::Color::Black),
                sf::Vertex(sf::Vector2f(w, y0), sf::Color::Black)
            };
            okno.draw(xAxis, 2, sf::Lines);

            sf::Vertex xArrow[] = {
                sf::Vertex(sf::Vector2f(w, y0), sf::Color::Black),
                sf::Vertex(sf::Vector2f(w - arrowS, y0 - arrowS / 2), sf::Color::Black),
                sf::Vertex(sf::Vector2f(w, y0), sf::Color::Black),
                sf::Vertex(sf::Vector2f(w - arrowS, y0 + arrowS / 2), sf::Color::Black)
            };
            okno.draw(xArrow, 4, sf::Lines);

            xLabel.setPosition(w - 25, y0 - 25);
            okno.draw(xLabel);
        }

        double x0 = m + (0 - minX) * scaleX;
        if (x0 >= 0 && x0 <= w)
        {
            sf::Vertex yAxis[] = {
                sf::Vertex(sf::Vector2f(x0, 0), sf::Color::Black),
                sf::Vertex(sf::Vector2f(x0, h), sf::Color::Black)
            };
            okno.draw(yAxis, 2, sf::Lines);

            sf::Vertex yArrow[] = {
                sf::Vertex(sf::Vector2f(x0, 0), sf::Color::Black),
                sf::Vertex(sf::Vector2f(x0 - arrowS / 2, arrowS), sf::Color::Black),
                sf::Vertex(sf::Vector2f(x0, 0), sf::Color::Black),
                sf::Vertex(sf::Vector2f(x0 + arrowS / 2, arrowS), sf::Color::Black)
            };
            okno.draw(yArrow, 4, sf::Lines);

            yLabel.setPosition(x0 + 10, 10);
            okno.draw(yLabel);
        }

        if (!graph.empty())
        {
            okno.draw(&graph[0], graph.size(), sf::LineStrip);
        }

        okno.draw(fLabel);
        okno.display();
    }

    return 0;
}
