#include "Board.h"

Board::Board(int width, int height, int mines) {
    this->width = width;
    this->height = height;
    this->mines = mines;
}

Tile& Board::GetTile(int tile) {
    return board.at(tile);
}

int Board::GetSize() const {
    return width * height;
}

void Board::ToggleDebug() {
    debug = !debug;
    if (!gameOver && !victory) {
        for (int i = 0; i < GetSize(); i++) {
            if (debug && board.at(i).GetSecretState() == Tile::SecretState::MINE)
                board.at(i).SetSprite("mine");
            else if (!debug && board.at(i).GetState() == Tile::State::FLAGGED)
                board.at(i).SetSprite("flag");
            else if (!debug && board.at(i).GetSecretState() == Tile::SecretState::MINE)
                board.at(i).SetSprite("tile_hidden");
        }
    }
}

void Board::Initialize() {

    vector<int> randoMines;
    randoMines.push_back(randInt(0, (width * height) - 1));
    while (randoMines.size() < mines) {
        int x = randInt(0,(width * height) - 1);
        bool dup = false;
        for (int i = 0; i < randoMines.size(); i++) {
            if (x == randoMines.at(i)) {
                dup = true;
                break;
            }
        }
        if (!dup)
            randoMines.push_back(x);
    }

    gameOver = false;
    victory = false;
    board.clear();
    mineCount = 0;
    flagCount = 0;

    for (int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            sf::Vector2f position = sf::Vector2f((float)(j) * 32, (float)(i) * 32);
            Tile::SecretState sec_state = Tile::SecretState::EMPTY;
            Tile temp = Tile(position, "tile_hidden", sec_state);
            board.push_back(temp);
        }
    }

    for (int i = 0; i < randoMines.size(); i++) {
        board.at(randoMines.at(i)).SetSecretState(Tile::SecretState::MINE);
        mineCount++;
        if (debug)
            board.at(randoMines.at(i)).SetSprite("mine");
    }

    SetAllNeighbors();
    SetNeighborNumbers();
}

bool Board::GetGameOver() const {
    return gameOver;
}

void Board::SetGameOver() {
    gameOver = true;

    for (int i = 0; i < board.size(); i++) {
        board.at(i).SetClickable(false);
        board.at(i).SetRightClickable(false);
        if (board.at(i).GetSecretState() == Tile::SecretState::MINE) {
            board.at(i).SetSprite("mine");
        }
    }
}

