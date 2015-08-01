#include <SFML/Graphics.hpp>
#include <iostream>

bool mouseover(sf::Vector2i localposition, sf::RectangleShape object)
{
    bool mouse_contact = false;
    sf::Vector2f objectorigin = object.getPosition(); //has to be upper left hand corner
    sf::Vector2f dimensions = object.getSize();
    std::cout << localposition.x << " " << objectorigin.x << std::endl;

    if ((localposition.x > objectorigin.x - dimensions.x/2.f) && (localposition.x < objectorigin.x + dimensions.x/2.f))
    {
        if ((localposition.y > objectorigin.y - dimensions.y/2.f) && (localposition.y < objectorigin.y + dimensions.y/2.f))
        {
            mouse_contact = true;
        }
    }


    return mouse_contact;
}

int main()
{
    bool play = false;
    // Create the main window
    sf::RenderWindow app(sf::VideoMode(1024, 640), "SFML window");

    sf::RectangleShape play_box(sf::Vector2f(720,100));
    play_box.setOrigin(360,50);
    play_box.setPosition(512,240);
    play_box.setFillColor(sf::Color::Green);

    sf::RectangleShape quit_box(sf::Vector2f(720,100));
    quit_box.setOrigin(360,50);
    quit_box.setPosition(512,420);
    quit_box.setFillColor(sf::Color::Red);


    sf::Font Mont;
    if (!Mont.loadFromFile("Mont.ttf"))
    {
        return EXIT_FAILURE;
    }

    sf::Font Lies;
    if (!Lies.loadFromFile("Lies.ttf"))
    {
        return EXIT_FAILURE;
    }

    sf::Text text_title;
    text_title.setFont(Mont);
    text_title.setString("Brickbreaker");
    text_title.setCharacterSize(96);
    text_title.setColor(sf::Color::White);
    text_title.setPosition(250,30);

    sf::Text text_question;
    text_question.setFont(Mont);
    text_question.setString("Art thou ready to embark on the noble quest of breaking bricks?");
    text_question.setCharacterSize(36);
    text_question.setColor(sf::Color::Yellow);
    text_question.setPosition(10,130);

    sf::Text text_play;
    text_play.setFont(Lies);
    text_play.setString("Bricks bad. Bricks die.");
    text_play.setCharacterSize(60);
    text_play.setColor(sf::Color::Black);
    text_play.setPosition(180,200);

    sf::Text text_quit;
    text_quit.setFont(Lies);
    text_quit.setString("I am a worthless coward");
    text_quit.setCharacterSize(60);
    text_quit.setColor(sf::Color::Black);
    text_quit.setPosition(160,380);

    // Loads background of start window
    sf::Texture background_texture;
    if (!background_texture.loadFromFile("metal.jpeg"))
    {
        return EXIT_FAILURE;
    }
    sf::Sprite background(background_texture);

	// Start the game loop
    while (app.isOpen())
    {
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
        }

        while (!play)
        {
        // Clear screen
        app.clear();

            //Test area
            sf::Vector2i localposition = sf::Mouse::getPosition(app);
            bool playmouse = mouseover(localposition, play_box);
            //std::cout << test << std::endl;
            if(playmouse)
            {
                play_box.setFillColor(sf::Color::White);
                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    play = true;
                }
            }
            else
            {
                play_box.setFillColor(sf::Color::Green);
            }

            bool quitmouse = mouseover(localposition, quit_box);
            if(quitmouse)
            {
                quit_box.setFillColor(sf::Color::White);
                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    app.close();
                    break;
                }
            }
            else
            {
                quit_box.setFillColor(sf::Color::Red);
            }

        app.draw(background);
        app.draw(text_title);
        app.draw(text_question);
        app.draw(play_box);
        app.draw(text_play);
        app.draw(quit_box);
        app.draw(text_quit);
        // Update the window
        app.display();
        }

        while (play)
        {
        // Clear screen
        app.clear();

        // Update the window
        app.display();
        }
    }

    return EXIT_SUCCESS;
}
