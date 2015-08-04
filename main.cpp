#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <SFML/Window.hpp>


#include <iomanip>

#include <locale>

#include <sstream>



using namespace std;



bool gameOn = true;



class playerclass

{

    public:

    playerclass(string inputname, int inputscore)

    {

        name = inputname;

        score = inputscore;

    }

    string name;

    int score;

};



string * ReadHighscores()

{

    string highscores[10];

    int i = 0;

    ifstream inputfile;

    string line;

    inputfile.open("highscores.txt");

    while(getline(inputfile,line))

    {

        highscores[i]=line;

        cout << highscores[i] << endl;

        i++;

    }



    return highscores;

}



string NamePrompt()

{

    string outputstring;

    sf::RenderWindow prompt(sf::VideoMode(400, 300), "Welcome to Brickbreaker");

    sf::String userInput;



    sf::Font pixel;

    if (!pixel.loadFromFile("pixel.ttf"))

    {

        return "no bueno";

    }



    sf::Text entertext;

    entertext.setFont(pixel);

    entertext.setString("Please enter your name:");

    entertext.setCharacterSize(24);

    entertext.setColor(sf::Color::White);

    entertext.setPosition(25,50);



    userInput.insert(userInput.getSize(),">");

    while(prompt.isOpen())

    {

        sf::Event event;

        while( prompt.pollEvent(event))

        {

            prompt.clear();



            // Close window : exit

            if (event.type == sf::Event::Closed)

            prompt.close();



            if(event.type == sf::Event::TextEntered)

            {

                userInput.insert(userInput.getSize(), event.text.unicode);

            }

            else if(event.type == sf::Event::KeyPressed)

            {

                if(event.key.code == sf::Keyboard::BackSpace) // delete the last character

                {

                    userInput.clear();

                    userInput.insert(userInput.getSize(),">");

                }

                if(event.key.code == sf::Keyboard::Space) // delete the last character

                {

                    userInput.clear();

                    userInput.insert(userInput.getSize(), "No_Spaces_Allowed!");

                }

                else if(event.key.code == sf::Keyboard::Return)

                {

                    outputstring = std::string(userInput);

                    prompt.close();

                }

            }



            sf::Text username;

            username.setFont(pixel);

            username.setString(userInput);

            username.setCharacterSize(24);

            username.setColor(sf::Color::White);

            username.setPosition(10,100);

            prompt.draw(entertext);

            prompt.draw(username);

            prompt.display();

        }

    }

    return outputstring;

}



bool highscore(playerclass batman)

{

    bool ishigh;

    ifstream stream("highscores.txt");

    string item_string;

    int item_int;

    int count = 0;

    vector<string> name (10);

    vector<int> score (10);

    int indexer=0;



    while(!stream.eof()) //Format must be "NAME ###" each line

    {

        indexer=count/2;

        if(count%2 == 0 && indexer < 10) //If even, aka item is a name

        {

            stream >> item_string;

            name[indexer] = item_string;

        }

        else         //If odd, aka item is a score

        {

            if (indexer > 9) {break;}

            stream >> item_int;

            score[indexer] = item_int;

        }

        count++;

    }







    if (batman.score > score[9])

    {

        ishigh = true;

        //After new score is a highscore sort the numbers in ascending order (reverse them)

        sort(score.begin(),score.end());

        int insertposition = 0;

        for(int i=0;i<10;i++)

        {

            if(batman.score > score[i])

            {

                insertposition = 9-i;       //If scores are not already sorted will not work

            }

        }



        reverse(score.begin(),score.end()); //returns them to original order, descending



        //insertposition is position to insert new score in descending order



        //This shifts all scores below insertposition down one



        for(int j = 9; j > insertposition;j--)

        {



                name[j]= name[j-1];

                score[j]= score[j-1];

        }



        name[insertposition] = batman.name;

        score[insertposition] = batman.score;



        ofstream outfile;

        outfile.open("highscores.txt");



        for(int n=0;n<10;n++)

        {

        string out = name[n];

        out += ' ';

        out += to_string(score[n]);

        outfile << out <<endl;

        }



        outfile.close();

    }

    else

    {

        ishigh = false;

    }







    return ishigh;

}



