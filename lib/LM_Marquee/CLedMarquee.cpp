#include "CLedMarquee.h"

#pragma region Public methods

void CLedMarquee::ShowMarquee()
{
    switch(m_currentMarquee)
    {
        case EMarqueeStyle::Test:
            if (m_iNumDevices == 1)
                TestsOneDevice();
            TestsAdvanced();
            TestsTransformations();
            Shift();
            break;

        case EMarqueeStyle::Text:
            this->ScrollText();
            break;

        case EMarqueeStyle::Pacman:
            this->ShowPacman();
            break;

        case EMarqueeStyle::BlinkEyes:
            // limited to 2 devices
            this->BlinkEyes();
            break;

        default:
            break;
    }
}

#pragma endregion

#pragma region Private methods

void CLedMarquee::TestsOneDevice()
{
    int i, j;

    /// Demonstrates the use of setPoint() and show where the zero point is in the display
    m_leds->clear();
    for (i = 0; i < ROW_SIZE; i++)
    {
        m_leds->setPoint(i, i, true);
        m_leds->setPoint(0, i, true);
        m_leds->setPoint(i, 0, true);
        m_leds->setPoint(i, ROW_SIZE - i, true);
        m_leds->setPoint(ROW_SIZE - i, i, true);
        Common.Sleep(DELAYTIME);
    }

    /// Demonstrate the use of drawHLine(). Another way of drawing a line different to setRow()/setColumn().
    m_leds->clear();
    for (i = 0; i < m_leds->getColumnCount(); i++)  // getColumnCount() = (maxDevices * COL_SIZE)
    {
        m_leds->drawHLine(i, i, m_leds->getColumnCount() - ROW_SIZE + i, true);
        delay(2 * DELAYTIME);
    }

    // Demonstrate the use of drawVLine().
    m_leds->clear();
    for (i = 0; i < m_leds->getColumnCount(); i++)
    {
        m_leds->drawVLine(i, 0, i % ROW_SIZE, true);
        delay(2 * DELAYTIME);
    }

    /// Demonstrates the use of setRow()
    m_leds->clear();
    for (i = 0; i < ROW_SIZE; i++)
    {
        m_leds->setRow(i, 0b01101101);  //the 'value' parameter represents the pixel of the line that will be switched on/off
        delay(2 * DELAYTIME);
        m_leds->setRow(i, 0x00);
        delay(2 * DELAYTIME);
    }

    /// Demonstrates the use of setColumn()
    m_leds->clear();
    for (i = 0; i < m_leds->getColumnCount(); i++)
    {
        m_leds->setColumn(i, 0xff);
        delay(2 * DELAYTIME);
        m_leds->setColumn(i, 0x00);
    }

    /// nested rectangles spanning the entire display
    m_leds->clear();
    for (i = 0; i < 2; i++)     //loop 2 times
    {
        // cumulate rectangles from outside to inside
        for (j = 0; j < ROW_SIZE / 2; j++)
        {
            m_leds->drawRectangle(j, j, ROW_SIZE - 1 - j, m_leds->getColumnCount() - 1 - j, true);
            delay(2 * DELAYTIME);
        }
        // clear rectangles from outside to inside
        for (j = 0; j < ROW_SIZE / 2; j++)
        {
            m_leds->drawRectangle(j, j, ROW_SIZE - 1 - j, m_leds->getColumnCount() - 1 - j, false);
            delay(2 * DELAYTIME);
        }
    }

    /// Run through display of the the entire font ASCII characters set
    for (i = 0; i < 128; i++)
    {
        m_leds->clear();
        //FC-16 board's column goes from R to L instead of normal way L to R 
        m_leds->setChar((COL_SIZE) / 2 + (m_leds->getMaxFontWidth() / 3), i);
        delay(DELAYTIME * 3);
    }
}

