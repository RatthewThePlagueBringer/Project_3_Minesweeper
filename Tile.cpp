#include "Tile.h"

Tile::Tile(sf::Vector2f position, const char* texture, SecretState sec_state) {
    sprite = sf::Sprite(GetTexture(texture));
    sprite.setPosition(position.x, position.y);
    this->position = position;
    state = State::HIDDEN;
    secret_state = sec_state;
    clickable = true;
    right_clickable = true;
}

Tile::State Tile::GetState() const {
    return state;
}

Tile::SecretState Tile::GetSecretState() const {
    return secret_state;
}

void Tile::SetState(State state_change) {
    state = state_change;
}

void Tile::SetSecretState(SecretState secret_state_change) {
    secret_state = secret_state_change;
}

sf::Sprite& Tile::GetSprite() {
    return sprite;
}

void Tile::SetSprite(const char* texture) {
    sprite.setTexture(GetTexture(texture));
    sprite.setPosition(position);
}

bool Tile::Reveal() {

    if (clickable) {
        if (state == State::HIDDEN) {
            state = State::REVEALED;
            if (secret_state == SecretState::MINE) {
                SetSprite("mine");
                return true;
            }
            else if (secret_state == SecretState::ONE) {
                SetSprite("number_1");
                return false;
            }
            else if (secret_state == SecretState::TWO) {
                SetSprite("number_2");
                return false;
            }
            else if (secret_state == SecretState::THREE) {
                SetSprite("number_3");
                return false;
            }
            else if (secret_state == SecretState::FOUR) {
                SetSprite("number_4");
                return false;
            }
            else if (secret_state == SecretState::FIVE) {
                SetSprite("number_5");
                return false;
            }
            else if (secret_state == SecretState::SIX) {
                SetSprite("number_6");
                return false;
            }
            else if (secret_state == SecretState::SEVEN) {
                SetSprite("number_7");
                return false;
            }
            else if (secret_state == SecretState::EIGHT) {
                SetSprite("number_8");
                return false;
            }
            else {
                SetSprite("tile_revealed");
                for (int i = 0; i < GetNeighborCount(); i++)
                    GetNeighbor(i)->Reveal();
                return false;
            }
        }
    }
    return false;
}

void Tile::SetClickable(bool t_or_f) {
    if (t_or_f)
        clickable = true;
    else
        clickable = false;
}

void Tile::SetRightClickable(bool t_or_f) {
    if (t_or_f)
        right_clickable = true;
    else
        right_clickable = false;
}

void Tile::ToggleFlag(bool debug) {
    if (right_clickable) {
        if (state == State::HIDDEN) {
            clickable = false;
            state = State::FLAGGED;
            SetSprite("flag");
        }
        else if (state == State::FLAGGED && debug && secret_state == SecretState::MINE) {
            clickable = true;
            state = State::HIDDEN;
            SetSprite("mine");
        }
        else if (state == State::FLAGGED) {
            clickable = true;
            state = State::HIDDEN;
            SetSprite("tile_hidden");
        }
    }
}

int Tile::GetNeighborCount() {
    return neighbors.size();
}

void Tile::AddNeighbor(Tile* _neighbor) {
    neighbors.push_back(_neighbor);
}

Tile* Tile::GetNeighbor(unsigned int _neighbor) {
    return neighbors.at(_neighbor);
}