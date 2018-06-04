#include <MD_MAX72xx.h>
#include <CJoystick.h>

enum class EGame { Tetris, Snake };
enum class EGameTurn { Computer, User };

class CLedGame
{
public:
    // Constructors
    CLedGame(int csPin, int iNumDevices, int iPinAxisX, int iPinAxisY, int iPinButton)
    {
        // initialize variables
        m_leds = new MD_MAX72XX(csPin, iNumDevices);
        m_leds->begin();

        m_Joystick = new CJoystick(iPinAxisX, iPinAxisY, iPinButton);
        m_iNumDevices = iNumDevices;
        m_currentGame = EGame::Tetris;
        m_currentTurn = EGameTurn::Computer;
    };

    // Public methods
    void SetGame(EGame newGame)
    {
        m_currentGame = newGame;
    };

    void StartGame();

    // Data accessors
	int GetNumDevices()
	{
		return m_iNumDevices;
	};
private:
    // Fields
    MD_MAX72XX* m_leds;
    CJoystick* m_Joystick;
    int m_iNumDevices;
    EGame m_currentGame;
    EGameTurn m_currentTurn;

    // Private methods
    void GameTetris();
    void GameSnake();
    void ReadUserControls();
};