void CLedMarquee::TestsAdvanced()
{
    int i, j, n;
  
    /// Combination of setRow() and setColumn() to form a cross in movement
    m_leds->clear();
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);   //equivalent to m_leds->update(MD_MAX72XX::OFF)
    for (i = 0; i < ROW_SIZE; i++)      // diagonally down the display R to L
    {
        for (j = 0; j < m_iNumDevices; j++)
        {
            m_leds->setColumn(j, i, 0xff);
            m_leds->setRow(j, i, 0xff);
        }
        m_leds->update();               // update manually as control(MD_MAX72XX::UPDATE) is switched off
        delay(2 * DELAYTIME);
        for (j = 0; j < m_iNumDevices; j++)
        {
            m_leds->setColumn(j, i, 0x00);
            m_leds->setRow(j, i, 0x00);
        }
    }

    for (i = ROW_SIZE - 1; i >= 0; i--) // moving up the display on the R
    {
        for (j = 0; j < m_iNumDevices; j++)
        {
            m_leds->setColumn(j, i, 0xff);
            m_leds->setRow(j, ROW_SIZE - 1, 0xff);
        }
        m_leds->update();
        delay(2 * DELAYTIME);
        for (j = 0; j < m_iNumDevices; j++)
        {
            m_leds->setColumn(j, i, 0x00);
            m_leds->setRow(j, ROW_SIZE - 1, 0x00);
        }
    }

    for (i = 0; i < ROW_SIZE; i++)      // diagonally up the display L to R
    {
        for (j = 0; j < m_iNumDevices; j++)
        {
            m_leds->setColumn(j, i, 0xff);
            m_leds->setRow(j, ROW_SIZE - 1 - i, 0xff);
        }
        m_leds->update();
        delay(3 * DELAYTIME);
        for (j = 0; j < m_iNumDevices; j++)
        {
            m_leds->setColumn(j, i, 0x00);
            m_leds->setRow(j, ROW_SIZE - 1 - i, 0x00);
        }
    }
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

    /// Bullseye. Demonstrate the use of buffer based repeated patterns across all devices.
    m_leds->clear();
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
    for (n = 0; n < 2; n++)     //loop 2 times
    {
        byte b = 0xff;
        i = 0;

        while (b != 0x00)
        {
            for (j = 0; j < m_iNumDevices + 1; j++)
            {
                m_leds->setRow(j, i, b);
                m_leds->setColumn(j, i, b);
                m_leds->setRow(j, ROW_SIZE - 1 - i, b);
                m_leds->setColumn(j, COL_SIZE - 1 - i, b);
            }
            m_leds->update();
            delay(2 * DELAYTIME);
            for (j = 0; j < m_iNumDevices + 1; j++)
            {
                m_leds->setRow(j, i, 0);
                m_leds->setColumn(j, i, 0);
                m_leds->setRow(j, ROW_SIZE - 1 - i, 0);
                m_leds->setColumn(j, COL_SIZE - 1 - i, 0);
            }

            bitClear(b, i);
            bitClear(b, 7 - i);
            i++;
        }

        while (b != 0xff)
        {
            for (j = 0; j < m_iNumDevices + 1; j++)
            {
                m_leds->setRow(j, i, b);
                m_leds->setColumn(j, i, b);
                m_leds->setRow(j, ROW_SIZE - 1 - i, b);
                m_leds->setColumn(j, COL_SIZE - 1 - i, b);
            }
            m_leds->update();
            delay(2 * DELAYTIME);
            for (j = 0; j < m_iNumDevices + 1; j++)
            {
                m_leds->setRow(j, i, 0);
                m_leds->setColumn(j, i, 0);
                m_leds->setRow(j, ROW_SIZE - 1 - i, 0);
                m_leds->setColumn(j, COL_SIZE - 1 - i, 0);
            }

            i--;
            bitSet(b, i);
            bitSet(b, 7-i);
        }
    }
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

    /// Demonstrates animation of a diagonal stripe moving across the display with points plotted outside the display region ignored.
    const uint8_t stripeWidth = 10;
    m_leds->clear();
    for (i = 0; i < m_leds->getColumnCount() + ROW_SIZE + stripeWidth; i++)
    {
        for (j = 0; j < ROW_SIZE; j++)
        {
            m_leds->setPoint(j, i - j, true);
            m_leds->setPoint(j, i - j - stripeWidth, false);
        }
        delay(DELAYTIME);
    }

    // draws a Spiral across the whole display using setPoint()
    int rmin = 0, rmax = ROW_SIZE - 1;
    int cmin = 0, cmax = (COL_SIZE * m_iNumDevices) - 1;

    m_leds->clear();
    while ((rmax > rmin) && (cmax > cmin))
    {
        // do row
        for (int i = cmin; i <= cmax; i++)
        {
            m_leds->setPoint(rmin, i, true);
            delay(DELAYTIME / m_iNumDevices);
        }
        rmin++;

        // do column
        for (i = rmin; i <= rmax; i++)
        {
            m_leds->setPoint(i, cmax, true);
            delay(DELAYTIME / m_iNumDevices);
        }
        cmax--;

        // do row
        for (i = cmax; i >= cmin; i--)
        {
            m_leds->setPoint(rmax, i, true);
            delay(DELAYTIME / m_iNumDevices);
        }
        rmax--;

        // do column
        for (i = rmax; i >= rmin; i--)
        {
            m_leds->setPoint(i, cmin, true);
            delay(DELAYTIME / m_iNumDevices);
        }
        cmin++;
    }

    /// Animation of a bouncing ball
    i = 0;
    int r = 0, c = 2;
    int dR = 1, dC = 1;	// delta row and column

    m_leds->clear();
    while (i++ < 100)
    {
        m_leds->setPoint(r, c, false);
        r += dR;
        c += dC;
        m_leds->setPoint(r, c, true);
        delay(DELAYTIME/2);

        if ((r == 0) || (r == ROW_SIZE - 1))
            dR = -dR;
        if ((c == 0) || (c == m_leds->getColumnCount() - 1))
            dC = -dC;
    }

    /// Demonstrates control of display intensity (brightness) across the full range
    // m_leds->clear();
    // j = 0;
    // for (i = 0; i <= MAX_INTENSITY / 2; i++)        // NOTE: seems 1088AS don't support MAX_INTENSITY and can provoke strange behavior
    // {
    //     m_leds->control(MD_MAX72XX::INTENSITY, i);  // Grow and get brighter
    //     if (i % 2 == 0)
    //         m_leds->setRow(j++, 0xff);
    //     delay(DELAYTIME * 2);
    // }
    // m_leds->control(MD_MAX72XX::INTENSITY, MAX_INTENSITY / 2);
    // delay(DELAYTIME);

    ///Blinks the display on and off.
    // i = 3;
    // m_leds->clear();
    // while (i > 0)
    // {
    //     m_leds->control(MD_MAX72XX::TEST, MD_MAX72XX::ON);
    //     delay(DELAYTIME * i);
    //     m_leds->control(MD_MAX72XX::TEST, MD_MAX72XX::OFF);
    //     delay(DELAYTIME * i);
    //     i--;
    // }

    /// Uses scan limit function to restrict the number of rows displayed.
    m_leds->clear();
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
    for (i = 0; i < ROW_SIZE; i++)
        m_leds->setRow(i, 0xff);
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

    for (i = MAX_SCANLIMIT; i >= 0; i--)
    {
        m_leds->control(MD_MAX72XX::SCANLIMIT, i);
        delay(DELAYTIME * 3);
    }
    m_leds->control(MD_MAX72XX::SCANLIMIT, MAX_SCANLIMIT);
}

