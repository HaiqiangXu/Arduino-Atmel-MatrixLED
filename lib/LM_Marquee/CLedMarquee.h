#include "MD_MAX72xx.h"
#include "TrueRandom.h"
#include "CCommon.h"

const int IN_BTN = 2;
const int DELAYTIME = 100;  // in milliseconds
const int MAX_FRAMES = 4;
const uint8_t PROGMEM pacman[MAX_FRAMES][18] =  // ghost pursued by a pacman. Matrix of 4 rows * 18 cols
{
    { 0xfe, 0x73, 0xfb, 0x7f, 0xf3, 0x7b, 0xfe, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0x7e, 0xff, 0xe7, 0xc3, 0x81, 0x00 },
    { 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0xff, 0xff, 0xe7, 0xe7, 0x42, 0x00 },
    { 0xfe, 0x73, 0xfb, 0x7f, 0xf3, 0x7b, 0xfe, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0xff, 0xff, 0xff, 0xe7, 0x66, 0x24 },
    { 0xfe, 0x7b, 0xf3, 0x7f, 0xf3, 0x7b, 0xfe, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c },
};
const uint8_t DATA_WIDTH = (sizeof(pacman[0]) / sizeof(pacman[0][0]));

enum class EMarqueeStyle { Test, Text, Pacman, BlinkEyes, Shift };

class CLedMarquee
{
public:
    // Constructors
    CLedMarquee(int csPin, int iNumDevices)
    {
        // initialize devices and variables
        pinMode(IN_BTN, INPUT_PULLUP);
        m_leds = new MD_MAX72XX(csPin, iNumDevices);
        m_leds->begin();
        /* Default library's values: control(MD_MAX72XX::INTENSITY, MAX_INTENSITY / 2);
    Half intensity and auto-update:  control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);       */
        m_leds->control(MD_MAX72XX::INTENSITY, MAX_INTENSITY / 4);

        m_currentMarquee = EMarqueeStyle::Text;
        m_iNumDevices = iNumDevices;
    };

    // Public methods
    void ShowMarquee(EMarqueeStyle style);

    // Data accessors
	int GetDevices()
	{
		return m_iNumDevices;
	};
private:
    // Fields
    MD_MAX72XX* m_leds;
    EMarqueeStyle m_currentMarquee;
    int m_iNumDevices;
    
    // Private methods
    void Test();
    void TestAdvanced();
    void TestTransformations();
    void Shift();
    void ShiftTransform(MD_MAX72XX::transformType_t tt, bool bNew);
};