bool mouseover(sf::Vector2i localposition, sf::RectangleShape object)

{

    bool mouse_contact = false;

    sf::Vector2f objectorigin = object.getPosition(); //has to be upper left hand corner

    sf::Vector2f dimensions = object.getSize();

    //std::cout << localposition.x << " " << objectorigin.x << std::endl;



    if ((localposition.x > objectorigin.x - dimensions.x/2.f) && (localposition.x < objectorigin.x + dimensions.x/2.f))

    {

        if ((localposition.y > objectorigin.y - dimensions.y/2.f) && (localposition.y < objectorigin.y + dimensions.y/2.f))

        {

            mouse_contact = true;

        }

    }





    return mouse_contact;

}



/////////////////////////////////////////////////////



                        struct Ball

                        {

                        sf::CircleShape b;

                        sf::Vector2f velocity;



                        Ball(float x, float y) //ball constructor

                              : velocity(6.0f, 6.0f)

                            {

                                b.setPosition(x, y);

                                b.setRadius(10.0f);

                                b.setFillColor(sf::Color::Red);

                                b.setRadius(10.0f);

                                b.setOrigin(10.0f, 10.0f);

                            }



                            void speed (int counter){



                        //        if (velocity.x>0){

                        //            velocity.x += counter * 0.1;}

                        //        else if (velocity.x<0){

                        //            velocity.x -= counter * 0.1;}

                                if (velocity.y>0){

                                    velocity.y += counter * 0.1;}

                                else if (velocity.y < 0){

                                    velocity.y -= counter * .01;}

                            }



                        //ball movement

                        bool update_ball()

                            {

                            b.move(velocity);





                            if (xLeft() <= 0)

                                    velocity.x = -velocity.x;

                                else if (xRight() >= 800)

                                velocity.x = -velocity.x;







                            if (yTop() <= 0)

                                velocity.y = -velocity.y;

                                else if (yBottom() >= 600)

                                return gameOn=false;

                            }



                            float x() const { return b.getPosition().x; }

                            float y() const { return b.getPosition().y; }



                            float xLeft() const { return x() - b.getRadius(); }



                            float xRight() const { return x() + b.getRadius(); }



                            float yTop() const { return y() - b.getRadius(); }



                            float yBottom() const { return y() + b.getRadius(); }

                        };





                        struct Paddle

                        {

                            sf::RectangleShape p;

                            sf::Vector2f velocity;



                            Paddle(float x)

                              : velocity(0.f, 0.f)

                            {



                                p.setPosition(x, 600 - 50.0f);

                                p.setSize(sf::Vector2f(100.f, 20.f));

                                p.setFillColor(sf::Color::Magenta);

                                p.setOrigin(80.f / 2.f, 20.f / 2.f);

                            }







                        void update_paddle(sf::Vector2i localposition) /*Mouse input section, local position*/

                            {



                                if(localposition.x < 40)

                                {

                                    p.setPosition(40, 600 - 50.0f);

                                }

                                else if(localposition.x > 760)

                                {

                                    p.setPosition(760, 600 - 50.0f);

                                }

                                else

                                {

                                    p.setPosition(localposition.x, 600 - 50.0f);

                                }

                                std::cout<<localposition.x<<std::endl;

                            }



                            float x() const { return p.getPosition().x; }

                            float y() const { return p.getPosition().y; }



                            float xLeft() const { return x() - p.getSize().x / 2.f; }

                            float xRight() const { return x() + p.getSize().x / 2.f; }

                            float yTop() const { return y() - p.getSize().y / 2.f; }

                            float yBottom() const { return y() + p.getSize().y / 2.f; }

                        };





                        struct Brick

                        {

                            sf::RectangleShape br;

                            //bool for collision function

                            bool brickDestroyed;

                            float lives;





                            Brick(float x, float y)

                              : brickDestroyed(false)

                            {



                            br.setPosition((x+1)*(66.f), (y + 2)*(23.f));

                            br.setSize(sf::Vector2f(60.0f, 20.0f));

                            br.setOrigin(60.0f / 2.f, 20.0f / 2.f);

                            if (y == 4)

                                br.setFillColor(sf::Color::Cyan);

                            if (y == 3)

                                br.setFillColor(sf::Color::Red);

                            if (y == 2)

                                br.setFillColor(sf::Color::Green);

                            if (y == 1)

                               br.setFillColor(sf::Color::Yellow);

                            if (y == 0)

                               br.setFillColor(sf::Color::Magenta);



                            lives = 5.f - y;



                            }





                            float x() const { return br.getPosition().x; }

                            float y() const { return br.getPosition().y; }



                            float xLeft() const { return x() - br.getSize().x / 2.f; }

                            float xRight() const { return x() + br.getSize().x / 2.f; }

                            float yTop() const { return y() - br.getSize().y / 2.f; }

                            float yBottom() const { return y() + br.getSize().y / 2.f; }

                        };



                        bool intersecting_bp(Ball const& ball, Paddle const& paddle)

                        {

                            return ball.xRight() >= paddle.xLeft() && ball.xLeft() <= paddle.xRight() &&

                                   ball.yBottom() >= paddle.yTop() && ball.yTop() <= paddle.yBottom();

                        }





                        bool intersecting_bb(Ball const& ball, Brick const& br)

                         {

                            return ball.xRight() >= br.xLeft() && ball.xLeft() <= br.xRight() &&

                                   ball.yBottom() >= br.yTop() && ball.yTop() <= br.yBottom();

                        }



                        //ball and paddle collision check + bounce back r

                        void collision_check(Ball& ball, Paddle const& paddle)

                        {





                            if (!intersecting_bp(ball, paddle))

                                return;





                            ball.velocity.y = -6.0f;



                           /* CHANGES MADE HERE: THIS CHANGES BALL ANGLE */

                            float mag = sqrt(pow(ball.velocity.x,2)+pow(ball.velocity.y,2));

                            if (ball.x() < paddle.x()-30) //0-20

                            {

                                ball.velocity.x=mag*-cos(3.141592653589/4);

                                ball.velocity.y=mag*sin(-3.141592653589/4);

                                //ball.velocity.x = -6.0f;

                            }

                            else if((ball.x() > paddle.x()-30) && (ball.x() < paddle.x()-10)) //20-40

                            {

                                ball.velocity.x=mag*-cos(3.141592653589/3);

                                ball.velocity.y=mag*sin(-3.141592653589/3);

                            }

                            else if((ball.x() > paddle.x()-10) && (ball.x() < paddle.x()+10))//40-60

                            {

                                ball.velocity.x=mag*cos(3.141592653589/2);

                                ball.velocity.y=mag*sin(-3.141592653589/2);

                            }

                            else if((ball.x() > paddle.x()+10) && (ball.x() < paddle.x()+30)) //60-80

                            {

                                ball.velocity.x=mag*cos(3.141592653589/3);

                                ball.velocity.y=mag*sin(-3.141592653589/3);

                            }

                            else

                            {

                                ball.velocity.x=mag*cos(3.141592653589/4);

                                ball.velocity.y=mag*sin(-3.141592653589/4);

                            }

                        }





                        void collision_check(Ball& ball, Brick& brick, int& counter, sf::Vector2f& velocity)

                        {



                            if (!intersecting_bb(ball, brick))

                                return;



                            brick.lives -= 1.f;



                            //otherwise, return bool to brick eraser in main

                            if (brick.lives == 0){

                                brick.brickDestroyed = true;



                            counter++;

                            if (velocity.x>0){

                                    velocity.x += counter * 0.1;}

                            else if (velocity.x<0){

                                    velocity.x -= counter * 0.1;}

                            if (velocity.y>0){

                                    velocity.y += counter * 0.1;}

                            else if (velocity.y < 0){

                                    velocity.y -= counter * .01;}



                            }



                            bool leftCollide = abs(ball.xRight() - brick.xLeft()) < abs(brick.xRight() - ball.xLeft());



                            bool topCollide = abs(ball.yBottom() - brick.yTop()) < abs(brick.yBottom() - ball.yTop());



                            float overlapX;

                            float overlapY;



                            if (leftCollide)

                                overlapX = (ball.xRight() - brick.xLeft());

                            else

                              overlapX = (brick.xRight() - ball.xLeft());



                            if (topCollide)

                              overlapY = (ball.yBottom() - brick.yTop());

                            else

                              overlapY = (brick.yBottom() - ball.yTop());



                            if(abs(overlapX) < abs(overlapY)) {
                                if (leftCollide == true)
                                    {ball.velocity.x = -1*ball.velocity.x;}

                                else
                                    {ball.velocity.x = ball.velocity.x;}
                            }

                            else {
                                if (topCollide == true)
                                    {ball.velocity.y = -6.0f;}

                                else
                                    {ball.velocity.y = 6.0f;}

                                }

                            }

                        string scorer (int counter, sf::Vector2f velocity){

                            string score;

                            float mag = sqrt(pow(velocity.x,2)+pow(velocity.y,2));

                            int temp = counter * 100;



                            score = "SCORE: ";

                            ostringstream convert;

                            convert<<temp;

                            score = score + convert.str();



                            return score;

                        }



                        void colorSet(vector<Brick>& brickgrid){

                            for (int i=0; i<brickgrid.size(); i++){

                                if (brickgrid[i].lives == 1)

                                    brickgrid[i].br.setFillColor(sf::Color::Cyan);

                                if (brickgrid[i].lives == 2)

                                    brickgrid[i].br.setFillColor(sf::Color::Red);

                                if (brickgrid[i].lives == 3)

                                    brickgrid[i].br.setFillColor(sf::Color::Green);

                                if (brickgrid[i].lives == 4)

                                   brickgrid[i].br.setFillColor(sf::Color::Yellow);

                                if (brickgrid[i].lives == 5)

                                   brickgrid[i].br.setFillColor(sf::Color::Magenta);

                            }

                        }