void CLedMarquee::TestsTransformations()
{
    int i;

    /// Demonstrates the use of transform() to move and animate bitmaps on the display
    uint8_t PROGMEM arrow[COL_SIZE] =
    {
        0b00001000, 0b00011100, 0b00111110, 0b01111111, //0x08, 0x1c, 0x3E, 0x7F
        0b00011100, 0b00011100, 0b00111110, 0b00000000  //0x1c, 0x1c, 0x3E, 0x00
    };
    MD_MAX72XX::transformType_t PROGMEM t[] =
    {
        MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
        MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
        MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
        MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
        MD_MAX72XX::TFLR,
        MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
        MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
        MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
        MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
        MD_MAX72XX::TRC,
        MD_MAX72XX::TSD, MD_MAX72XX::TSD, MD_MAX72XX::TSD, MD_MAX72XX::TSD,
        MD_MAX72XX::TSD, MD_MAX72XX::TSD, MD_MAX72XX::TSD, MD_MAX72XX::TSD,
        MD_MAX72XX::TFUD,
        MD_MAX72XX::TSU, MD_MAX72XX::TSU, MD_MAX72XX::TSU, MD_MAX72XX::TSU,
        MD_MAX72XX::TSU, MD_MAX72XX::TSU, MD_MAX72XX::TSU, MD_MAX72XX::TSU,
        MD_MAX72XX::TINV,
        MD_MAX72XX::TRC, MD_MAX72XX::TRC, MD_MAX72XX::TRC, MD_MAX72XX::TRC,
        MD_MAX72XX::TINV
    };

    // set the arrow bitmap in the matrix's buffer
    m_leds->clear();
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
    for (i = 0; i < m_leds->getDeviceCount(); i++)
        m_leds->setBuffer(((i + 1) * COL_SIZE) - 1, COL_SIZE, arrow);
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
    delay(DELAYTIME);

    // run through all in-table transformations and apply them to the arrow bitmap
    m_leds->control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::ON);    //equivalent to m_leds->wraparound(MD_MAX72XX::ON)
    for (i = 0; i < (sizeof(t) / sizeof(t[0])); i++)
    {
        m_leds->transform(t[i]);
        delay(DELAYTIME * 3);
    }
    m_leds->control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::OFF);


    /// Display text ('W') and animate scrolling using auto wraparound of the buffer
    m_leds->clear();
    m_leds->wraparound(MD_MAX72XX::ON);
    // draw the char to show
    for (i = 0; i < m_leds->getDeviceCount(); i++)
    {
        m_leds->setChar((COL_SIZE / 2) + (m_leds->getMaxFontWidth() / 3), (i & 1 ? 'M' : 'W'));
    }
    delay(DELAYTIME * 5);

    // run thru transformations
    for (i = 0; i < (3 * COL_SIZE * m_iNumDevices); i++)
    {
        m_leds->transform(MD_MAX72XX::TSL);
        delay(DELAYTIME / 2);
    }
    for (i = 0; i < (3 * COL_SIZE * m_iNumDevices); i++)
    {
        m_leds->transform(MD_MAX72XX::TSR);
        delay(DELAYTIME / 2);
    }
    for (i = 0; i < ROW_SIZE; i++)
    {
        m_leds->transform(MD_MAX72XX::TSU);
        delay(DELAYTIME * 2);
    }
    for (i = 0; i < ROW_SIZE; i++)
    {
        m_leds->transform(MD_MAX72XX::TSD);
        delay(DELAYTIME * 2);
    }
    m_leds->wraparound(MD_MAX72XX::OFF);

    /// Demonstrates the use of transform() to move animate font characters on the display.
    // MD_MAX72XX::transformType_t PROGMEM t1[]
    // {
    //     MD_MAX72XX::TINV,
    //     MD_MAX72XX::TRC, MD_MAX72XX::TRC, MD_MAX72XX::TRC, MD_MAX72XX::TRC,
    //     MD_MAX72XX::TINV,
    //     MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
    //     MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
    //     MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
    //     MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
    //     MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
    //     MD_MAX72XX::TSD, MD_MAX72XX::TSU, MD_MAX72XX::TSD, MD_MAX72XX::TSU,
    //     MD_MAX72XX::TFLR, MD_MAX72XX::TFLR, MD_MAX72XX::TFUD, MD_MAX72XX::TFUD
    // };

    // // draw something that will show changes
    // m_leds->clear();
    // m_leds->control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::OFF);
    // for (i = 0; i < m_leds->getDeviceCount(); i++)
    //     m_leds->setChar(((i + 1) * COL_SIZE) - 1, '0' + i);
    // delay(DELAYTIME * 5);

    // // run through all in-table transformations
    // for (i = 0; i < (sizeof(t1) / sizeof(t1[0])); i++)
    // {
    //     m_leds->transform(t1[i]);
    //     delay(DELAYTIME * 3);
    // }
}

