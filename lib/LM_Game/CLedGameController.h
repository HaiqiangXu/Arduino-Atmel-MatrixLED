#include "CLedGame.h"

enum class EGame { Tetris, Snake };

class CLedGameController
{
public:
    // Constructors
    CLedGameController(int csPin, int iNumDevices, int iPinAxisX, int iPinAxisY, int iPinButton, EGame newGame)
    {
        m_currentGame = newGame;
        if (newGame == EGame::Tetris)
        {
            m_ledGame = new CLedGameTetris(csPin, iNumDevices, iPinAxisX, iPinAxisY, iPinButton);
        }
        else if (newGame == EGame::Snake)
        {
            m_ledGame = new CLedGameSnake(csPin, iNumDevices, iPinAxisX, iPinAxisY, iPinButton);
        }
    };

    // Public methods  
    void StartGame()
    {
        m_ledGame->StartGame();
    };

    // Data accessors
    EGame GetGame()
	{
		return m_currentGame;
	};

    void SetGame(EGame newGame)
    {
        m_currentGame = newGame;
    };
private:
    CLedGame* m_ledGame;
    EGame m_currentGame;
};