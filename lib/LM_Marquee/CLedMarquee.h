#include "MD_MAX72xx.h"
#include "TrueRandom.h"
#include "CCommon.h"
#include "CLedEyes.h"

const int IN_BTN = 2;
const int DELAYTIME = 100;  // in milliseconds

enum class EMarqueeStyle { Test, Text, Pacman, BlinkEyes };

class CLedMarquee
{
public:
    // Constructors
    CLedMarquee(int csPin, int iNumDevices, EMarqueeStyle style)
    {
        // initialize devices and variables
        pinMode(IN_BTN, INPUT_PULLUP);
        m_leds = new MD_MAX72XX(csPin, iNumDevices);
        m_leds->begin();
        /* Default library's values: control(MD_MAX72XX::INTENSITY, MAX_INTENSITY / 2);
    Half intensity and auto-update:  control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);       */
        m_leds->control(MD_MAX72XX::INTENSITY, MAX_INTENSITY / 4);
        m_iNumDevices = iNumDevices;

        m_currentMarquee = style;
        if (m_currentMarquee == EMarqueeStyle::BlinkEyes ||
            m_currentMarquee == EMarqueeStyle::Text)
        {
            m_eyes = new CLedEyes();
            m_eyes->begin(m_leds);
        }
    };

    // Public methods
    void ShowMarquee();

    // Data accessors
	int GetDevices()
	{
		return m_iNumDevices;
	};

    void SetTextToScroll(const char* text)
    {
        strcpy(m_msgText, text);
    };
private:
    // Fields
    MD_MAX72XX* m_leds;
    CLedEyes* m_eyes;
    EMarqueeStyle m_currentMarquee;
    int m_iNumDevices;
    char* m_msgText;
    
    // Private methods
    void TestsOneDevice();
    void TestsAdvanced();
    void TestsTransformations();
    void Shift();
    void ScrollText();
    void ShowPacman();
    void BlinkEyes();
};