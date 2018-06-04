#include "CLedMarquee.h"

#pragma region Public methods

void CLedMarquee::ShowMarquee(EMarqueeStyle style)
{
    switch(style)
    {
        case EMarqueeStyle::Test:
            Test();
            // TestAdvanced();
            // TestTransformations();
            break;

        case EMarqueeStyle::Text:
            break;

        case EMarqueeStyle::Pacman:
            //TODO: check Pacman animation works
            int idx;                        // display index (column)
            int frame;                      // current animation frame
            int deltaFrame; 

            // // Is it time to animate?
            // if (millis() - m_prevTimeAnim < 75) //TODO: 75 ms animation
            //     return;
            // m_prevTimeAnim = millis();			// starting point for next time
            m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
            // Initialise
            m_leds->clear();
            idx = -DATA_WIDTH;
            frame = 0;
            deltaFrame = 1;

            // Lay out the dots
            for (uint8_t i = 0; i < m_iNumDevices; i++)
            {
                m_leds->setPoint(3, (i * COL_SIZE) + 3, true);
                m_leds->setPoint(4, (i * COL_SIZE) + 3, true);
                m_leds->setPoint(3, (i * COL_SIZE) + 4, true);
                m_leds->setPoint(4, (i * COL_SIZE) + 4, true);
            }

            // clear old graphic
            for (uint8_t i = 0; i < DATA_WIDTH; i++)
                m_leds->setColumn(idx - DATA_WIDTH + i, 0);
            // move reference column and draw new graphic
            idx++;
            for (uint8_t i = 0; i < DATA_WIDTH; i++)
                m_leds->setColumn(idx - DATA_WIDTH + i, pacman[frame][i]);

            // advance the animation frame
            frame += deltaFrame;
            if (frame == 0 || frame == MAX_FRAMES - 1)
                deltaFrame = -deltaFrame;

            // check if we are completed and set initialise for next time around
            //bInit = (idx == m_leds->getColumnCount() + DATA_WIDTH);
            m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
            delay(75);

            break;

        case EMarqueeStyle::BlinkEyes:
            break;

        case EMarqueeStyle::Shift:
            Shift();
            break;

        default:
            break;
    }
}

#pragma endregion

#pragma region Private methods

void CLedMarquee::Test()
{
    int i, j;

    /// Demonstrates the use of setPoint and show where the zero point is in the display
    m_leds->clear();
    for (i = 0; i < ROW_SIZE; i++)
    {
        m_leds->setPoint(i, i, true);
        m_leds->setPoint(0, i, true);
        m_leds->setPoint(i, 0, true);
        delay(DELAYTIME);
    }
    delay(DELAYTIME * 3);

    /// Demonstrate the use of drawHLine(). Draws from init to end of line coordinates with only one pixel on at the same time
    // m_leds->clear();
    // for (i = 0; i < m_leds->getColumnCount(); i++)
    // {
    //     m_leds->drawHLine(i, i, m_leds->getColumnCount() - ROW_SIZE + i, true);
    //     delay(2 * DELAYTIME);
    // }

    // // Demonstrate the use of drawVLine().
    // m_leds->clear();
    // for (i = 0; i < m_leds->getColumnCount(); i++)
    // {
    //     m_leds->drawVLine(i, 0, i % ROW_SIZE, true);
    //     delay(2 * DELAYTIME);
    // }

    /// Demonstrates the use of setRow()
    m_leds->clear();
    for (i = 0; i < ROW_SIZE; i++)
    {
        m_leds->setRow(i, 0xff);
        delay(2 * DELAYTIME);
        m_leds->setRow(i, 0x00);
    }

    /// nested rectangles spanning the entire display
    m_leds->clear();
    for (i = 0; i < 2; i++)     //repeat 2 times
    {
        for (j = 0; j < ROW_SIZE / 2; j++)
        {
            m_leds->drawRectangle(j, j, ROW_SIZE - 1 - j, m_leds->getColumnCount() - 1 - j, true);
            delay(2 * DELAYTIME);
        }
        for (j = 0; j < ROW_SIZE / 2; j++)
        {
            m_leds->drawRectangle(j, j, ROW_SIZE - 1 - j, m_leds->getColumnCount() - 1 - j, false);
            delay(2 * DELAYTIME);
        }
    }

    /// Demonstrates the use of setColumn()
    m_leds->clear();
    for (i = 0; i < m_leds->getColumnCount(); i++)
    {
        m_leds->setColumn(i, 0xff);
        delay(2 * DELAYTIME);
        m_leds->setColumn(i, 0x00);
    }

    /// Run through display of the the entire font ASCII characters set
    m_leds->clear();
    m_leds->update(MD_MAX72XX::OFF);
    for (i = 0; i < 256; i++)
    {
        m_leds->clear();
        //FC-16 board's column goes from R to L instead of normal way L to R 
        m_leds->setChar((COL_SIZE) / 2 + (m_leds->getMaxFontWidth() / 3), i);
        m_leds->update();
        delay(DELAYTIME * 3);
    }
    m_leds->update(MD_MAX72XX::ON);
}