///////////////////////////////////////



int main()

{



    bool play = false;

    bool promptuser = true;

    string playername = "No_name";

    // Create the main window

    sf::RenderWindow app(sf::VideoMode(1024, 640), "SFML window");



    sf::RectangleShape play_box(sf::Vector2f(750,100));

    play_box.setOrigin(375,50);

    play_box.setPosition(512,240);

    play_box.setFillColor(sf::Color::Green);



    sf::RectangleShape quit_box(sf::Vector2f(750,100));

    quit_box.setOrigin(375,50);

    quit_box.setPosition(512,420);

    quit_box.setFillColor(sf::Color::Red);





    sf::Font Mont;

    if (!Mont.loadFromFile("Mont.ttf"))

    {

        return EXIT_FAILURE;

    }



    sf::Font type;

    if (!type.loadFromFile("type.ttf"))

    {

        return EXIT_FAILURE;

    }



    sf::Font Lies;

    if (!Lies.loadFromFile("pixel.ttf"))

    {

        return EXIT_FAILURE;

    }



    sf::Text text_title;

    text_title.setFont(Mont);

    text_title.setString("BrickKiller");

    text_title.setCharacterSize(96);

    text_title.setColor(sf::Color::White);

    text_title.setPosition(250,20);



    sf::Text text_question;

    text_question.setFont(type);

    text_question.setString("Art thou ready to embark on the noble quest of breaking bricks?");

    text_question.setCharacterSize(38);

    text_question.setColor(sf::Color::Black);

    text_question.setPosition(12,115);



    sf::Text text_play;

    text_play.setFont(Lies);

    text_play.setString("KILL THE BRICKS!");

    text_play.setCharacterSize(60);

    text_play.setColor(sf::Color::Black);

    text_play.setPosition(180,200);



    sf::Text text_quit;

    text_quit.setFont(Lies);

    text_quit.setString("NO! Busy debugging PA4!");

    text_quit.setCharacterSize(60);

    text_quit.setColor(sf::Color::Black);

    text_quit.setPosition(160,380);





    sf::Texture background_texture;

    if (!background_texture.loadFromFile("bricks.jpg"))

    {

        return EXIT_FAILURE;

    }

    sf::Sprite background(background_texture);



    // Loads background of start window

    sf::Texture endscreen;

    if (!endscreen.loadFromFile("end.png"))

    {

        return EXIT_FAILURE;

    }

    sf::Sprite endbackground(endscreen);



    sf::Text bellaspeaks;

    bellaspeaks.setFont(type);

    bellaspeaks.setString("Professor will not be happy...");

    bellaspeaks.setCharacterSize(50);

    bellaspeaks.setColor(sf::Color::Black);

    bellaspeaks.setPosition(300,30);



    sf::RectangleShape scorescreen(sf::Vector2f(400,400));

    scorescreen.setOrigin(200,200);

    scorescreen.setPosition(650,400);

    scorescreen.setFillColor(sf::Color::Cyan);



    sf::RectangleShape outline(sf::Vector2f(440,440));

    outline.setOrigin(220,220);

    outline.setPosition(650,400);

    outline.setFillColor(sf::Color::White);



    sf::Text hightext;

    hightext.setFont(Mont);

    hightext.setString("Highscores");

    hightext.setCharacterSize(40);

    hightext.setColor(sf::Color::Black);

    hightext.setPosition(550,200);



    int spacing = 30;

    int x_score = 550;

    int initial = 270;



    sf::Text score0;

    score0.setFont(Lies);

    score0.setString("scores[0]");

    score0.setCharacterSize(20);

    score0.setColor(sf::Color::Black);

    score0.setPosition(x_score,initial);



    sf::Text score1;

    score1.setFont(Lies);

    score1.setString("scores[1]");

    score1.setCharacterSize(20);

    score1.setColor(sf::Color::Black);

    score1.setPosition(x_score,initial+1*spacing);



    sf::Text score2;

    score2.setFont(Lies);

    score2.setString("scores[2]");

    score2.setCharacterSize(20);

    score2.setColor(sf::Color::Black);

    score2.setPosition(x_score,initial+2*spacing);



    sf::Text score3;

    score3.setFont(Lies);

    score3.setString("scores[3]");

    score3.setCharacterSize(20);

    score3.setColor(sf::Color::Black);

    score3.setPosition(x_score,initial+3*spacing);



    sf::Text score4;

    score4.setFont(Lies);

    score4.setString("scores[4]");

    score4.setCharacterSize(20);

    score4.setColor(sf::Color::Black);

    score4.setPosition(x_score,initial+4*spacing);



    sf::Text score5;

    score5.setFont(Lies);

    score5.setString("scores[5]");

    score5.setCharacterSize(20);

    score5.setColor(sf::Color::Black);

    score5.setPosition(x_score,initial+5*spacing);



    sf::Text score6;

    score6.setFont(Lies);

    score6.setString("scores[6]");

    score6.setCharacterSize(20);

    score6.setColor(sf::Color::Black);

    score6.setPosition(x_score,initial+6*spacing);



    sf::Text score7;

    score7.setFont(Lies);

    score7.setString("scores[7]");

    score7.setCharacterSize(20);

    score7.setColor(sf::Color::Black);

    score7.setPosition(x_score,initial+7*spacing);



    sf::Text score8;

    score8.setFont(Lies);

    score8.setString("scores[8]");

    score8.setCharacterSize(20);

    score8.setColor(sf::Color::Black);

    score8.setPosition(x_score,initial+8*spacing);



    sf::Text score9;

    score9.setFont(Lies);

    score9.setString("scores[9]");

    score9.setCharacterSize(20);

    score9.setColor(sf::Color::Black);

    score9.setPosition(x_score,initial+9*spacing);



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



        while(play) //change to while

        {

            if(promptuser)

            {

                playername = NamePrompt();

                promptuser = false;



            }









/*///////////////////////////////// GAME INFORMATION ///////////////////////////////////////////*/













                            sf::RenderWindow window(sf::VideoMode(800, 600), "EC327 BrickKiller");



                            window.setFramerateLimit(40);

                            int counter = 0;

                            sf::Font font;

//                            font.loadFromFile("DroidSansFallbackFull.ttf");

//                            if (!font.loadFromFile("DroidSansFallbackFull.ttf")){

//                                std::cout<<"Error"<<std::endl;

//                            }



                            font.loadFromFile("type.ttf");

                            if (!font.loadFromFile("type.ttf")){

                                std::cout<<"Error"<<std::endl;

                            }

                            sf::Text text;

                            text.setFont(font);

                            text.setCharacterSize(30);

                            text.setStyle(sf::Text::Regular);

                            text.setColor(sf::Color::Red);

                            //text.setPosition(630,0);

                            std::string score;



                            Ball ball (800 / 2, 600 / 2);

                            Paddle paddle (800 / 2);

                            vector<Brick> brickgrid;



                            for (int x = 0; x < 11; x++)

                                for (int y = 0; y < 5; y++)

                                    brickgrid.push_back(Brick(x,y));



                            while(!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) && gameOn)//esc or ball leaving window ends game

                            {

                            score = scorer(counter, ball.velocity);

                            text.setString(score);



                            window.clear(sf::Color::Black);



                            sf::Vector2i localposition = sf::Mouse::getPosition(window);



                            ball.update_ball();



                            paddle.update_paddle(localposition);



                            collision_check(ball, paddle);





                            for (auto& current_brick: brickgrid)

                                collision_check(ball, current_brick, counter, ball.velocity);



                                brickgrid.erase(

                                    remove_if(brickgrid.begin(), brickgrid.end(),  [] (Brick const& current_brick)

                                 {return current_brick.brickDestroyed; }), brickgrid.end());



                            if(brickgrid.empty())

                            {

                                gameOn = false;

                            }





                            colorSet(brickgrid);





                            window.draw(text);

                            window.draw(ball.b);

                            window.draw(paddle.p);



                            for (auto const& current_brick: brickgrid)

                                    window.draw(current_brick.br);



                            window.display();

                            }





/*////////////////////////// END GAME INFORMATION //////////////////////////////////*/











            if(brickgrid.empty() || !gameOn) //end trigger

            {

                window.clear();

                window.close();

                cout<<"Player name is: "<<playername<<endl;



                int inputscore = counter * 100;

                playerclass batman(playername, inputscore);

                bool decent = highscore(batman);



                    string scorelist[10];

                    int i = 0;

                    ifstream inputfile;

                    string line;

                    inputfile.open("highscores.txt");

                    while(getline(inputfile,line))

                    {

                        scorelist[i]=line;

                        i++;

                    }



                score0.setString(scorelist[0]);

                score1.setString(scorelist[1]);

                score2.setString(scorelist[2]);

                score3.setString(scorelist[3]);

                score4.setString(scorelist[4]);

                score5.setString(scorelist[5]);

                score6.setString(scorelist[6]);

                score7.setString(scorelist[7]);

                score8.setString(scorelist[8]);

                score9.setString(scorelist[9]);



                while(play)

                {

                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))

                    {

                        app.close();

                        break;



                    }

                    app.clear();



                    app.draw(endbackground);

                    app.draw(bellaspeaks);

                    app.draw(outline);

                    app.draw(scorescreen);

                    app.draw(hightext);

                    app.draw(score0);

                    app.draw(score1);

                    app.draw(score2);

                    app.draw(score3);

                    app.draw(score4);

                    app.draw(score5);

                    app.draw(score6);

                    app.draw(score7);

                    app.draw(score8);

                    app.draw(score9);



                    app.display();

                }

                //Begin printing scores from highscores.txt

            }



            app.display();

        }

    }



    return EXIT_SUCCESS;

}
