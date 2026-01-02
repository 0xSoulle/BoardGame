#include "player.h"

Player::Player() {
    this->name = "";
    this->position = 1;
    this->freezeTime = 0;
}

Player::Player(std::string name) {
    this->name = name;
    this->position = 1;
    this->freezeTime = 0;
}

std::string Player::getName() {
    return name;
}

void Player::move(int dice) {
    position += dice;
    if (position < 1) position = 1;
    if (position > 90) position = 90;
}

int Player::getPosition() {
    return position;
}

int Player::getFreeze() {
    return freezeTime;
}

void Player::freeze() {
    freezeTime = 1;
}

void Player::reduceFreeze() {
    if (freezeTime > 0) {
        freezeTime--;
    }
}

void Player::kill() {
    freezeTime = -1;
}

bool Player::isAlive() {
    return freezeTime != -1;
}