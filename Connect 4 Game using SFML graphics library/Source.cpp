#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <cmath>
#include <random>

using namespace sf;
using namespace std;

const int height = 8;
const int width = 8;
int maxClm[8];
int redCount, yellowCount, counter;
bool yellowSide;
int boardChecker[height][width];
bool yellowWon;
bool redWon;
Texture p1Won;
Texture p2Won;
Texture lose;
Texture win;
Sprite p1Winner;
Sprite p2Winner;
Sprite Losing;
Sprite Winner;



Texture board;
Texture redBall;
Texture yellowBall;
Texture endI;
Sprite endG;
Sprite mainBoard;
Sprite red[32];
Sprite yellow[32];

void set();
void gameContinue();
void yellowPlayer(Vector2i a);

void redPLayer(Vector2i a);
int box[8][8];

void resetGame()
{
   
    // Reset game state and variables to their initial values
    // Reset positions, scores, timers, etc.
    redCount = 0;
    yellowCount = 0;
    counter = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            boardChecker[i][j] = 0;
            for (int i = 0; i < 32; i++) {
                red[i].setPosition(-100, -100);
                yellow[i].setPosition(-100, -100);
            }

        }
    }
}



int main() {
    
    cout << "yellow is play\n";
    endI.loadFromFile("images/end.jpg");
    p1Won.loadFromFile("images/p1Won.jpg");
    p2Won.loadFromFile("images/p2Won.jpg");
    lose.loadFromFile("images/lose.jpg");
    win.loadFromFile("images/win.jpg");
    p1Winner.setTexture(p1Won);
    p2Winner.setTexture(p2Won);
    Losing.setTexture(lose);
    Winner.setTexture(win);
    endG.setTexture(endI);
   
    for (int i = 0; i < 32; i++) {
        red[i].setPosition(-100, -100);
        yellow[i].setPosition(-100, -100);
    }

    RenderWindow window(VideoMode(800, 800), "connect-4", Style::Titlebar | Style::Close);


    set();

    window.setFramerateLimit(120);
    Event e;
    Vector2i pos;
    
    
    while (window.isOpen()) {
        
    
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::R)
            {

                // Restart the game if 'R' key is pressed
                resetGame();
            }
            switch (e.type)
            {
            case Event::Closed:
                window.close();
                break;
            case Event::KeyPressed:
                if (e.key.code == Keyboard::Escape) {
                    window.close();
                }
                break;
            case Event::MouseButtonPressed:
                if (e.mouseButton.button == Mouse::Button::Left) {
                    // cout << "you pressed the right mous" << endl;
                     //
                   // gameContinue();
                    if (yellowWon) {
                        cout << " yellow winner" << endl;
                    }
                    if (counter == 63) {
                        RenderWindow endGame(VideoMode(300, 300), "End Game", Style::Titlebar | Style::Close);
                        while (endGame.isOpen()) {
                            while (endGame.pollEvent(e)) {
                                if (e.type == Event::Closed) {
                                    endGame.close();
                                }
                            }
                            endGame.clear();
                            endGame.draw(endG);
                            endGame.display();
                        }
                      
                    }
                   
                    pos = Mouse::getPosition(window);
                    pos.x /= 100;
                    pos.y /= 100;
                    pos.x++;
                    pos.y++;

                   
                    if (yellowSide) {
                        cout << "red is play" << endl;
                        yellowPlayer(pos);
                        yellowSide = false;
                    }
                    
                    else {
                        cout << "yellow is play" << endl;
                        redPLayer(pos);
                        yellowSide = true;
                    }
                    

                }


            }

        }
        window.clear();
        window.draw(mainBoard);
        for (int i = 0; i < 32; i++) {
            window.draw(red[i]);
            window.draw(yellow[i]);
            //   red[i].setPosition(Vector2f(-100,-100));
              // yellow[i].setPosition(Vector2f(-100, -100));
        }
        window.display();

    }
   

    return 0;
}

void isYellow() {
    Event e;
    if (yellowWon) {
        //cout << "yhhhhhhh" << endl;
        RenderWindow window(VideoMode(300, 300), "End Game", Style::Titlebar | Style::Close);
        while (window.isOpen()) {
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed) {
                    window.close();
                }
            }
            window.clear();
            window.draw(p1Winner);
            window.display();
        }
    }
}

