#include <main.h>

#ifdef GAME
#include <CLedGameController.h>
#elif MARQUEE
#include <CLedMarquee.h>
#endif

// Type uint8_t is equivalent to unsigned byte/char and it's native/fastest data type for Atmel 8-bit controllers (ATMega 328P). int is signed 16-bit and long signed 32-bit for these MCUs
const uint8_t DATA_PIN = 11;    //MOSI [Master Out Slave In]: 11
const uint8_t CLK_PIN = 13;     //SCK  [Master Clock]:        13
const uint8_t CS_PIN = 10;      //SS   [Slave Select]:        10
const uint8_t IN_AXIS_X = 17;   //A3
const uint8_t IN_AXIS_Y = 18;   //A4
const uint8_t IN_BUTTON = 2;    //WakeUp Input -> press Joystick button if in sleep to wake the game up
const uint8_t NUM_DEVICES = 2;  //number of Matrix leds attached
const char TEXT[] = "Hello world";

#ifdef GAME
CLedGameController* m_leds;
#elif MARQUEE
CLedMarquee* m_leds;
#endif

void setup()
{
#ifdef GAME
    #ifdef DEBUG
        Serial.begin(9600);
        Serial.println("Debugging Led Matrix game");
    #endif

    #ifdef GAME_TETRIS    
        m_leds = new CLedGameController(CS_PIN, NUM_DEVICES, IN_AXIS_X, IN_AXIS_Y, IN_BUTTON, EGame::Tetris);
    #elif GAME_SNAKE
        m_leds = new CLedGameController(CS_PIN, NUM_DEVICES, IN_AXIS_X, IN_AXIS_Y, IN_BUTTON, EGame::Snake);
    #endif

#elif MARQUEE
    #ifdef DEBUG
        Serial.begin(9600);
        Serial.println("Debugging LM Marquee");
    #endif

    #ifdef MARQUEE_TEST
        m_leds = new CLedMarquee(CS_PIN, NUM_DEVICES, EMarqueeStyle::Test);
    #elif MARQUEE_TEXT
        m_leds = new CLedMarquee(CS_PIN, NUM_DEVICES, EMarqueeStyle::Text);
        m_leds->SetText(TEXT);
    #elif MARQUEE_PACMAN
        m_leds = new CLedMarquee(CS_PIN, NUM_DEVICES, EMarqueeStyle::Pacman);
    #elif MARQUEE_BLINK_EYES
        m_leds = new CLedMarquee(CS_PIN, NUM_DEVICES, EMarqueeStyle::BlinkEyes);
    #endif
#endif
}

void loop()
{
#ifdef GAME
    m_leds->StartGame();
#elif MARQUEE
    m_leds->ShowMarquee();
#endif
}