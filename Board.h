#include <vector>
#include "Tile.h"

#pragma once

using namespace std;

class Board
{
private:
    int width;
    int height;
    int mines;
    int mineCount = 0;
    int flagCount = 0;
    vector<Tile> board;
    bool debug = false;
    bool gameOver = false;
    bool victory = false;
    
public:
    // Constructor
    Board(int width, int height, int mines);
    
    void Initialize();

    //Accessors
    Tile& GetTile(int tile);
    int GetSize() const;
    bool GetGameOver() const;
    bool GetVictory();
    int GetFlagCount();
    int GetMineCount();

    //Mutators
    void SetAllNeighbors();
    void SetNeighborNumbers();
    
    void ToggleDebug();
    void SetGameOver();
    void RevealTile(Tile* tile);
    void ToggleFlag(Tile* tile);
};
