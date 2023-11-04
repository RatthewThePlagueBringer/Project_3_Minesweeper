#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <sstream>

#include "Board.h"

using namespace std;

//Function for changing numbers to sprites
vector<int> counterDigitizer(int number){
    vector<int> counterNums;

    if (number < -99) {
        counterNums.push_back(10);
        counterNums.push_back(9);
        counterNums.push_back(9);
    }
    else if (number < -9) {
        counterNums.push_back(10);
        int temp = (number * -1) / 10;
        counterNums.push_back(temp);
        temp = (number * -1) % 10;
        counterNums.push_back(temp);
    }
    else if (number < 0) {
        counterNums.push_back(10);
        counterNums.push_back(0);
        counterNums.push_back(number * -1);
    }
    else if (number < 10) {
        counterNums.push_back(0);
        counterNums.push_back(0);
        counterNums.push_back(number);
    }
    else if (number < 100) {
        counterNums.push_back(0);
        int temp = number / 10;
        counterNums.push_back(temp);
        temp = number % 10;
        counterNums.push_back(temp);
    }
    else if (number < 1000) {
        int temp = number / 100;
        counterNums.push_back(temp);
        temp = number % 100;
        temp /= 10;
        counterNums.push_back(temp);
        temp = number % 10;
        counterNums.push_back(temp);
    }
    else {
        counterNums.push_back(9);
        counterNums.push_back(9);
        counterNums.push_back(9);
    }
    return counterNums;
}

vector<int> timerDigitizer(int number){
    vector<int> timerNums;

    if (number < 10) {
        timerNums.push_back(0);
        timerNums.push_back(number);
    }
    else if (number < 60) {
        timerNums.push_back(number/10);
        timerNums.push_back(number%10);
    }
    else {
        timerNums.push_back((number%60)/10);
        timerNums.push_back((number%60)%10);
    }
    return timerNums;
}

int timeToInt(string time){
    int mins = stoi(time.substr(0,2));
    int secs = stoi(time.substr(3,2));
    return (60*mins) + secs;
}

string intToTime(int num){
    int mins = num / 60;
    int secs = num % 60;
    return to_string(mins) + ":" + to_string(secs);
}

void displayLB(int columns, int rows, int newMin, int newSec, string username){

    float width = 16*columns;
    float height = (rows*16)+50;
    sf::RenderWindow leaderBoard(sf::VideoMode(width, height), "Leaderboard");
    sf::RectangleShape background(sf::Vector2f(width, height));
    background.setFillColor(sf::Color::Blue);

    sf::Font font;
    font.loadFromFile("files/font.ttf");

    auto title = sf::Text{ "LEADERBOARD", font, 20};
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect titleRect = title.getLocalBounds();
    title.setOrigin(titleRect.left + titleRect.width/2.0f,titleRect.top  + titleRect.height/2.0f);
    title.setPosition(sf::Vector2f(width/2.0f,(height/2.0f)-120));

    auto people = sf::Text{ "Placeholder", font, 18};
    people.setStyle(sf::Text::Bold);
    sf::FloatRect textRect = people.getLocalBounds();
    people.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    people.setPosition(sf::Vector2f(width/2.0f,(height/2.0f)+20));

    string input;


    vector<int> intTimes;
    vector<string> strTimes;
    vector<string> names;
    map<string, string> lb;

    string time;
    string name;
    ifstream file("files/leaderboard.txt");
    string line;
    while (getline(file,line)){
        stringstream stream(line);
        string item;
        getline(stream, item, ',');
        time = item;
        getline(stream, item, ',');
        name = item;
        intTimes.push_back(timeToInt(time));
        names.push_back(name);
        lb.emplace(time,name);
    }


//    int newScore = (newMin * 60) + newSec;
//    intTimes.push_back(newScore);
//    names.push_back(username);
//
//    for (int i = 5; i >= 0; i--){
//        if (intTimes.at(i) < intTimes.at(i-1)){
//            int temp = intTimes.at(i-1);
//            intTimes.at(i-1) = intTimes.at(i);
//            intTimes.at(i) = temp;
//        }
//    }
//
//    for (int i = 0; i < 6; i++){
//        cout << intTimes.at(i) << endl;
//    }
//
//    lb1.erase(lb1.find(intToTime(intTimes.at(5))));

    map<string,string>::iterator iter;
    int i = 1;
    for (iter = lb.begin(); iter != lb.end(); iter++){
        input += to_string(i) + ".\t" + iter->first + "\t" + iter->second + "\n\n";
        i++;
    }

    people.setString(input);

    while(leaderBoard.isOpen()){

        sf::Event event;
        while (leaderBoard.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                leaderBoard.close();
        }

        leaderBoard.clear();
        leaderBoard.draw(background);
        leaderBoard.draw(title);
        leaderBoard.draw(people);
        leaderBoard.display();
    }
}

