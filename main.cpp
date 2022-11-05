#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <bits/stdc++.h>
#include <fstream>

using namespace sf;
using namespace std;

#define dl "\n"

class Menu{
    private:
        Font font;
        Text text[3];
        int choice = 0;

    public:

        Menu(double width, double hight){
            if(not font.loadFromFile("fonts/BebasNeue-Regular.ttf")){
                cout << "cannot load the font" << dl;
            }

            width -= 800;

            text[0].setCharacterSize(75);
            text[0].setFont(font);
            text[0].setString("Read");
            text[0].setColor(Color(255,165,0));
            text[0].setPosition(Vector2f(width / 2, hight / (3 + 2)));

            text[1].setCharacterSize(75);
            text[1].setFont(font);
            text[1].setString("Play");
            text[1].setColor(Color::White);
            text[1].setPosition(Vector2f(width / 2, hight / (3 + 2) * 2));

            text[2].setCharacterSize(75);
            text[2].setFont(font);
            text[2].setString("Exit");
            text[2].setColor(Color::White);
            text[2].setPosition(Vector2f(width / 2, hight / (3 + 2) * 3));
        }

        void disPlay(RenderWindow& window){
            for(int i=0; i<3; i++){
                window.draw(text[i]);
            }
        }

        void changeChoice(int type){
            if(type == 1){
                choice = min(2 , choice + 1);
                text[choice].setColor(Color(255,165,0));
                text[choice - 1].setColor(Color::White);

            }
            if(type == 0){
                choice = max(0 , choice - 1);
                text[choice].setColor(Color(255,165,0));
                text[choice + 1].setColor(Color::White);
            }
        }

        int getChoice(){
            return choice;
        }
};

// -----------------------------------------------------------------------------//

int currSlide = 0;
const int numberOfSlides = 38;
Texture slidesTex[numberOfSlides];
Sprite slides[numberOfSlides];

