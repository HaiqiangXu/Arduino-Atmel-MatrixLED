#include <MD_MAX72xx.h>
#include <TrueRandom.h>
#include <CCommon.h>
#include <CJoystick.h>

const unsigned long TIME_TO_POWER_DOWN = 60000;     //1 minute

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
    { };

private:    
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