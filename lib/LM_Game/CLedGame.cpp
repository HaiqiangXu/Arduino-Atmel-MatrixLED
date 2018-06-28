#include "CLedGame.h"

#pragma region Public methods

void CLedGame::StartGame()
{
    ReadUserControls();

    // make movement only if Joystick has moved
    if (m_joystick->GetDirectionX() != EDirection::None ||
        m_joystick->GetDirectionY() != EDirection::None ||
        m_iButtonZ == LOW)
    {
        //reset count to PowerDown
        m_lLastTime = millis();
    }
    
    //show animations and calculate game mechanics
    this->RefreshAnimation();
    this->GameCalculate();

    // check if enter Power Down to save battery
    if (millis() - m_lLastTime >= TIME_TO_POWER_DOWN)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        Common.PowerDownInt0();
        digitalWrite(LED_BUILTIN, LOW);
    }
}

#pragma endregion

// Protected methods
void CLedGame::ReadUserControls()
{
    // Read inputs from user
    m_iButtonZ = m_joystick->ReadButton();

    //we don't need values for axis (X, Y) because we just need direction
    m_joystick->ReadAxisX();
	m_joystick->ReadAxisY();
    
    //set changes to corresponding led
    m_lastDirectionX = m_joystick->GetDirectionX();
    m_lastDirectionY = m_joystick->GetDirectionY();
}