#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <vector>
#include <iomanip>
#include <locale>
#include <sstream>
#include <cmath>

bool gameOn = true;

using namespace std;

//John
//beta ball class
struct Ball
{
sf::CircleShape b;
sf::Vector2f velocity; //2d vector to store ball's velocity

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

//ball movement function
bool update_ball()
    {
    b.move(velocity);

    // how to keep ball inside the screen?
    // fixed

		// if hits left wall change velocity to positive (to the right).
		// if hits right wall, change velocity to negative (to the left)
    if (xLeft() <= 0)
            velocity.x = -velocity.x;
        else if (xRight() >= 800) //change windox x as needed
        velocity.x = -velocity.x;

        //same idea for top/bottom collisions

    if (yTop() <= 0)
        velocity.y = -velocity.y;
        else if (yBottom() >= 600) //change window y as needed
        return gameOn=false;
    }

     // MAKE SURE TO RETYPE ALL BELOW IN oTHER CLASSES

     //methods to store useful position info via Circleshape for class use
    float x() const { return b.getPosition().x; }
    float y() const { return b.getPosition().y; }

    float xLeft() const { return x() - b.getRadius(); }

    float xRight() const { return x() + b.getRadius(); }

    float yTop() const { return y() - b.getRadius(); }

    float yBottom() const { return y() + b.getRadius(); }
};

//thomas
struct Paddle
{
    sf::RectangleShape p;
    sf::Vector2f velocity; //vector to store paddle's position

    Paddle(float x) //paddle consructor
      : velocity(0.f, 0.f)
    {
       //change numbers as needed but peserve ratio
        p.setPosition(x, 600 - 50.0f);
        p.setSize(sf::Vector2f(100.f, 20.f));
        p.setFillColor(sf::Color::Magenta);
        p.setOrigin(80.f / 2.f, 20.f / 2.f);
    }


//paddle movement -- based on keystrokes
//++mouse??
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
         cout<<localposition.x<< endl;
    }

    //store useful position info via RectangleShape for class use
    float x() const { return p.getPosition().x; }
    float y() const { return p.getPosition().y; }

    float xLeft() const { return x() - p.getSize().x / 2.f; }
    float xRight() const { return x() + p.getSize().x / 2.f; }
    float yTop() const { return y() - p.getSize().y / 2.f; }
    float yBottom() const { return y() + p.getSize().y / 2.f; }
};

//aurelia + nick
struct Brick
{
    sf::RectangleShape br;
    //bool for collision function
    bool brickDestroyed;
    float lives;

    //brick constructor -- fixed
    Brick(float x, float y)
      : brickDestroyed(false)
    {
    //changenumbers as needed but preserve ratio
    br.setPosition((x+1)*(66.f), (y + 2)*(23.f));
    br.setSize(sf::Vector2f(60.0f, 20.0f));
    br.setOrigin(60.0f / 2.f, 20.0f / 2.f);

    lives = 5.f - y;

    }


    //get useful position info via RectangleShape for class use
    float x() const { return br.getPosition().x; }
    float y() const { return br.getPosition().y; }

    float xLeft() const { return x() - br.getSize().x / 2.f; }
    float xRight() const { return x() + br.getSize().x / 2.f; }
    float yTop() const { return y() - br.getSize().y / 2.f; }
    float yBottom() const { return y() + br.getSize().y / 2.f; }
};

//intersecting function checks collision, can accept two different types of input
//from all 3 classes, ball and paddle or ball and brick

template <typename T1, typename T2>
bool intersecting(T1 const& obj1, T2 const& obj2)
{
    return obj1.xRight() >= obj2.xLeft() && obj1.xLeft() <= obj2.xRight() &&
           obj1.yBottom() >= obj2.yTop() && obj1.yTop() <= obj2.yBottom();
}

