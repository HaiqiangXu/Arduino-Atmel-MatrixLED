#ifdef GAME
#include <CLedGameController.h>
#elif MARQUEE
#include <CLedMarquee.h>
#elif TEST_HW
#include <main.h>
#endif

// Type uint8_t is equivalent to unsigned byte/char and it's native/fastest data type for Atmel 8-bit controllers (ATMega 328P). int is signed 16-bit and long signed 32-bit for these MCUs
// const uint8_t CLK_PIN = D13;    //SCK  [Master Clock]:        13
// const uint8_t DATA_PIN = D11;   //MOSI [Master Out Slave In]: 11
#ifdef IS_ATMEL
const uint8_t CS_PIN = 10;      //SS   [Slave Select]:        10
#else
const uint8_t CS_PIN = D10;
#endif
const uint8_t IN_AXIS_X = 17;   //A3
const uint8_t IN_AXIS_Y = 18;   //A4
const uint8_t IN_BUTTON = 2;    //WakeUp Input -> press Joystick button if in sleep to wake the game up
const uint8_t NUM_DEVICES = 2;  //number of Matrix leds attached
const char TEXT[] = "Hello world";

#ifdef GAME
CLedGameController* m_ledsController;
#elif MARQUEE
CLedMarquee* m_ledsController;
#endif

void setup()
{
#ifdef GAME
    #ifdef DEBUG
        Serial.begin(9600);
        Serial.println("Debugging Led Matrix game");
    #endif

    #ifdef GAME_TETRIS    
        m_ledsController = new CLedGameController(CS_PIN, NUM_DEVICES, IN_AXIS_X, IN_AXIS_Y, IN_BUTTON, EGame::Tetris);
    #elif GAME_SNAKE
        m_ledsController = new CLedGameController(CS_PIN, NUM_DEVICES, IN_AXIS_X, IN_AXIS_Y, IN_BUTTON, EGame::Snake);
    #endif
#elif MARQUEE
    #ifdef DEBUG
        Serial.begin(9600);
        Serial.println("Debugging LM Marquee");
    #endif

        m_ledsController = new CLedMarquee(CS_PIN, NUM_DEVICES, EMarqueeStyle::Test);
        m_ledsController->SetText(TEXT);

#endif
}

void loop()
{
#ifdef GAME
    m_ledsController->StartGame();
#elif MARQUEE
    while (true)
    {
        m_ledsController->SetMarqueeStyle(EMarqueeStyle::Test);
        m_ledsController->ShowMarquee();
        delay(300);
        m_ledsController->SetMarqueeStyle(EMarqueeStyle::Text);
        m_ledsController->ShowMarquee();
        delay(300);
        m_ledsController->SetMarqueeStyle(EMarqueeStyle::Pacman);
        m_ledsController->ShowMarquee();
        delay(300);
    }
#endif
}