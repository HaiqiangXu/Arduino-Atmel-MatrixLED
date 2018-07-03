#include <MD_MAX72xx.h>
#include <TrueRandom.h>
#include <CCommon.h>
#include <CJoystick.h>

const unsigned long TIME_TO_POWER_DOWN = 60000;     //1 minute
enum class EState { S_LOAD, S_SHOW, S_CALCULATE };

// abstract base class
class CLedGame
{
public:
    CLedGame(int csPin, int iNumDevices, int iPinAxisX, int iPinAxisY, int iPinButton)
    {
        // initialize variables
        m_leds = new MD_MAX72XX(csPin, iNumDevices);
        m_leds->begin();
        m_leds->control(MD_MAX72XX::INTENSITY, MAX_INTENSITY / 4);
        m_iNumDevices = iNumDevices;

        m_joystick = new CJoystick(iPinAxisX, iPinAxisY, iPinButton);
        m_lastDirectionX = EDirection::None;
        m_lastDirectionY = EDirection::None;
        m_iCurrentLevel = 0;
        m_lLastTime = millis();
    };
    void StartGame();

    // Data accessors
	int GetNumDevices()
	{
		return m_iNumDevices;
	};
protected:
    // Fields
    MD_MAX72XX* m_leds;
    CJoystick* m_joystick;
    int m_iNumDevices;          //number of Matrix leds attached
    EDirection m_lastDirectionX, m_lastDirectionY;
    int m_iButtonZ;
    int m_iCurrentLevel;
    unsigned long m_lLastTime;

    // Protected methods
    void ReadUserControls();
    virtual void RefreshAnimation() = 0;   //pure virtual function -> to be defined in the derived classes
    virtual void GameCalculate() = 0;
};

// derived class for Tetris game
class CLedGameTetris : public CLedGame
{
public:
    CLedGameTetris(int csPin, int iNumDevices, int iPinAxisX, int iPinAxisY, int iPinButton) : CLedGame(csPin, iNumDevices, iPinAxisX, iPinAxisY, iPinButton)
    {
        m_leds->setBuffer(1, sizeof(Pieces[0]), Pieces[0]);
        m_leds->setBuffer(4, sizeof(Pieces[0]), Pieces[1]);
        // m_leds->transform(4, MD_MAX72XX::TRC);
        m_leds->setBuffer(7, sizeof(Pieces[0]), Pieces[2]);
        m_leds->setBuffer(10, sizeof(Pieces[0]), Pieces[3]);
        m_leds->setBuffer(13, sizeof(Pieces[0]), Pieces[4]);
        m_leds->setBuffer(16, sizeof(Pieces[0]), Pieces[5]);
        m_leds->setBuffer(20, sizeof(PieceStick), PieceStick);
        m_state = EState::S_LOAD;
    };

private:
    // fields
    const uint8_t PROGMEM Pieces[6][2] = 
    {
        {0b00011000, 0b00011000},       //PieceSquare
        {0b00010000, 0b00111000},       //PieceTriangle
        {0b00011000, 0b00110000},       //PieceLeftTurn
        {0b00110000, 0b00011000},       //PieceRightTurn
        {0b00001000, 0b00111000},       //PieceLeftGun
        {0b00100000, 0b00111000}        //PieceRightGun
    };
    const uint8_t PROGMEM PieceStick = 0b00111100;
    
    EState m_state;
    uint8_t m_currentPiece[2];

    // private methods
    virtual void RefreshAnimation();
    virtual void GameCalculate();

};

// derived class for Snake game
class CLedGameSnake : public CLedGame
{
public:
    CLedGameSnake(int csPin, int iNumDevices, int iPinAxisX, int iPinAxisY, int iPinButton) : CLedGame(csPin, iNumDevices, iPinAxisX, iPinAxisY, iPinButton)
    { };

private:    
    virtual void RefreshAnimation();
    virtual void GameCalculate();
};