void CLedMarquee::ShowPacman()
{
    //TODO: fix Pacman animation -> try to use the animation engine of CLedEyes engine.
    // int idx;                        // display index (column)
    // int frame;                      // current animation frame
    // int deltaFrame; 
    // const int MAX_FRAMES = 4;
    // const uint8_t PROGMEM pacman[MAX_FRAMES][18] =  // ghost pursued by a pacman. Matrix of 4 rows * 18 cols
    // {
    //     { 0xfe, 0x73, 0xfb, 0x7f, 0xf3, 0x7b, 0xfe, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0x7e, 0xff, 0xe7, 0xc3, 0x81, 0x00 },
    //     { 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0xff, 0xff, 0xe7, 0xe7, 0x42, 0x00 },
    //     { 0xfe, 0x73, 0xfb, 0x7f, 0xf3, 0x7b, 0xfe, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0xff, 0xff, 0xff, 0xe7, 0x66, 0x24 },
    //     { 0xfe, 0x7b, 0xf3, 0x7f, 0xf3, 0x7b, 0xfe, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c },
    // };
    // const uint8_t DATA_WIDTH = (sizeof(pacman[0]) / sizeof(pacman[0][0]));

    // m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
    // // Initialise
    // m_leds->clear();
    // idx = -DATA_WIDTH;
    // frame = 0;
    // deltaFrame = 1;

    // // Lay out the dots
    // for (uint8_t i = 0; i < m_iNumDevices; i++)
    // {
    //     m_leds->setPoint(3, (i * COL_SIZE) + 3, true);
    //     m_leds->setPoint(4, (i * COL_SIZE) + 3, true);
    //     m_leds->setPoint(3, (i * COL_SIZE) + 4, true);
    //     m_leds->setPoint(4, (i * COL_SIZE) + 4, true);
    // }

    // // clear old graphic
    // for (uint8_t i = 0; i < DATA_WIDTH; i++)
    //     m_leds->setColumn(idx - DATA_WIDTH + i, 0);
    // // move reference column and draw new graphic
    // idx++;
    // for (uint8_t i = 0; i < DATA_WIDTH; i++)
    //     m_leds->setColumn(idx - DATA_WIDTH + i, pacman[frame][i]);

    // // advance the animation frame
    // frame += deltaFrame;
    // if (frame == 0 || frame == MAX_FRAMES - 1)
    //     deltaFrame = -deltaFrame;

    // // check if we are completed and set initialise for next time around
    // //bInit = (idx == m_leds->getColumnCount() + DATA_WIDTH);
    // m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
    // delay(75);
}