void CLedMarquee::TestAdvanced()
{
    int i, j, n;
  
    /// Combination of setRow() and setColumn() to form a cross
    m_leds->clear();
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
    for (i = 0; i < ROW_SIZE; i++)      // diagonally down the display R to L
    {
        for (j = 0; j < m_iNumDevices; j++)
        {
            m_leds->setColumn(j, i, 0xff);
            m_leds->setRow(j, i, 0xff);
        }
        m_leds->update();               //update manually as control(MD_MAX72XX::UPDATE) is switched off
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
    for (n = 0; n < 2; n++)
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
    // const uint16_t maxCol = m_iNumDevices * ROW_SIZE;
    // const uint8_t  stripeWidth = 10;
    // m_leds->clear();
    // for (i = 0; i < maxCol + ROW_SIZE + stripeWidth; i++)
    // {
    //     for (j = 0; j < ROW_SIZE; j++)
    //     {
    //         m_leds->setPoint(j, i - j, true);
    //         m_leds->setPoint(j, i - j - stripeWidth, false);
    //     }
    //     delay(DELAYTIME);
    // }

    // setPoint() used to draw a Spiral across the whole display
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

    /// Demonstrates control of display intensity (brightness) across the full range.
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

void CLedMarquee::TestTransformations()
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

    // use the arrow bitmap
    m_leds->clear();
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
    for (i = 0; i < m_leds->getDeviceCount(); i++)
        m_leds->setBuffer(((i + 1) * COL_SIZE) - 1, COL_SIZE, arrow);
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
    delay(DELAYTIME);

    // run through all in-table transformations
    m_leds->control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::ON);
    for (i = 0; i < (sizeof(t) / sizeof(t[0])); i++)
    {
        m_leds->transform(t[i]);
        delay(DELAYTIME * 3);
    }
    m_leds->control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::OFF);

    // /// Demonstrates the use of transform() to move animate font characters on the display.
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

    /// Display text and animate scrolling using auto wraparound of the buffer
    m_leds->clear();
    m_leds->wraparound(MD_MAX72XX::ON);

    // draw something that will show changes
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
}

void CLedMarquee::Shift()
{
    static int8_t tState = -1;
    static bool bNew = true;
    static uint32_t	lastTime = 0;
    static uint8_t	repeatCount = 0;

    m_leds->control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::ON);
    if (bNew)
        tState = (tState + 1) % 8;

    switch (tState)
    {
        case 0: ShiftTransform(MD_MAX72XX::TSL,	bNew);	break;
        case 1:	ShiftTransform(MD_MAX72XX::TSR,	bNew);	break;
        case 2:	ShiftTransform(MD_MAX72XX::TSU,	bNew);	break;
        case 3:	ShiftTransform(MD_MAX72XX::TSD,	bNew);	break;
        case 4:	ShiftTransform(MD_MAX72XX::TFUD,bNew);	break;
        case 5:	ShiftTransform(MD_MAX72XX::TFLR,bNew);	break;
        case 6:	ShiftTransform(MD_MAX72XX::TRC,	bNew);	break;
        case 7:	ShiftTransform(MD_MAX72XX::TINV,bNew);	break;
        default:tState = 0;
    }

    //changeState -> can be interactive with a physical Switch
    if (repeatCount == 0)
        repeatCount = 16;   //const int REPEATS_PRESET;

    if (millis()-lastTime >= DELAYTIME * 5)
    {
        lastTime = millis();
        bNew = (--repeatCount == 0);
    }
}

void CLedMarquee::ShiftTransform(MD_MAX72XX::transformType_t tt, bool bNew)
{
    static uint32_t lastTime = 0;

    if (bNew)
    {
        m_leds->clear();
        for (int i = 0; i < m_iNumDevices; i++)
            m_leds->setChar(((i + 1) * COL_SIZE) - 1, 'o' + i);
        lastTime = millis();
    }

    if (millis() - lastTime >= DELAYTIME * 5)
    {
        m_leds->transform(0, m_iNumDevices - 1, tt);
        lastTime = millis();
    }
}

#pragma endregion