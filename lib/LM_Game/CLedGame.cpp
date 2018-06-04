#include "CLedGame.h"

#pragma region Public methods

void CLedGame::StartGame()
{
    switch (m_currentGame)
    {
        case EGame::Tetris:
            GameTetris();
            break;

        case EGame::Snake:
            GameSnake();
            break;
        default:
            break;
    }

    if (m_currentTurn == EGameTurn::User)
    {
        ReadUserControls();
    }
}

#pragma endregion

// Private methods
void CLedGame::GameTetris()
{

}

void CLedGame::GameSnake()
{

}

void CLedGame::ReadUserControls()
{
    // Read inputs from user

}