void CLedMarquee::Shift()
{
    /// more transformation example separated in methods
    m_leds->control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::ON);
    for (int i = 0; i <= 7; i++)
    {
        // switch between the 7 possible transformations
        m_leds->clear();
        for (int j = 0; j < m_iNumDevices; j++)
            m_leds->setChar(((j + 1) * COL_SIZE) - 1, 'o' + j);

        switch (i)
        {
            case 0: m_leds->transform(0, m_iNumDevices - 1, MD_MAX72XX::TSL);  break;
            case 1:	m_leds->transform(0, m_iNumDevices - 1, MD_MAX72XX::TSR);  break;
            case 2:	m_leds->transform(0, m_iNumDevices - 1, MD_MAX72XX::TSU);  break;
            case 3:	m_leds->transform(0, m_iNumDevices - 1, MD_MAX72XX::TSD);  break;
            case 4:	m_leds->transform(0, m_iNumDevices - 1, MD_MAX72XX::TFUD); break;
            case 5:	m_leds->transform(0, m_iNumDevices - 1, MD_MAX72XX::TFLR); break;
            case 6:	m_leds->transform(0, m_iNumDevices - 1, MD_MAX72XX::TRC);  break;
            case 7:	m_leds->transform(0, m_iNumDevices - 1, MD_MAX72XX::TINV); break;
        }
        delay(DELAYTIME * 4);
    }
}

