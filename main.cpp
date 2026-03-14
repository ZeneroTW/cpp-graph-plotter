#include <SFML/Graphics.hpp>  // Библиотека SFML для графики и окна
#include <muParser.h>           // Библиотека muParser для разбора математических выражений
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

// Размеры окна и отступы
const int w = 800;        // Ширина окна в пикселях
const int h = 600;        // Высота окна в пикселях
const int m = 5;          // Отступ от краёв окна
const float arrowS = 10.0f; // Размер стрелок на осях координат

int main() 
{
    // Создание окна приложения 800x600 с заголовком "Graph"
    sf::RenderWindow okno(sf::VideoMode(w, h), "Graph");

    // Загрузка шрифта для отображения подписей на графике
    sf::Font f;
    if (!f.loadFromFile("arial.ttf"))
    {
        cerr << "Ошибка загрузки шрифта!\n";
    }

    // Ввод математической функции от пользователя
    string inputFunc;
    cout << "Введите функцию от x: ";
    getline(cin, inputFunc);

    // Ввод диапазона значений по оси X
    double minX, maxX;
    cout << "Введите минимум X: ";
    cin >> minX;
    cout << "Введите максимум X: ";
    cin >> maxX;

    // Проверка корректности диапазона
    if (minX >= maxX)
    {
        cout << "minX должен быть меньше maxX\n";
        return 1;
    }

    // Настройка парсера: связываем переменную x с нашей переменной
    double x = 0;
    mu::Parser parser;
    parser.DefineVar("x", &x);
    try {
        parser.SetExpr(inputFunc); // Передаём введённую функцию парсеру
    }
    catch (...) {
        cout << "Ошибка при разборе функции\n";
        return 1;
    }

    // Вычисление значений функции и поиск минимума/максимума по Y
    double minY = 1e9, maxY = -1e9;
    vector<pair<double, double>> dots; // Вектор точек (x, y)

    for (x = minX; x <= maxX; x += 0.001) // Шаг 0.001 для плавного графика
    {
        double y;
        try {
            y = parser.Eval(); // Вычисляем значение функции в точке x
        }
        catch (...) {
            continue; // Пропускаем точки где функция не определена
        }

        // Пропускаем NaN, бесконечности и слишком большие значения
        if (isnan(y) || isinf(y) || abs(y) > 1e6)
        {
            continue;
        }

        dots.push_back({x, y});
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }

    // Если нет ни одной допустимой точки — выходим
    if (dots.empty()) 
    {
        cout << "нет допустимых значений\n";
        return 1;
    }

    // Вычисляем масштаб: сколько пикселей приходится на единицу по X и Y
    double scaleX = (w - 2 * m) / (maxX - minX);
    double scaleY = (h - 2 * m) / (maxY - minY);

    // Переводим математические координаты в экранные пиксели
    vector<sf::Vertex> graph;
    for (auto& p : dots)
    {
        double sx = m + (p.first - minX) * scaleX;          // Экранная X
        double sy = h - m - (p.second - minY) * scaleY;     // Экранная Y (ось Y перевёрнута)

        if (sy < 0 || sy > h)
            continue;

        graph.push_back(sf::Vertex(sf::Vector2f(sx, sy), sf::Color::Red));
    }

    // Подпись функции в левом верхнем углу окна
    sf::Text fLabel("f(x) = " + inputFunc, f, 16);
    fLabel.setPosition(10, 10);
    fLabel.setFillColor(sf::Color::Blue);

    // Подписи осей координат
    sf::Text xLabel("X", f, 16);
    xLabel.setFillColor(sf::Color::Black);

    sf::Text yLabel("Y", f, 16);
    yLabel.setFillColor(sf::Color::Black);

    // Главный цикл отрисовки — выполняется пока окно открыто
    while (okno.isOpen())
    {
        // Обработка событий окна (закрытие)
        sf::Event e;
        while (okno.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            okno.close();
        }

        okno.clear(sf::Color::White); // Очищаем экран белым цветом

        // Отрисовка оси X (горизонтальная) если y=0 попадает в видимую область
        double y0 = h - m - (0 - minY) * scaleY;
        if (y0 >= 0 && y0 <= h)
        {
            sf::Vertex xAxis[] = {
                sf::Vertex(sf::Vector2f(0, y0), sf::Color::Black),
                sf::Vertex(sf::Vector2f(w, y0), sf::Color::Black)
            };
            okno.draw(xAxis, 2, sf::Lines);

            // Стрелка на конце оси X
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

        // Отрисовка оси Y (вертикальная) если x=0 попадает в видимую область
        double x0 = m + (0 - minX) * scaleX;
        if (x0 >= 0 && x0 <= w)
        {
            sf::Vertex yAxis[] = {
                sf::Vertex(sf::Vector2f(x0, 0), sf::Color::Black),
                sf::Vertex(sf::Vector2f(x0, h), sf::Color::Black)
            };
            okno.draw(yAxis, 2, sf::Lines);

            // Стрелка на конце оси Y
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

        // Отрисовка графика функции
        if (!graph.empty())
        {
            okno.draw(&graph[0], graph.size(), sf::LineStrip);
        }

        okno.draw(fLabel); // Подпись функции
        okno.display();    // Вывод кадра на экран
    }

    return 0;
}
