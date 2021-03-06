#include "Player.hpp"

Player::Player() {
}

Player::~Player() {

}

void Player::Init(SpriteAnimation * spriteAnimation) {
    _spriteAnimation = spriteAnimation;
}

void Player::SetPosition(int x, int y) {
    _x = x;
    _y = y;
}

void Player::Draw() {

    _spriteAnimation->Draw(_x,_y);
}

void Player::Update(float delta) {
    if(_jump) {
        if(_jumpStateUp) {
            _y-=delta;
            if(_y<=_jumpPosStart-_jumpSize) {
                _jumpStateUp = false;
            }
        } else {
            _y+=delta;
            if(_y>=_jumpPosStart) {
                _jump = false;
            }
        }
    }
}

void Player::ToRight(float delta) {
    _x+=delta;
    _spriteAnimation->Step(delta);
}

void Player::ToLeft(float delta) {
    _x-=delta;
    _spriteAnimation->Step(delta);
}

void Player::Jump(int delta) {
    if(_jump) {
        return;
    }
    _jumpStateUp = true;
    _jump = true;
    _jumpSize = delta;
    _jumpPosStart = _y;
}
