#include "CLedGame.h"

const uint8_t PROGMEM PieceStick = 0b00001000;
const uint8_t PROGMEM PieceSquare[2] = { 0b00001000, 0b00011100 };
const uint8_t PROGMEM PieceTriangle[2] = { 0b00001000, 0b00011100 };
const uint8_t PROGMEM PieceLeftTurn[2] = { 0b00001000, 0b00011100 };
const uint8_t PROGMEM PieceRightTurn[2] = { 0b00001000, 0b00011100 };
const uint8_t PROGMEM PieceLeftGun[2] = { 0b00001000, 0b00011100 };
const uint8_t PROGMEM PieceRightGun[2] = { 0b00001000, 0b00011100 };

// Protected methods
void CLedGameTetris::RefreshAnimation()
{
    m_leds->clear();
    
}

void CLedGameTetris::GameCalculate()
{

}