void isRed() {
    Event e;
    if (redWon) {
     
        RenderWindow window(VideoMode(300, 300), "End Game", Style::Titlebar | Style::Close);
        while (window.isOpen()) {
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed) {
                    window.close();
                }
            }
            window.clear();
            window.draw(p2Winner);
            window.display();
        }
    }
}


void yellowPlayer(Vector2i a)
{
    int line = a.x;
    
    
    maxClm[line - 1]++;
    if (maxClm[line - 1] > height) {
        maxClm[line - 1]--;
    }
    for (int i = width - 1; i >= 0; i--)
    {
        if (boardChecker[i][line - 1] == 0)
        {
            boardChecker[i][line - 1] = 1;


            if (yellowCount != 32)
            {
                counter++;
                int x = (line - 1) * 100;
                int y = i * 100;
                Vector2f p(x, y);
              
              
                yellow[yellowCount].setPosition(p);
            }

            yellowCount++;
            break;
        }
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // check yellow row win
            if (boardChecker[i][j] == 1 && boardChecker[i][j - 1] == 1 && boardChecker[i][j - 2] == 1 && boardChecker[i][j - 3] == 1) {
                cout << "yellow won" << endl;
                yellowWon = true;
                isYellow();
               
            }
            if (boardChecker[i][j] == 1 && boardChecker[i - 1][j] == 1 && boardChecker[i - 2][j] == 1 && boardChecker[i - 3][j] == 1) {
                cout << "yellow won" << endl;
                yellowWon = true;
                isYellow();
            }
            if (boardChecker[i][j] == 1 && boardChecker[i - 1][j - 1] == 1 && boardChecker[i - 2][j - 2] == 1 && boardChecker[i - 3][j - 3] == 1) {
                cout << "yellow won" << endl;
                yellowWon = true;
                isYellow();
            }
            if (boardChecker[i][j] == 1 && boardChecker[i - 1][j + 1] == 1 && boardChecker[i - 2][j + 2] == 1 && boardChecker[i - 3][j + 3] == 1) {
                cout << "yellow won" << endl;
                yellowWon = true;
                isYellow();
            }
        }
    }
  
}

void redPLayer(Vector2i a)
{
    int line = a.x;
    maxClm[line - 1]++;
    if (maxClm[line - 1] > height) {
        maxClm[line - 1]--;
    }
    for (int i = width - 1; i >= 0; i--)
    {
        if (boardChecker[i][line - 1] == 0)
        {
            boardChecker[i][line - 1] = 2;


            if (redCount != 32)
            {
                counter++;
                red[redCount].setPosition((line-1)*100, i * 100);
            }

            redCount++;
            break;
        }
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // check yellow row win
            if (boardChecker[i][j] == 2 && boardChecker[i][j - 1] == 2 && boardChecker[i][j - 2] == 2 && boardChecker[i][j - 3] == 2) {
                cout << "red won" << endl;
                redWon = true;
                isRed();
            }
            if (boardChecker[i][j] == 2 && boardChecker[i - 1][j] == 2 && boardChecker[i - 2][j] == 2 && boardChecker[i - 3][j] == 2) {
                cout << "red won" << endl;
                redWon = true;
                isRed();
            }
            if (boardChecker[i][j] == 2 && boardChecker[i - 1][j - 1] == 2 && boardChecker[i - 2][j - 2] == 2 && boardChecker[i - 3][j - 3] == 2) {
                cout << "red won" << endl;
                redWon = true;
                isRed();
            }
            if (boardChecker[i][j] == 2 && boardChecker[i - 1][j + 1] == 2 && boardChecker[i - 2][j + 2] == 2 && boardChecker[i - 3][j + 3] == 2) {
                cout << "red won" << endl;
                redWon = true;
                isRed();
            }
        }
    }
}


void set() {
    board.loadFromFile("images/test.jpg");
    redBall.loadFromFile("images/red.png");
    yellowBall.loadFromFile("images/yellow.png");

    mainBoard.setTexture(board);
    for (int i = 0; i < 32; i++) {
        red[i].setTexture(redBall);
        yellow[i].setTexture(yellowBall);
    }
    redCount = 0;
    yellowCount = 0;
    counter = 0;
    // counter = 0;
    yellowSide = true;
    for (int i = 0; i < height; i++) {
        maxClm[i] = 0;
        // cout << maxClm[i] << endl;
        for (int j = 0; j < width; j++) {
            boardChecker[i][j] = 0;
        }
    }

}


