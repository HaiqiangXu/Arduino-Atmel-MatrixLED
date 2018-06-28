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
const char TEXT[] = "Lorem Ipsum es simplemente el texto de relleno de las imprentas y archivos de texto. Lorem Ipsum ha sido el texto de relleno estándar de las industrias desde el año 1500, cuando un impresor (N. del T. persona que se dedica a la imprenta) desconocido usó una galería de textos y los mezcló de tal manera que logró hacer un libro de textos especimen. No sólo sobrevivió 500 años, sino que tambien ingresó como texto de relleno en documentos electrónicos, quedando esencialmente igual al original. Fue popularizado en los 60s con la creación de las hojas Letraset, las cuales contenian pasajes de Lorem Ipsum, y más recientemente con software de autoedición, como por ejemplo Aldus PageMaker, el cual incluye versiones de Lorem Ipsum";

#ifdef GAME
CLedGameController* m_leds;
#elif MARQUEE
CLedMarquee* m_leds;
#endif

void setup()
{
    #ifdef GAME
    m_leds = new CLedGameController(CS_PIN, NUM_DEVICES, IN_AXIS_X, IN_AXIS_Y, IN_BUTTON, EGame::Tetris);
    #elif MARQUEE
    //EMarqueeStyle:   Test  ,  Pacman  ,  Text  ,  BlinkEyes
    m_leds = new CLedMarquee(CS_PIN, NUM_DEVICES, EMarqueeStyle::Text);
    m_leds->SetTextToScroll(TEXT);
    #endif
}

void loop()
{
    #ifdef GAME
    m_leds->StartGame();
    #elif MARQUEE
    m_leds->ShowMarquee();
    delay(500);
    #endif
}