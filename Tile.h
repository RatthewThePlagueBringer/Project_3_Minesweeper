#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <map>
#include <random>

#pragma once

using namespace std;

inline unordered_map<string, sf::Texture> textures;

inline void LoadTexture(const char* fileName){
    string filePath = "files/images/";
    filePath += fileName;
    filePath += ".png";

    textures[fileName].loadFromFile(filePath);
}

inline sf::Texture& GetTexture(const char* key){
    if (textures.find(key) == textures.end()) {
        LoadTexture(key);
    }
    return textures[key];
}

inline void Clear(){
    textures.clear();
}

inline mt19937 randomSeed;

inline int randInt(int min, int max){
    std::uniform_int_distribution<int> dist(min, max);
    return dist(randomSeed);
}



class Tile
{
private:
    sf::Vector2f position;
    vector<Tile*> neighbors;
    sf::Sprite sprite;
    bool clickable;
    bool right_clickable;

public:
    enum class State { HIDDEN, REVEALED, FLAGGED };
    State state;
    State GetState() const;
    void SetState(State state_change);
    enum class SecretState { EMPTY, MINE, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, FLAG };
    SecretState secret_state;
    SecretState GetSecretState() const;
    void SetSecretState(SecretState secret_state_change);

    // Constructor
    Tile(sf::Vector2f _position, const char* texture, SecretState sec_state);

    // Accessors
    sf::Sprite& GetSprite();
    Tile* GetNeighbor(unsigned int _neighbor);

    // Mutators
    void SetClickable(bool t_or_f);
    void SetRightClickable(bool t_or_f);
    void SetSprite(const char* texture);
    bool Reveal();
    void ToggleFlag(bool debug);
    int GetNeighborCount();
    void AddNeighbor(Tile* _neighbor);
};