void CLedMarquee::ScrollText()
{
    static enum { S_LOAD,
                  S_SHOW,
                  S_SPACE } state = S_LOAD;
    static uint8_t curLen, showLen;
    static uint8_t cBuf[COL_SIZE];

    // if (bInit)
    // {
    //     m_leds->clear();
    //     state = S_LOAD;
    // }

    // Now scroll the text
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF); // start manual control
    m_leds->transform(MD_MAX72XX::TSL);                   // scroll along all devices

    // Now work out what's next using finite state machine to control what we do
    switch (state)
    {
        case S_LOAD: // Load the next character from the font table
            // if we reached end of message or empty string, reset the message pointer
            if (*m_msgText == '\0')
            {
                m_msgText = nullptr;
                break;
            }

            // otherwise load the character
            showLen = m_leds->getChar(*m_msgText++, ARRAY_SIZE(cBuf), cBuf);
            curLen = 0;
            state = S_SHOW;
            // fall through to the next state

        case S_SHOW: // display the next part of the character
            m_leds->setColumn(0, 0, cBuf[curLen++]);
            if (curLen == showLen)
            {
                showLen = (*m_msgText == '\0' ? 2 * COL_SIZE : 1); // either 1 space or pad to the end of the display if finished
                curLen = 0;
                state = S_SPACE;
            }
            break;

        case S_SPACE: // display inter-character spacing (blank columns)
            m_leds->setColumn(0, 0, 0);
            curLen++;
            if (curLen >= showLen)
                state = S_LOAD;
            break;

        default:
            state = S_LOAD;
    }
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
    delay(DELAYTIME * 3);
}

void CLedMarquee::BlinkEyes()
{
    typedef struct
    {
        char name[7];
        CLedEyes::emotion_t emotion;
        uint16_t timePause;  // in milliseconds
    } sampleItem_t;

    const sampleItem_t eSeq[] =
    {
        { "Neutral", CLedEyes::E_NEUTRAL, 20000 },
        { "Blink" , CLedEyes::E_BLINK, 1000 },
        { "Wink"  , CLedEyes::E_WINK, 1000 },
        { "Left"  , CLedEyes::E_LOOK_L, 1000 },
        { "Right" , CLedEyes::E_LOOK_R, 1000 },
        { "Up"    , CLedEyes::E_LOOK_U, 1000 },
        { "Down"  , CLedEyes::E_LOOK_D, 1000 },
        { "Angry" , CLedEyes::E_ANGRY, 1000 },
        { "Sad"   , CLedEyes::E_SAD, 1000 },
        { "Evil"  , CLedEyes::E_EVIL, 1000 },
        { "Evil2" , CLedEyes::E_EVIL2, 1000 },
        { "Squint", CLedEyes::E_SQUINT, 1000 },
        { "Dead"  , CLedEyes::E_DEAD, 1000 },
        { "ScanV" , CLedEyes::E_SCAN_UD, 1000 },
        { "ScanH" , CLedEyes::E_SCAN_LR, 1000 },
    };
    static enum { S_IDLE,
                  S_ANIM,
                  S_PAUSE } state = S_IDLE;
    static uint32_t timeStartDelay;
    static uint8_t index = ARRAY_SIZE(eSeq);

    bool b = m_eyes->runAnimation();    // always run the animation
    switch (state)
    {
        case S_IDLE:
            index++;
            if (index >= ARRAY_SIZE(eSeq)) 
                index = 0;
            //change emotion each time previous one has finished    
            m_eyes->setAnimation(eSeq[index].emotion, true);
            state = S_ANIM;
            break;

        case S_ANIM:    // checking animation is completed
            if (b)      // animation is finished
            {
                timeStartDelay = millis();
                state = S_PAUSE;
            }
            break;

        case S_PAUSE:   // non blocking waiting for a period between animations
            if (millis() - timeStartDelay >= eSeq[index].timePause)
                state = S_IDLE;
            break;

        default:
            state = S_IDLE;
            break;
    }
}

#pragma endregion