int main()
{

    for(int i=0; i<numberOfSlides; i++){
        string s = "slides/SS";
        s += to_string(i);
        s += ".png";

        if(not slidesTex[i].loadFromFile(s)){
            cout << "cannot load the slides ..." << dl;
            return -1;
        }

        slides[i].setTexture(slidesTex[i]);
        slides[i].setPosition(200, 0);
    }

    RenderWindow window(VideoMode(1024, 600), "Final Project");
    window.setFramerateLimit(80);

    Font font;
	if(not font.loadFromFile("fonts/Limit Breaking.ttf")){
        cout << "font cannot be load .." << "\n";
        return -1;
	}

    /// game over ///
    bool isOver = false;

    Text gameOver;
    gameOver.setCharacterSize(50);
    gameOver.setFillColor(Color::Red);
    gameOver.setFont(font);
    gameOver.setString("Game Over ..");
    gameOver.setPosition(250 , 120);

    /// sound when get damaged ///
    SoundBuffer soundSource, gameOversource;
	if(not soundSource.loadFromFile("music/soot.ogg") or not gameOversource.loadFromFile("music/GameOverSound.ogg")){
        cout << "cannot load the sooooot ..." << dl;
        return -1;
	}

	Sound sound, gameOverSound;
	sound.setVolume(600);
	sound.setBuffer(soundSource);
	gameOverSound.setVolume(400);
	gameOverSound.setBuffer(gameOversource);


    /// score & lvl ///
    int totalScore = 0;

    Text score, lvl;
    score.setCharacterSize(11);
    score.setColor(Color::White);
    score.setPosition(800, 50);
    score.setFont(font);
    lvl.setCharacterSize(12);
    lvl.setColor(Color::Green);
    lvl.setPosition(500, 30);
    lvl.setString("lvl 1");
    lvl.setFont(font);


    /// health ///
	int hp = 10;

    RectangleShape hpBar;
	hpBar.setFillColor(Color::Red);
	hpBar.setSize(Vector2f(-(float)hp * 30.f, 30.f));
	hpBar.setPosition(920, 12);

	/// ship ///
    Texture shipTex;
	Sprite ship;

	if(not shipTex.loadFromFile("images/ship.png")){
        cout << "cannot load shipTex ..." << dl;
        return -1;
	}
	ship.setTexture(shipTex);
	ship.setScale(.3, .3);


	/// asteroid ///
	Texture astTex;
	Sprite ast;

	if(not astTex.loadFromFile("images/astroid.png")){
        cout << "cannot load the astroid Tex ..." << dl;
        return -1;
    }
	ast.setTexture(astTex);
	ast.setScale(.2, .2);
	int spawnTimer = 15;
	vector<Sprite> spawnedAst;
	spawnedAst.push_back(ast);

	/// game back ground ///
	Texture gameBackTex;
	Sprite gameBackGround;
	if(not gameBackTex.loadFromFile("images/spaceBackGround.png")){
        cout << "cannot load the game back ground ..." << dl;
        return -1;
	}
	gameBackGround.setTexture(gameBackTex);
	gameBackGround.setScale(.5, .5);

    /// avatar ///
    Texture avatarTex, frameTex;
    Sprite avatar, frame;

    if(not avatarTex.loadFromFile("images/Avatar.png") or not frameTex.loadFromFile("images/AvatarFrame.png")){
        cout << "cannot load the avatar ..." << dl;
        return -1;
    }
    frame.setTexture(frameTex);
    frame.setScale(.45, .45);
    frame.setPosition(909, 0);
    avatar.setTexture(avatarTex);
    avatar.setScale(.3, .3);
    avatar.setPosition(925, 15);

    /// menu back ground ///
    Sprite MenuBack;
    Texture backGroundMenu;

    if(not backGroundMenu.loadFromFile("images/MenuBackGround.png")){
        cout << "cannot load backGroundMenu ..." << dl;
        return -1;
    }
    MenuBack.setTexture(backGroundMenu);

    /// 0 for menu , 1 for Ebook , 2 for game ///
    int curr = 0;
    float speed = -11;
    Event event;
    Menu menu(window.getSize().x, window.getSize().y);

    Text teamName;
    teamName.setCharacterSize(32);
    teamName.setFont(font);
    teamName.setString("GTCS");
    teamName.setColor(Color::White);
    teamName.setPosition(300, 25);

    /// music in menu ///
    Music menuMusic;
    if(not menuMusic.openFromFile("music/dayman.wav")){
        cout << "cannot load the music .." << dl;
        return -1;
    }
    menuMusic.play();
    menuMusic.setLoop(true);

    float lastOffset = 0;

    while(window.isOpen()){

        while(window.pollEvent(event)){

            if(Keyboard::isKeyPressed(Keyboard::Escape)){
                curr = 0, window.setSize(Vector2u(1024, 600));
                totalScore = 0, hp = 10, speed = -11;
                lvl.setString("lvl 1");
                lvl.setColor(Color::Green);
                isOver = false;
            }

            if(Keyboard::isKeyPressed(Keyboard::P) and curr != 2)
                menuMusic.setPlayingOffset(seconds(lastOffset)), menuMusic.play();

            if(Keyboard::isKeyPressed(Keyboard::O) and curr != 2)
                lastOffset = menuMusic.getPlayingOffset().asSeconds(), menuMusic.pause();


            if(event.type == Event::Closed)
                window.close();

            ///menu///
            if(curr == 0){
                if(Keyboard::isKeyPressed(Keyboard::Down))
                    menu.changeChoice(1);
                else if(Keyboard::isKeyPressed(Keyboard::Up))
                    menu.changeChoice(0);

                if(Keyboard::isKeyPressed(Keyboard::Enter)){
                    int currChoice = menu.getChoice();

                    if(currChoice == 0)
                        curr = 1, window.setSize(Vector2u(1024, 600));
                    if(currChoice == 1)
                        curr = 2, menuMusic.pause();
                    if(currChoice == 2)
                        window.close();
                }

            }

            /// Ebook ///
            if(curr == 1){

                if(Keyboard::isKeyPressed(Keyboard::Right))
                    currSlide = min(numberOfSlides-1 , currSlide + 1);
                else if(Keyboard::isKeyPressed(Keyboard::Left))
                    currSlide = max(0 , currSlide - 1);

                if(currSlide >= 35)
                    slides[currSlide].setPosition(20, 10) ,slides[currSlide].setScale(.515, .545);
                else
                    slides[currSlide].setScale(1, 1);
            }
        }

            /// game ///
            if(curr == 2){
                if(totalScore == 2000){
                    speed -= .2;
                    lvl.setString("lvl 2");
                    lvl.setColor(Color(255, 165, 0));
                }
                if(totalScore == 5000){
                    speed -= .2;
                    lvl.setString("lvl 3");
                    lvl.setColor(Color::Red);
                }

                if(totalScore == 8000){
                    speed -= .3;
                    lvl.setString("lvl 4");
                    lvl.setColor(Color::Black);
                }

                /// the position of the ship ///
                ship.setPosition(ship.getPosition().x, Mouse::getPosition(window).y);

                /// ship position fixing ///
                if (ship.getPosition().y > window.getSize().y - ship.getGlobalBounds().height)
                    ship.setPosition(ship.getPosition().x, window.getSize().y - ship.getGlobalBounds().height);

                if (ship.getPosition().y < 0)
                    ship.setPosition(ship.getPosition().x, 0);


                for (int i = 0; i < int(spawnedAst.size()) and not isOver; i++){
                    spawnedAst[i].move(speed, 0);

                    if (spawnedAst[i].getPosition().x < 0 - spawnedAst[i].getGlobalBounds().width)
                        spawnedAst.erase(spawnedAst.begin() + i), totalScore += 100;
                }

                /// spawning new astroids ///
                if (spawnTimer < 50 and not isOver)
                    spawnTimer++;

                if(spawnTimer >= 50 and not isOver){
                    ast.setPosition(window.getSize().x, rand()%int(window.getSize().y - ast.getGlobalBounds().height));
                    spawnedAst.push_back(ast);
                    spawnTimer = 0;
                }


                /// damage and dec the hp ///
                for (int i = 0; i < int(spawnedAst.size()) and not isOver; i++){
                    if (ship.getGlobalBounds().intersects(spawnedAst[i].getGlobalBounds())){
                        hp--;
                        spawnedAst.erase(spawnedAst.begin() + i);
                        sound.play();
                    }
                }

                if(hp <= 0 and not isOver)
                    isOver = true, gameOverSound.play();

                hpBar.setSize(Vector2f(-(float)hp * 20.f, 20));
            }

            window.clear();

            /// menu display ///
            if(curr == 0){
                window.draw(MenuBack);
                window.draw(teamName);
                menu.disPlay(window);
            }

            /// Ebook display ///
            if(curr == 1){
                window.draw(slides[currSlide]);
            }

            /// game display ///
            if(curr == 2){
                window.draw(gameBackGround);
                score.setString( "Score : " + to_string(totalScore));
                window.draw(score);
                window.draw(hpBar);
                window.draw(avatar);
                window.draw(frame);
                window.draw(lvl);
                window.draw(score);
                window.draw(ship);

                for (int i = 0; i < int(spawnedAst.size()) and not isOver; i++)
                    window.draw(spawnedAst[i]);

                if(isOver)
                    window.draw(gameOver);
            }

            window.display();
        }

        return 0;
    }