void Board::SetAllNeighbors() {
    for (int i = 0; i < board.size(); i++) {
            // Top Left
        if (i == 0) {
            board.at(i).AddNeighbor(&board.at(1));
            board.at(i).AddNeighbor(&board.at(width));
            board.at(i).AddNeighbor(&board.at(width + 1));
        }
            // Top Right
        else if (i == width - 1) {
            board.at(i).AddNeighbor(&board.at(i - 2));
            board.at(i).AddNeighbor(&board.at(i * 2));
            board.at(i).AddNeighbor(&board.at((i * 2) + 1));
        }
            // Bottom Left
        else if (i == (width * height) - width) {
            board.at(i).AddNeighbor(&board.at(i - width));
            board.at(i).AddNeighbor(&board.at(i - width + 1));
            board.at(i).AddNeighbor(&board.at(i + 1));
        }
            // Bottom Right
        else if (i == (width * height) - 1) {
            board.at(i).AddNeighbor(&board.at(i - width));
            board.at(i).AddNeighbor(&board.at(i - width - 1));
            board.at(i).AddNeighbor(&board.at(i - 1));
        }
            // Top Edge
        else if (i > 0 && i < width - 1) {
            board.at(i).AddNeighbor(&board.at(i - 1));
            board.at(i).AddNeighbor(&board.at(i + 1));
            board.at(i).AddNeighbor(&board.at(i + width - 1));
            board.at(i).AddNeighbor(&board.at(i + width));
            board.at(i).AddNeighbor(&board.at(i + width + 1));
        }
            // Bottom Edge
        else if (i > (width * height) - width && i < (width * height) - 1) {
            board.at(i).AddNeighbor(&board.at(i - width - 1));
            board.at(i).AddNeighbor(&board.at(i - width));
            board.at(i).AddNeighbor(&board.at(i - width + 1));
            board.at(i).AddNeighbor(&board.at(i - 1));
            board.at(i).AddNeighbor(&board.at(i + 1));
        }
            // Left Edge
        else if (i % width == 0) {
            board.at(i).AddNeighbor(&board.at(i + 1));
            board.at(i).AddNeighbor(&board.at(i + 1 - width));
            board.at(i).AddNeighbor(&board.at(i + 1 + width));
            board.at(i).AddNeighbor(&board.at(i + width));
            board.at(i).AddNeighbor(&board.at(i - width));
        }
            // Right Edge
        else if ((i + 1) % width == 0) {
            board.at(i).AddNeighbor(&board.at(i - 1));
            board.at(i).AddNeighbor(&board.at(i - width));
            board.at(i).AddNeighbor(&board.at(i + width));
            board.at(i).AddNeighbor(&board.at(i - 1 - width));
            board.at(i).AddNeighbor(&board.at(i - 1 + width));
        }
            // All Others
        else {
            board.at(i).AddNeighbor(&board.at(i - 1));
            board.at(i).AddNeighbor(&board.at(i - 1 + width));
            board.at(i).AddNeighbor(&board.at(i - 1 - width));
            board.at(i).AddNeighbor(&board.at(i + 1));
            board.at(i).AddNeighbor(&board.at(i + 1 + width));
            board.at(i).AddNeighbor(&board.at(i + 1 - width));
            board.at(i).AddNeighbor(&board.at(i + width));
            board.at(i).AddNeighbor(&board.at(i - width));
        }
    }
}

void Board::SetNeighborNumbers() {
    for (int i = 0; i < board.size(); i++) {

        if (board.at(i).GetSecretState() != Tile::SecretState::MINE) {
            int count = 0;

            for (int j = 0; j < board.at(i).GetNeighborCount(); j++) {
                Tile* t = board.at(i).GetNeighbor(j);
                if (t->GetSecretState() == Tile::SecretState::MINE)
                    count++;
            }

            if (count == 1)
                board.at(i).SetSecretState(Tile::SecretState::ONE);
            else if (count == 2)
                board.at(i).SetSecretState(Tile::SecretState::TWO);
            else if (count == 3)
                board.at(i).SetSecretState(Tile::SecretState::THREE);
            else if (count == 4)
                board.at(i).SetSecretState(Tile::SecretState::FOUR);
            else if (count == 5)
                board.at(i).SetSecretState(Tile::SecretState::FIVE);
            else if (count == 6)
                board.at(i).SetSecretState(Tile::SecretState::SIX);
            else if (count == 7)
                board.at(i).SetSecretState(Tile::SecretState::SEVEN);
            else if (count == 8)
                board.at(i).SetSecretState(Tile::SecretState::EIGHT);
            else
                board.at(i).SetSecretState(Tile::SecretState::EMPTY);
        }
    }
}

void Board::RevealTile(Tile* tile) {
    bool end = tile->Reveal();
    if (end)
        SetGameOver();
}

void Board::ToggleFlag(Tile* tile) {
    tile->ToggleFlag(debug);
}

bool Board::GetVictory() {
    int count = 0;
    for (int i = 0; i < GetSize(); i++) {
        if (board.at(i).GetState() == Tile::State::REVEALED)
            count++;
    }
    if (GetSize() - count - mineCount == 0) {
        for (int i = 0; i < board.size(); i++) {
            board.at(i).SetClickable(false);
            board.at(i).SetRightClickable(false);
            if (board.at(i).GetSecretState() == Tile::SecretState::MINE) {
                board.at(i).SetSprite("flag");
            }
        }
        victory = true;
        return true;
    }
    return false;
}

int Board::GetFlagCount() {
    flagCount = 0;
    for (int i = 0; i < GetSize(); i++) {
        if (board.at(i).GetState() == Tile::State::FLAGGED)
            flagCount++;
    }
    return flagCount;
}

int Board::GetMineCount() {
    return mineCount;
}