//ball and paddle collision check + bounce back r
void collision_check(Ball& ball, Paddle const& paddle)
{

    //no overlap, break
    if (!intersecting(ball, paddle))
        return;

   //bounce back
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

//ball + brick collision check/response
void collision_check(Ball& ball, Brick& brick, int& counter, sf::Vector2f& velocity) //add sf::Sound sound as paramter
{


    if (!intersecting(ball, brick)) //wrong type?? error
        return;

    brick.lives -= 1.f;

    //otherwise, return bool to brick eraser in main
    if (brick.lives == 0){
        brick.brickDestroyed = true;
    //sound.play();

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
    //calculate where collision was
    float left = ball.xRight() - brick.xLeft();
    float right = brick.xRight() - ball.xLeft();
    float top = ball.yBottom() - brick.yTop();
    float bottom = brick.yBottom() - ball.yTop();

    //THIS PART WAS SO HARD

    //true if left collision, false if right
    bool leftCollide = abs(left) < abs(right);

    //true if top collision, false if bottom
    bool topCollide = abs(top) < abs(bottom);

    //store overlaps x and y axes, determine if left or right/top or bottom using 2 bools above

    float overlapX = leftCollide ? left : right;
    float overlapY = topCollide ? top : bottom;

    //if overlap x > overlap y, ball hit brick horizontally
    //otherwise, it hit it vertically
    //change either x or y velo based on what makes sense
    //not compiling? --fixed
    if(abs(overlapX) < abs(overlapY))
        //ball.velocity.x = leftCollide ? -6.0f : 6.0f;
        ball.velocity.x = leftCollide ? -1*ball.velocity.x : ball.velocity.x;


    else
        //when I try to change this so that it bounces back with -velocity.y it starts to destroy multiple bricks at a time
        ball.velocity.y = topCollide ? -6.0f : 6.0f;
}
//Adds to the users score every time a brick is destroyed
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

void const colorSet(vector<Brick>& brickgrid){
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


int main()
{
    //game window
    sf::RenderWindow window(sf::VideoMode(800, 600), "EC327 BrickKiller");
    //too high/low?
    window.setFramerateLimit(40);
    int counter = 0;
    sf::Font font;
    font.loadFromFile("DroidSansFallbackFull.ttf");
    if (!font.loadFromFile("DroidSansFallbackFull.ttf")){
         cout<<"Error"<< endl;
    }

    sf::Texture texture;
    if(!texture.loadFromFile("castle2.png"))
    {
        return EXIT_FAILURE;
    }

    sf::Sprite background;
    background.setTexture(texture);
    background.setTextureRect(sf::IntRect(100,100,800,600));

//    sf::Music music;
//    if (!music.openFromFile("DangerZone.ogg")){
//        return EXIT_FAILURE;
//    }
//    music.play();
//    music.setLoop(true);
//
//    sf::SoundBuffer buffer;
//    if(!buffer.loadFromFile("Explode.ogg")){
//        return EXIT_FAILURE;
//    }
//    sf::Sound sound;
//    sound.setBuffer(buffer);

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(30);
    text.setStyle(sf::Text::Regular);
    text.setColor(sf::Color::Red);
    //text.setPosition(630,0);
     string score;
    //"ball" is instance of Ball class, place in center change window as needed
    Ball ball (800 / 2, 600 / 2);

    //"paddle" is instance of Paddle class, center of bottom
    Paddle paddle (800 / 2);

    //vector for grid of bricks
    //error? not usable in a constant expression? -- fixed
    vector<Brick> brickgrid;

    //fill up vector, change dimensions to whatever    window.draw(ball.b);
    for (int x = 0; x < 11; x++)
        for (int y = 0; y < 5; y++)
            brickgrid.push_back(Brick(x,y));

    //game loop
    while(!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) && gameOn)//esc ends game
    {

    score = scorer(counter, ball.velocity);
    text.setString(score);

    //erase anything old?
    window.clear(sf::Color::Black);


    sf::Vector2i localposition = sf::Mouse::getPosition(window);

    ball.update_ball(); //run ball movement

    paddle.update_paddle(localposition); //run paddle movement


    collision_check(ball, paddle); //always running

    //always running, checking for all bricks
    for (auto& current_brick: brickgrid)
        collision_check(ball, current_brick, counter, ball.velocity); //add sound in as parameter

    //brick eraser -- when intersect check returns brickDestroyed = true
        brickgrid.erase(
            remove_if(brickgrid.begin(), brickgrid.end(),
               [] (Brick const& current_brick)
         {return current_brick.brickDestroyed; }),
            brickgrid.end()
         );

    //ending message/score
    colorSet(brickgrid);
    if (brickgrid.empty()) //game over
    break;

    //draw instance of ball
    window.draw(background);
    window.draw(text);
    window.draw(ball.b);

    //draw instance of paddle
    window.draw(paddle.p);

    //range-based for, draw every brick in brick grid vector
    for (auto const& current_brick: brickgrid)
            window.draw(current_brick.br);

    //run
    window.display();
    }
    return 0;
}