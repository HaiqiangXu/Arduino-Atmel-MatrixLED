#include "CLedGame.h"

#pragma region Public methods

void CLedGame::StartGame()
{
    if (m_currentTurn == EGameTurn::User)
        ReadUserControls();

    // make movement only if Joystick has moved
    if (m_joystick->GetDirectionX() != EDirection::None ||
        m_joystick->GetDirectionY() != EDirection::None ||
        m_iButtonZ == LOW)
    {
        //reset count to PowerDown
        m_lLastTime = millis();

        //set changes to corresponding led
        m_lastDirectionX = m_joystick->GetDirectionX();
        m_lastDirectionY = m_joystick->GetDirectionY();
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
    }

    // check if enter Power Down to save battery
    if (millis() - m_lLastTime >= TIME_TO_POWER_DOWN)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        Common.PowerDownInt0();
        digitalWrite(LED_BUILTIN, LOW);
    }
}

#pragma endregion

// Private methods
void CLedGame::ReadUserControls()
{
    // Read inputs from user
    m_iButtonZ = m_joystick->ReadButton();
#ifdef DEBUG
    int iX = m_joystick->ReadAxisX();
	int iY = m_joystick->ReadAxisY();
    
    Serial.println("X: " + String(iX) + " Y: " + String(iY) + " Button: " + String(m_iButtonZ) +
                   " DirectionX: " + String(m_joystick->GetDirectionX()) + " DirectionY: " + String(m_joystick->GetDirectionY()));
#else
    //we don't need values for axis (X, Y) because we just need direction
    m_joystick->ReadAxisX();
	m_joystick->ReadAxisY();
#endif    
}