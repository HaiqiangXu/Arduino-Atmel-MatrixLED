#include <main.h>

#ifdef GAME
#include <CLedGameController.h>
#elif MARQUEE
#include <CLedMarquee.h>
#endif

const int DATA_PIN = 11;    //MOSI [Master Out Slave In]: 11
const int CLK_PIN = 13;     //SCK  [Master Clock]:        13
const int CS_PIN = 10;      //SS   [Slave Select]:        10
const int IN_AXIS_X = 17;   //A3
const int IN_AXIS_Y = 18;   //A4
const int IN_BUTTON = 2;
const int NUM_DEVICES = 3;
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
    Serial.println("Debugging LM Game");
#endif

    m_leds = new CLedGameController(CS_PIN, NUM_DEVICES, IN_AXIS_X, IN_AXIS_Y, IN_BUTTON, EGame::Tetris);
    #elif MARQUEE
#ifdef DEBUG
    Serial.begin(9600);
    Serial.println("Debugging LM Marquee");
#endif

    //Available EMarqueeStyle:   Test  ,  Pacman  ,  Text  ,  BlinkEyes
    m_leds = new CLedMarquee(CS_PIN, NUM_DEVICES, EMarqueeStyle::Pacman);
    if (m_leds->GetMarquee() == EMarqueeStyle::Text)
        m_leds->SetText(TEXT);
    #endif
}

void loop()
{
    #ifdef GAME
    m_leds->StartGame();
    #elif MARQUEE
    m_leds->ShowMarquee();
    //NOTE: Only Test needs this delay. The rest uses their own delay inside them
    if (m_leds->GetMarquee() == EMarqueeStyle::Test)
        delay(500);
    #endif
}