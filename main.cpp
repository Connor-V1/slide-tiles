#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Random.hpp"

const float WINDOW_WIDTH = 600.0f;
const float WINDOW_HEIGHT = 600.0f;

const sf::Color BACKGROUND_COLOR = sf::Color(205, 192, 180);
const sf::Color OUTLINE_COLOR = sf::Color(187, 173, 160);

const int OUTLINE_THICKNESS = 10;

const int SIDE = 4;
const int ROWS = SIDE;
const int COLS = SIDE;

const float RECT_WIDTH = WINDOW_WIDTH / COLS;
const float RECT_HEIGHT = WINDOW_HEIGHT / ROWS;

Random random;

class Tile {
public:
    sf::Color colors[9] = {
        sf::Color(237, 229, 218),
        sf::Color(238, 225, 201),
        sf::Color(243, 178, 122),
        sf::Color(246, 150, 101),
        sf::Color(247, 124, 95),
        sf::Color(247, 95, 59),
        sf::Color(237, 208, 115),
        sf::Color(237, 204, 99),
        sf::Color(236, 202, 80),
    };

    sf::RectangleShape rect;
    sf::Font font;
    sf::Text text;

    std::string value;
    int row, col;
    float x, y;

    Tile(std::string v, int r, int c) 
        : value(v), row(r), col(c) {

        if(!font.loadFromFile("./fonts/Comic Sans MS.ttf")) {
            std::cerr << "Error loading font file!" << std::endl;
            return;
        }

        rect.setSize(sf::Vector2f(RECT_WIDTH, RECT_HEIGHT));
        rect.setFillColor(colors[random.randint(0, 9)]);
    }

    bool mouseOver(const sf::Vector2f& mousePos) {
        if(mousePos.x >= x + OUTLINE_THICKNESS && mousePos.x <= x + RECT_WIDTH) {
            if(mousePos.y >= y + OUTLINE_THICKNESS && mousePos.y <= y + RECT_HEIGHT) {
                return true;
            }
        }
        return false;
    }

    void draw(sf::RenderWindow& surface) {
        x = RECT_WIDTH * col; y = RECT_HEIGHT * row;
        rect.setPosition(sf::Vector2f(x, y));

        text.setFont(font);
        text.setString(value);
        text.setPosition(sf::Vector2f(x + RECT_WIDTH/2 - text.getLocalBounds().width/2, y + RECT_HEIGHT/2 - text.getLocalBounds().height/2 - text.getLocalBounds().top));
        text.setCharacterSize(std::min(RECT_WIDTH/(int)std::to_string(ROWS*COLS).size(), RECT_HEIGHT/2)); 

        surface.draw(rect);
        surface.draw(text);
    }
};

void drawLine(sf::RenderWindow& surface, const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& color=sf::Color::White) {
    sf::Vertex lines[2] = {
        sf::Vertex(start),
        sf::Vertex(end)
    };
    lines[0].color = lines[1].color = color;
    surface.draw(lines, 2, sf::Lines);
}

void swapTile(Tile& lhs, Tile& rhs) {
    int row = lhs.row, col = lhs.col;
    lhs.row = rhs.row; lhs.col = rhs.col;
    rhs.row = row; rhs.col = col;
}

bool validMove(const Tile& clicked, const Tile& empty) {
    int row = empty.row, col = empty.col;
    const int totalMoves = 4;
    sf::Vector2i moves[totalMoves] = { {row, col-1}, {row, col+1}, {row-1, col}, {row+1, col} };

    for(int i = 0; i < totalMoves; i++) {
        if(clicked.row == moves[i].x && clicked.col == moves[i].y) {
            return true;
        }
    }
    return false;
}

void naiveShuffle(std::vector<int>& vec, int start, int end) {
    for(int i = 0; i < 100; i++) {
        int a = random.randint(start, end);
        int b = random.randint(start, end);
        while(a == b) {
            a = random.randint(start, end);
            b = random.randint(start, end);
        }
        int temp = vec[a];
        vec[a] = vec[b];
        vec[b] = temp;
    }
}

std::vector<Tile> generateTileSet(int rows, int cols) {
    std::vector<int> numbers;
    for(int i = 0; i < ROWS*COLS; i++) {
        numbers.push_back(i);
    }

    naiveShuffle(numbers, 1, numbers.size());

    std::vector<Tile> tiles;
    int value = 0;
    for(int y = 0; y < rows; y++) {
        for(int x = 0; x < cols; x++) {
            tiles.push_back(Tile(std::to_string(numbers[value]), y, x));
            value++;
        }
    }
    return tiles;
}


int main() {

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "", sf::Style::Close | sf::Style::Titlebar);


    std::vector<Tile> tiles = generateTileSet(ROWS, COLS);

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::MouseButtonPressed:
                    if(event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
                        for(unsigned int i = 1; i < tiles.size(); i++) {
                            if(tiles[i].mouseOver(mousePos) && validMove(tiles[i], tiles[0])) {
                                swapTile(tiles[i], tiles[0]);
                            }
                        }
                    }
                    break;

                default:
                    break;
            }
        }


        ////
        window.clear(BACKGROUND_COLOR);

        for(unsigned int i = 1; i < tiles.size(); i++) {
            tiles[i].draw(window);
        }


        // top
        for(int y = 0; y < OUTLINE_THICKNESS; y++) {
            drawLine(window, sf::Vector2f(0, y), sf::Vector2f(WINDOW_WIDTH, y), OUTLINE_COLOR);
        }

        // bottom
        for(int y = WINDOW_HEIGHT - OUTLINE_THICKNESS; y < WINDOW_HEIGHT; y++) {
            drawLine(window, sf::Vector2f(0, y), sf::Vector2f(WINDOW_WIDTH, y), OUTLINE_COLOR);
        }

        // left
        for(int x = 0; x < OUTLINE_THICKNESS; x++) {
            drawLine(window, sf::Vector2f(x, 0), sf::Vector2f(x, WINDOW_HEIGHT), OUTLINE_COLOR);
        }

        // right
        for(int x = WINDOW_WIDTH - OUTLINE_THICKNESS; x < WINDOW_WIDTH; x++) {
            drawLine(window, sf::Vector2f(x, 0), sf::Vector2f(x, WINDOW_HEIGHT), OUTLINE_COLOR);
        }

        for(int y = 1; y < ROWS; y++) {
            for(int t = 0; t < OUTLINE_THICKNESS; t++) {
                drawLine(window, sf::Vector2f(0, y * RECT_HEIGHT + t), sf::Vector2f(WINDOW_WIDTH, y * RECT_HEIGHT+ t), OUTLINE_COLOR);
            }
        }

        for(int x = 1; x < COLS; x++) {
            for(int t = 0; t < OUTLINE_THICKNESS; t++) {
                drawLine(window, sf::Vector2f(x * RECT_WIDTH + t, 0), sf::Vector2f(x * RECT_WIDTH + t, WINDOW_HEIGHT), OUTLINE_COLOR);
            }
        }

        window.display();
    }

    return 0;
}