int main()
{

    //Reading CFG Files

    int columns;
    int rows;
    int mines;
    ifstream cfg("files/board_config.cfg");
    if (cfg.is_open()) {
        cfg >> columns;
        cfg >> rows;
        cfg >> mines;
    }
    else {
        cout << "Config file not found, using default values" << endl;
        columns = 25;
        rows = 16;
        mines = 50;
    }
    cfg.close();

    ////////////////////////////////////////WELCOME SCREEN//////////////////////////////////////////////////////////////
    int gamestage = 1;

    float width = columns * 32.f;
    float height = rows * 32.f + 100;

    sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Minesweeper",sf::Style::Close | sf::Style::Titlebar);


    sf::RectangleShape background(sf::Vector2f(width, height));
    background.setFillColor(sf::Color::Blue);

    sf::Font font;
    font.loadFromFile("files/font.ttf");

    auto welcome = sf::Text{ "WELCOME TO MINESWEEPER!", font, 24};
    welcome.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect textRect = welcome.getLocalBounds();
    welcome.setOrigin(textRect.left + textRect.width/2.0f,textRect.top  + textRect.height/2.0f);
    welcome.setPosition(sf::Vector2f(width/2.0f,height/2.0f-150));

    auto inputPrompt = sf::Text{ "Enter your name:", font, 20};
    inputPrompt.setStyle(sf::Text::Bold);
    sf::FloatRect promptRect = inputPrompt.getLocalBounds();
    inputPrompt.setOrigin(promptRect.left + promptRect.width/2.0f,promptRect.top  + promptRect.height/2.0f);
    inputPrompt.setPosition(sf::Vector2f(width/2.0f,height/2.0f-75));

    auto inputText = sf::Text{ "|", font, 18};
    inputText.setStyle(sf::Text::Bold);
    inputText.setFillColor(sf::Color::Yellow);

    string name;

    /////////////////////////////GAME SCREEN///////////////////////////////

    bool isPause = false;
    bool timerToggle = true;
    auto start = chrono::high_resolution_clock::now();
    auto hold = chrono::high_resolution_clock::now();
    auto pauseStart = chrono::high_resolution_clock::now();

    bool lbControl = false;

    Board board = Board(columns, rows, mines);
    board.Initialize();

    Board backup = board;

    sf::Sprite smiley(GetTexture("face_happy"));
    smiley.setPosition((columns / 2.0) * 32 - 32, 32 * (rows + 0.5f));

    sf::Sprite debugButton(GetTexture("debug"));
    debugButton.setPosition((columns * 32) - 304, 32 * (rows + 0.5f));

    sf::Sprite pauseButton(GetTexture("pause"));
    pauseButton.setPosition((columns * 32) - 240, 32 * (rows + 0.5f));

    sf::Sprite leaderButton(GetTexture("leaderboard"));
    leaderButton.setPosition((columns * 32) - 176, 32 * (rows + 0.5f));

    sf::Texture digitTexture = GetTexture("digits");
    vector<sf::IntRect> digitRects;
    for (int i = 0; i < 211; i += 21)
        digitRects.push_back(sf::IntRect(i, 0, 21, 32));

    //Mine Counter
    vector<int> counterNums = counterDigitizer(board.GetMineCount());
    vector<sf::Sprite> counter;
    for (int i = 0, u = 33; i < 3; i++) {
        counter.push_back(sf::Sprite(digitTexture));
        counter.at(i).setPosition(u, 32 * (rows + 0.5f)+16);
        u += 21;
        counter.at(i).setTextureRect(digitRects.at(counterNums.at(i)));
    }

    //Timer
    //Minutes
    vector<int> timerNumsM = timerDigitizer(00);
    vector<sf::Sprite> timerM;
    for (int i = 0, u = ((columns * 32) - 97); i < 2; i++) {
        timerM.push_back(sf::Sprite(digitTexture));
        timerM.at(i).setPosition(u, 32 * (rows + 0.5f)+16);
        u += 21;
        timerM.at(i).setTextureRect(digitRects.at(timerNumsM.at(i)));
    }
    //Seconds
    vector<int> timerNumsS = timerDigitizer(00);
    vector<sf::Sprite> timerS;
    for (int i = 0, u = ((columns * 32) - 54); i < 2; i++) {
        timerS.push_back(sf::Sprite(digitTexture));
        timerS.at(i).setPosition(u, 32 * (rows + 0.5f)+16);
        u += 21;
        timerS.at(i).setTextureRect(digitRects.at(timerNumsS.at(i)));
    }

    //Detects if the gameWindow is open
    while (gameWindow.isOpen())
    {
        auto time = chrono::high_resolution_clock::now() - start;


        if (!timerToggle){
            time = hold - start;
        }

        int mins = chrono::duration_cast<chrono::minutes>(time).count();

        int secs = chrono::duration_cast<chrono::seconds>(time).count();

        timerNumsM = timerDigitizer(mins);
        for (int i = 0; i < 2; i++){
            timerM.at(i).setTextureRect(digitRects.at(timerNumsM.at(i)));
        }

        timerNumsS = timerDigitizer(secs);
        for (int i = 0; i < 2; i++){
            timerS.at(i).setTextureRect(digitRects.at(timerNumsS.at(i)));
        }

        //While gameWindow is open, there is an Event object called event
        sf::Event event;
        while (gameWindow.pollEvent(event)) {

            //Everything will go here

            //Updates the textbox bounds so it constantly stays centered
            sf::FloatRect inputRect = inputText.getLocalBounds();
            inputText.setOrigin(inputRect.left + inputRect.width/2.0f,inputRect.top  + inputRect.height/2.0f);
            inputText.setPosition(sf::Vector2f(width/2.0f,height/2.0f-45));

            //If text is entered while in gamestage 1
            if (event.type == sf::Event::TextEntered && gamestage == 1) {
                //Checking ASCII conditions
                if (((event.text.unicode > 64 && event.text.unicode < 91) || (event.text.unicode > 96 && event.text.unicode < 123)) && (name.length() < 10)) {
                    if (name.empty() && (event.text.unicode > 96 && event.text.unicode < 123)){
                        name += static_cast<char>(event.text.unicode-32);
                        inputText.setString(name+ "|");
                    }
                    else if (!name.empty() && (event.text.unicode > 64 && event.text.unicode < 91)){
                        name += static_cast<char>(event.text.unicode+32);
                        inputText.setString(name + "|");
                    }
                    else {
                        name += static_cast<char>(event.text.unicode);
                        inputText.setString(name + "|");
                    }
                }
            }

            //If a key is pressed
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    gameWindow.close();
                if (gamestage == 1){
                    if ((event.key.code == sf::Keyboard::Enter) && (!name.empty())) {
                        gamestage = 2;
                        start = chrono::high_resolution_clock::now();
                        background.setFillColor(sf::Color(200, 200, 200));
                    }
                    if ((event.key.code == sf::Keyboard::Backspace) && (!name.empty())) {
                        name.erase(name.end() - 1);
                        inputText.setString(name + "|");
                    }
                }
            }

            //If mouse is pressed while in gamestage 2 or 3
            if (event.type == sf::Event::MouseButtonPressed && gamestage != 1){

                //Left Click
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i position = sf::Mouse::getPosition(gameWindow);

                    //Tile reveal
                    if (position.y < (rows * 32) && !isPause && gamestage !=3) {
                        Tile* current = &board.GetTile( columns * (position.y / 32) + (position.x / 32));
                        board.RevealTile(current);
                        board.GetGameOver();
                        if (board.GetVictory()){
                            smiley.setTexture(GetTexture("face_win"));
                            gamestage = 3;
                            timerToggle = false;
                            for (int i = 0; i < 3; i++){
                                counterNums.at(i) = 0;
                                counter.at(i).setTextureRect(digitRects.at(counterNums.at(i)));
                            }
                        }
                        if (board.GetGameOver()){
                            smiley.setTexture(GetTexture("face_lose"));
                            gamestage = 3;
                            timerToggle = false;
                        }
                    }

                    //Debug Button
                    else if (((position.x > (columns * 32) - 304 && position.x < (columns * 32) - 239) && (position.y > 32 * (rows + 0.5f) && position.y < 32 * (rows + 0.5f) + 64)) && !isPause && gamestage !=3){
                        board.ToggleDebug();
                    }

                    //Pause Button
                    else if ((position.x > (columns * 32) - 240 && position.x < (columns * 32) - 176) && (position.y > 32 * (rows + 0.5f) && position.y < 32 * (rows + 0.5f)+64) && gamestage !=3){
                        isPause = !isPause;
                        if (!isPause) {
                            gamestage = 3;
                        }
                        if (!board.GetGameOver() && !board.GetVictory() && isPause){
                            gamestage = 3;
                        }
                    }

                    //Leaderboard Button
                    else if ((position.x > (columns * 32) - 176 && position.x < (columns * 32) - 112) && (position.y > 32 * (rows + 0.5f) && position.y < 32 * (rows + 0.5f)+64)){
                        isPause = !isPause;
                        if (!isPause) {
                            gamestage = 3;
                        }
                        if (!board.GetGameOver() && !board.GetVictory() && isPause){
                            gamestage = 3;
                        }
                        lbControl = true;
                    }

                    //Smiley Button
                    else if ((position.x > (columns / 2.0) * 32 - 32 && position.x < (columns / 2.0) * 32 + 32) && (position.y > 32 * (rows + 0.5f) && position.y < 32 * (rows + 0.5f) + 64)) {
                        timerToggle = true;
                        gamestage = 2;
                        board.Initialize();
                        backup = board;
                        smiley.setTexture(GetTexture("face_happy"));
                        counterNums = counterDigitizer(board.GetMineCount() - board.GetFlagCount());
                        for (int i = 0; i < 3; i++){
                            counter.at(i).setTextureRect(digitRects.at(counterNums.at(i)));
                        }
                        start = chrono::high_resolution_clock::now();
                    }
                }
                //Right Click
                else if (event.mouseButton.button == sf::Mouse::Right){
                    sf::Vector2i position = sf::Mouse::getPosition(gameWindow);

                    //Flags
                    if (position.y < (rows * 32) && !isPause && gamestage !=3){
                        Tile* current = &board.GetTile(columns * (position.y / 32) + (position.x / 32));
                        board.ToggleFlag(current);
                        counterNums = counterDigitizer(board.GetMineCount() - board.GetFlagCount());
                        for (int i = 0; i < 3; i++){
                            counter.at(i).setTextureRect(digitRects.at(counterNums.at(i)));
                        }
                    }
                }
            }
            //If the event closes, close the gameWindow as well
            if (event.type == sf::Event::Closed)
                gameWindow.close();
        }
        //Clears the old things from the gameWindow
        gameWindow.clear();

        //Draw the new things onto the gameWindow
        gameWindow.draw(background);

        //Welcome Screen
        if (gamestage == 1){
            gameWindow.draw(welcome);
            gameWindow.draw(inputPrompt);
            gameWindow.draw(inputText);
        }

        //Game Screen
        if (gamestage == 2 || gamestage == 3){
            gameWindow.draw(debugButton);
            gameWindow.draw(pauseButton);
            gameWindow.draw(leaderButton);
            gameWindow.draw(smiley);
            for (int i = 0; i < 3; i++){
                gameWindow.draw(counter.at(i));
            }
            for (int i = 0; i < 2; i++){
                gameWindow.draw(timerM.at(i));
                gameWindow.draw(timerS.at(i));
            }

            for (int i = 0; i < board.GetSize(); i++){
                gameWindow.draw(board.GetTile(i).GetSprite());
            }
        }

        //Paused Game
        if (gamestage == 3){
            hold = chrono::high_resolution_clock::now();
            if (isPause){
                backup = board;
                for (int i = 0; i < board.GetSize(); i++){
                    board.GetTile(i).GetSprite().setTexture(GetTexture("tile_revealed"));
                }
                pauseStart = chrono::high_resolution_clock::now();
                pauseButton.setTexture(GetTexture("play"));
                timerToggle = false;
            }
            else if (!isPause && !board.GetGameOver() && !board.GetVictory()){
                board = backup;
                pauseButton.setTexture(GetTexture("pause"));
                timerToggle = true;
                start = start + (chrono::high_resolution_clock::now() - pauseStart);
            }
            gamestage = 2;
        }
        
        //Display the new gameWindow
        gameWindow.display();
        
        if (lbControl){
            displayLB(columns, rows, mins, secs, name);
            lbControl = false;
        }
    }
    Clear();

    return 0;
}