#include <SFML/Graphics.hpp>
#include <iostream>
#include "node.h"
#include "tree.h"

#define WIDTH 1920
#define HEIGHT 1080

int main()
{
    RBTree tree;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Red-black tree");
    sf::Font font;
    font.loadFromFile("Montserrat-Regular.ttf");
    sf::Text log("", font, 20);
    log.setFillColor(sf::Color::Black);
    log.setPosition(20, 0);

    sf::Text number("", font, 20);
    number.setFillColor(sf::Color::Red);
    number.setPosition(WIDTH / 2 - 50, 0);

    std::string str = "Number: ";
    std::string number_str = "";
    number.setString(str);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    char c = static_cast<char>(event.text.unicode);
                    if ((c >= '0' && c <= '9') || (c == '-' && number_str.size() == 0)) {
                        str += c;
                        number_str += c;
                    }

                    number.setString(str);
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) 
                    if (log.getCharacterSize() > 5)
                        log.setCharacterSize(log.getCharacterSize() - 1);

                if (event.key.code == sf::Keyboard::Right)
                    log.setCharacterSize(log.getCharacterSize() + 1);

                if (event.key.code == sf::Keyboard::Up)
                    log.move(sf::Vector2f(0, -7));

                if (event.key.code == sf::Keyboard::Down)
                    log.move(sf::Vector2f(0, 7));

                if (event.key.code == sf::Keyboard::Subtract)
                    if (r > 10)
                        r--;

                if (event.key.code == sf::Keyboard::Add)
                    r++;
                
                if (event.key.code == sf::Keyboard::C) {
                    str = "Number: ";
                    number_str = "";
                    number.setString(str);
                }
                if (event.key.code == sf::Keyboard::BackSpace) {
                    if (str.length() > 8) {
                        str.pop_back();
                        number_str.pop_back();
                    }
                    number.setString(str);
                }

                if (number_str == "" || number_str == "-") continue;

                if (event.key.code == sf::Keyboard::Space) {
                    int n = std::stoi(number_str);
                    str = "Number: ";
                    number_str = "";
                    number.setString(str);
                    tree.add_element_and_display(n, window, log);
                }
                if (event.key.code == sf::Keyboard::F) {
                    int n = std::stoi(number_str);
                    str = "Number: ";
                    number_str = "";
                    number.setString(str);
                    tree.find_element_and_display(n, window, log);
                }
                if (event.key.code == sf::Keyboard::D) {
                    int n = std::stoi(number_str);
                    str = "Number: ";
                    number_str = "";
                    number.setString(str);
                    tree.delete_element_and_display(n, window, log);
                }
            }
        }
        display_tree(window, log, number, tree);
    }
    return 0;
}