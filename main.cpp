#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <random>
#include <time.h>
#include <iostream>

size_t const WIDTH = 100, HEIGHT = 100;

int getNumberOfLivingCellAround(bool map_activated[][WIDTH], int x, int y, int max_x, int max_y)
{
    int count = 0;

    if (x - 1 >= 0) {

        if (y - 1 >= 0) {
            map_activated[y - 1][x - 1] == true ? count++ : false;
        }

        map_activated[y][x - 1] == true ? count++ : false;
        if (y + 1 <= max_y) {

            map_activated[y + 1][x - 1] == true ? count++ : false;
        }
    }
    if (x + 1 <= max_x) {
        if (y - 1 >= 0) {
            map_activated[y - 1][x + 1] == true ? count++ : false;
        }

        map_activated[y][x + 1] == true ? count++ : false;

        if (y + 1 <= max_y) {
            map_activated[y + 1][x + 1] == true ? count++ : false;
        }
    }
    if (y - 1 >= 0) {
        map_activated[y - 1][x] == true ? count++ : false;
    }
    if (y + 1 <= max_y) {
        map_activated[y + 1][x] == true ? count++ : false;
    }

    return (count);
}



int main(void)
{
    srand(time(NULL));

    sf::Event event;
    sf::RenderWindow window;
    sf::Clock clock;

    unsigned int W_WIDTH = 800, W_HEIGHT = 800 - 1;

    window.create((sf::VideoMode){W_WIDTH, W_HEIGHT, 32}, "Game of Life", sf::Style::Default);
    window.setFramerateLimit(60);

    bool grid_activated[HEIGHT][WIDTH];
    sf::RectangleShape map[HEIGHT][WIDTH];

    float cell_width = (W_WIDTH - 1) / WIDTH;
    float cell_height = (W_HEIGHT - 1) / HEIGHT;

    sf::Vector2f cell_size = {cell_width, cell_height};

    sf::Color grid_separator = sf::Color(230, 230, 230, 255);

    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            grid_activated[y][x] = false;
        }
    }

    float cell_pos_y = 0;
    for (size_t y = 0; y < HEIGHT; y++) {
        float cell_pos_x = 0;
        for (size_t x = 0; x < WIDTH; x++) {
            grid_activated[y][x] = (std::rand() % (100 - 0 + 1) <= 10 ? true : false);

            sf::Vector2f cell_pos = {cell_pos_x, cell_pos_y};
            if (grid_activated[y][x] == true)
                map[y][x].setFillColor(sf::Color::Black);
            else
                map[y][x].setFillColor(sf::Color::White);

            map[y][x].setPosition(cell_pos);
            map[y][x].setSize(cell_size);
            map[y][x].setOutlineThickness(0.5);
            map[y][x].setOutlineColor(grid_separator);
            window.draw(map[y][x]);

            cell_pos_x += cell_width + 1;
        }
        cell_pos_y += cell_height + 1;
    }

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (clock.getElapsedTime().asMilliseconds() >= 15) {
            bool new_grid[HEIGHT][WIDTH] = {false};

            for (size_t y = 0; y < HEIGHT; y++) {
                for (size_t x = 0; x < WIDTH; x++) {
                    int number = getNumberOfLivingCellAround(grid_activated, x, y, WIDTH, HEIGHT);
                    if (grid_activated[y][x] == true) {
                        if (number == 2 || number == 3) {
                            new_grid[y][x] = true;
                        }
                        else
                            new_grid[y][x] = false;
                    }
                    else if (grid_activated[y][x] == false) {
                        if (number == 3) {
                            new_grid[y][x] = true;
                        }
                    }

                    if (new_grid[y][x] == true) {
                        map[y][x].setFillColor(sf::Color::Black);
                    }
                    else if (new_grid[y][x] == false)
                        map[y][x].setFillColor(sf::Color::White);
                }
            }

            window.clear(sf::Color::Black);

            for (size_t y = 0; y < HEIGHT; y++) {
                for (size_t x = 0; x < WIDTH; x++) {
                    window.draw(map[y][x]);
                }
            }

            window.display();

            for (size_t y = 0; y < HEIGHT; y++) {
                for (size_t x = 0; x < WIDTH; x++) {
                    grid_activated[y][x] = new_grid[y][x];
                }
            }

            clock.restart();
        }
    }

    return (0);
}