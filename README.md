# 📈 Function Graph Plotter

> 🇷🇺 [Читать на русском](README.ru.md)

A desktop application for plotting mathematical functions. The user enters a function of `x`, specifies the range, and the program renders an interactive graph in a window with coordinate axes.

Built with **C++**, **SFML** (graphics) and **muParser** (math expression parsing).

---

## Features

- Enter any mathematical function of `x` at runtime (e.g. `sin(x)`, `x^2`, `x*cos(x)`)
- Set custom X range
- Auto-scaling: Y range is computed automatically from the function values
- Coordinate axes with arrows drawn when they fall within the visible area
- Function label displayed in the top-left corner
- Handles undefined values gracefully (NaN, infinity, division by zero)

---

## Dependencies

| Library | Purpose |
|---|---|
| [SFML 2.x](https://www.sfml-dev.org/) | Window, rendering, graphics |
| [muParser](https://beltoforion.de/en/muparser/) | Math expression parser |

Install on Ubuntu:
```bash
sudo apt install libsfml-dev libmuparser-dev
```

---

## Build & Run

```bash
# Compile
g++ main.cpp -o graph -lsfml-graphics -lsfml-window -lsfml-system -lmuparser

# Run (arial.ttf must be in the same directory)
./graph
```

Then follow the prompts in the terminal:
```
Введите функцию от x: sin(x)
Введите минимум X: -6.28
Введите максимум X: 6.28
```

---

## Project Structure

```
├── main.cpp       # Main source file
├── arial.ttf      # Font file for graph labels
├── README.md      # This file (English)
└── README.ru.md   # Russian version
```

---

## Supported Syntax

muParser supports standard math expressions:

| Example | Description |
|---|---|
| `sin(x)`, `cos(x)`, `tan(x)` | Trigonometric functions |
| `x^2`, `x^3` | Powers |
| `sqrt(x)` | Square root |
| `exp(x)`, `log(x)` | Exponential and logarithm |
| `x*sin(x) + 2` | Combined expressions |

---

*Academic project (coursework) - RTU MIREA.*
