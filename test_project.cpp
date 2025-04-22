#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(1000, 700),
        "Button on SFML 2",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);

    // подключение фона
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("background.png"))
    {
        std::cerr << "Error: can't load background.png\n";
        return 1;
    }

    sf::Sprite bg(bgTexture);
    bg.setScale(
        float(window.getSize().x) / bgTexture.getSize().x,
        float(window.getSize().y) / bgTexture.getSize().y
    );

    // поле ввода
    sf::RectangleShape inputBox({400.f, 50.f});
    inputBox.setFillColor(sf::Color(255, 255, 255, 200));
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(2.f);
    inputBox.setPosition(300.f, 100.f);

    // кнопка
    sf::RectangleShape button({200.f, 65.f});
    button.setFillColor(sf::Color(130, 70, 88));
    button.setOutlineThickness(2.f);
    button.setOutlineColor(sf::Color::White);
    button.setPosition(400.f, 600.f);

    // шрифт
    sf::Font font;
    if (!font.loadFromFile("XI20.ttf"))
    {
        std::cerr << "Font error! XI20.ttf not found.\n";
        return 1;
    }

    // тексты
    sf::String userInput;                           
    sf::Text inputText("", font, 30);
    inputText.setStyle(sf::Text::Bold);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(inputBox.getPosition() + sf::Vector2f(10.f, 10.f));

    sf::Text buttonLabel(L"Push me!", font, 36);
    buttonLabel.setStyle(sf::Text::Bold);
    auto centerLabel = [&]()
    {
        auto r = buttonLabel.getLocalBounds();
        buttonLabel.setOrigin(r.left + r.width / 2, r.top + r.height / 2);
        buttonLabel.setPosition(
            button.getPosition() + sf::Vector2f(button.getSize().x / 2, button.getSize().y / 2)
        );
    };
    centerLabel();

    sf::Event ev;
    while (window.isOpen())
    {
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
                window.close();

            // ввод текста с проверкой ширины
            if (ev.type == sf::Event::TextEntered)
            {
                auto c = ev.text.unicode;
                if (c == 8) // Backspace
                {
                    if (!userInput.isEmpty())
                        userInput.erase(userInput.getSize() - 1, 1);
                    inputText.setString(userInput);
                }
                else if (c >= 32 && c != 127)
                {
                    // вычисляем доступную ширину (отступ слева и справа = 10px)
                    const float padding = 10.f;
                    float maxWidth = inputBox.getSize().x - padding * 2;

                    // временный текст с новым символом
                    sf::String tmp = userInput;
                    tmp += c;
                    inputText.setString(tmp);

                    // измеряем ширину
                    float textWidth = inputText.getLocalBounds().width;
                    if (textWidth <= maxWidth)
                    {
                        // сохраняем символ, если влазит
                        userInput = tmp;
                    }
                    // восстанавливаем фактический текст
                    inputText.setString(userInput);
                }
            }

            // клик мышкой
            if (ev.type == sf::Event::MouseButtonPressed &&
                ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mp = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});

                // клик по полю ввода — очищаем
                if (inputBox.getGlobalBounds().contains(mp))
                {
                    userInput.clear();
                    inputText.setString("");
                }
                // клик по кнопке — выводим приветствие
                else if (button.getGlobalBounds().contains(mp))
                {
                    sf::String greeting = L"Привет, " + userInput;
                    inputText.setString(greeting);
                }
            }
        }

        // отрисовка
        window.clear();
        window.draw(bg);
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(button);
        window.draw(buttonLabel);
        window.display();
    }

    return 0;
}
