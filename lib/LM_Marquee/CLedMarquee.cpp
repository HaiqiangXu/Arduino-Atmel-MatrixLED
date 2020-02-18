#include "CLedMarquee.h"

#pragma region Public methods

void CLedMarquee::ShowMarquee()
{
    switch(m_currentMarquee)
    {
        case EMarqueeStyle::Test:
            // NOTE: if Arduino hangs in some test, it's likely due to misconversion between int->unsigned int->uint16_t->etc. Watch out about the data type used in loops!
            if (m_iNumDevices == 1)
                TestsOneDevice();
            TestsAdvanced();
            TestsTransformations();
            delay(DELAY_TIME * 4);
            break;

        case EMarqueeStyle::Text:
            this->ScrollText();
            break;

        case EMarqueeStyle::Pacman:
            this->ShowPacman();
            break;

        case EMarqueeStyle::BlinkEyes:
            // limited to first 2 devices
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
        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME * 2);
    }

    /// Demonstrate the use of drawHLine(). Another way of drawing a line different to setRow()/setColumn().
    m_leds->clear();
    for (i = 0; i < m_leds->getColumnCount(); i++)  // getColumnCount() returns (m_iNumDevices * COL_SIZE)
    {
        m_leds->drawHLine(i, i, m_leds->getColumnCount() - ROW_SIZE + i, true);
        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME * 2);
    }

    /// Demonstrate the use of drawVLine().
    m_leds->clear();
    for (i = 0; i < m_leds->getColumnCount(); i++)
    {
        m_leds->drawVLine(i, 0, i % ROW_SIZE, true);
        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME * 2);
    }

    /// Demonstrates the use of setRow() for axis X. Together with setColumn() and setPoint() will be the most used methods to draw. Also setBuffer() to load figures saved into Arduino's PROGMEM
    m_leds->clear();
    for (i = 0; i < ROW_SIZE; i++)
    {
        m_leds->setRow(i, 0b11101111);  //the 'value' parameter represents the pixel of the line that will be switched on/off
        delay(DELAY_TIME * 2);
        m_leds->setRow(i, 0x00);
        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME * 2);
    }

    /// Demonstrates the use of setColumn() for axis Y.
    m_leds->clear();
    for (i = 0; i < m_leds->getColumnCount(); i++)
    {
        m_leds->setColumn(i, 0xff);
        delay(DELAY_TIME * 2);
        m_leds->setColumn(i, 0x00);
        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME * 2);
    }

    /// Demonstrates the use of drawRectangle(). Show nested rectangles spanning the entire display
    m_leds->clear();
    for (i = 0; i < 2; i++)     //loop 2 times
    {
        // cumulate rectangles from outside to inside
        for (j = 0; j < ROW_SIZE / 2; j++)
        {
            m_leds->drawRectangle(j, j, ROW_SIZE - 1 - j, m_leds->getColumnCount() - 1 - j, true);
            delay(DELAY_TIME * 2);
        }
        if (digitalRead(IN_BTN) == LOW)
            break;

        // clear rectangles from outside to inside
        for (j = 0; j < ROW_SIZE / 2; j++)
        {
            m_leds->drawRectangle(j, j, ROW_SIZE - 1 - j, m_leds->getColumnCount() - 1 - j, false);
            delay(DELAY_TIME * 2);
        }
        if (digitalRead(IN_BTN) == LOW)
            break;
    }

    /// Run through display of the the entire font ASCII characters set
    for (i = 0; i < 128; i++)
    {
        m_leds->clear();
        //FC-16 board's column goes from R to L instead of normal way L to R 
        m_leds->setChar((COL_SIZE) / 2 + (m_leds->getMaxFontWidth() / 3), i);
        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME * 2);
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
        m_leds->update();               // update manually all devices as control(MD_MAX72XX::UPDATE) is switched off
        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME * 2);
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
        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME * 2);
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
        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME * 3);
        for (j = 0; j < m_iNumDevices; j++)
        {
            m_leds->setColumn(j, i, 0x00);
            m_leds->setRow(j, ROW_SIZE - 1 - i, 0x00);
        }
    }
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

    /// Bullseye. Demonstrate the use of buffer based repeated patterns across all devices.
    m_leds->clear();
    m_leds->update(MD_MAX72XX::OFF);
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
            if (digitalRead(IN_BTN) == LOW)
                break;
            else
                delay(DELAY_TIME * 2);
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
            if (digitalRead(IN_BTN) == LOW)
                break;
            else
                delay(DELAY_TIME * 2);
            for (j = 0; j < m_iNumDevices + 1; j++)
            {
                m_leds->setRow(j, i, 0);
                m_leds->setColumn(j, i, 0);
                m_leds->setRow(j, ROW_SIZE - 1 - i, 0);
                m_leds->setColumn(j, COL_SIZE - 1 - i, 0);
            }

            i--;
            bitSet(b, i);
            bitSet(b, 7 - i);
        }
    }
    m_leds->update(MD_MAX72XX::ON);

    /// Demonstrates animation of a diagonal stripe moving across the display with points plotted outside the display region ignored.
    const uint8_t stripeWidth = 8;
    m_leds->clear();
    for (i = 0; i < m_leds->getColumnCount() + ROW_SIZE + stripeWidth; i++)
    {
        for (j = 0; j < ROW_SIZE; j++)
        {
            m_leds->setPoint(j, i - j, true);
            m_leds->setPoint(j, i - j - stripeWidth, false);
        }
        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME);
    }

    /// Draws a Spiral across the whole display using setPoint()
    int rmin = 0, rmax = ROW_SIZE - 1;                     // Rows are always 8
    int cmin = 0, cmax = (COL_SIZE * m_iNumDevices) - 1;   // Cols are 8 * NumDevices

    m_leds->clear();
    while ((rmax > rmin) && (cmax > cmin))
    {
        // do row
        for (i = cmin; i <= cmax; i++)
        {
            m_leds->setPoint(rmin, i, true);
            delay(DELAY_TIME / m_iNumDevices);
        }
        rmin++;

        // do column
        for (i = rmin; i <= rmax; i++)
        {
            m_leds->setPoint(i, cmax, true);
            delay(DELAY_TIME / m_iNumDevices);
        }
        cmax--;

        // do row
        for (i = cmax; i >= cmin; i--)
        {
            m_leds->setPoint(rmax, i, true);
            delay(DELAY_TIME / m_iNumDevices);
        }
        rmax--;

        // do column
        for (i = rmax; i >= rmin; i--)
        {
            m_leds->setPoint(i, cmin, true);
            delay(DELAY_TIME / m_iNumDevices);
        }
        cmin++;

        if (digitalRead(IN_BTN) == LOW)
            break;
    }

    /// Animation of a bouncing ball
    i = 0;
    unsigned int r = 0, c = 2;
    unsigned int dR = 1, dC = 1;	// delta row and column

    m_leds->clear();
    while (i++ < 100)
    {
        m_leds->setPoint(r, c, false);
        r += dR;
        c += dC;
        m_leds->setPoint(r, c, true);
        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME / 2);

        if ((r == 0) || (r == ROW_SIZE - 1))
            dR = -dR;
        if ((c == 0) || (c == m_leds->getColumnCount() - 1))
            dC = -dC;
    }

    /// Demonstrates control of display intensity (brightness) across the full range
    m_leds->clear();
    for (i = 0; i < 8; i++)     // NOTE: up to 8 levels of intensity by shifting the byte (8 bits) that represents the intensity. Module 1088AS seems to have 5 levels.
    {
        m_leds->control(MD_MAX72XX::INTENSITY, MAX_INTENSITY >> i);  // Decrease intensity and get darker
        m_leds->setRow(i, 0xff);
        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME * 4);
    }
    m_leds->control(MD_MAX72XX::INTENSITY, MAX_INTENSITY >> 4);
    delay(DELAY_TIME);

    /// Blinks the display on and off at max brightness.
    i = 3;
    m_leds->clear();
    while (i > 0)
    {
        m_leds->control(MD_MAX72XX::TEST, MD_MAX72XX::ON);
        delay(DELAY_TIME * i);
        m_leds->control(MD_MAX72XX::TEST, MD_MAX72XX::OFF);
        delay(DELAY_TIME * i);
        i--;
    }

    /// Uses scan limit function to restrict the number of rows displayed.
    m_leds->clear();
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
    for (i = 0; i < ROW_SIZE; i++)
    {
        m_leds->setRow(i, 0xff);
    }
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);

    for (i = MAX_SCANLIMIT; i >= 0; i--)
    {
        m_leds->control(MD_MAX72XX::SCANLIMIT, i);
        delay(DELAY_TIME * 4);
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
    {
        m_leds->setBuffer(((i + 1) * COL_SIZE) - 1, COL_SIZE, arrow);
    }
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);        //it provokes a refresh of the display
    delay(DELAY_TIME);

    // run through all in-table transformations and apply them to the arrow bitmap
    m_leds->control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::ON);    //equivalent to m_leds->wraparound(MD_MAX72XX::ON). Goes with transform() to update/apply the transformation and avoid object lose into another device
    for (i = 0; i < (sizeof(t) / sizeof(t[0])); i++)
    {
        m_leds->transform(t[i]);
        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME * 3);
    }
    m_leds->control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::OFF);


    /// Display text ('W') and animate scrolling using auto wraparound of the buffer
    m_leds->clear();
    m_leds->wraparound(MD_MAX72XX::ON);
    // draw the char to show
    for (i = 0; i < m_leds->getDeviceCount(); i++)
    {
        m_leds->setChar((COL_SIZE / 2) + (m_leds->getMaxFontWidth() / 3), (i & 1 ? 'M' : 'W')); //alternate between 'M' and 'W' through alternate devices
    }
    delay(DELAY_TIME * 4);

    // run through some transformations
    for (i = 0; i < (3 * COL_SIZE * m_iNumDevices); i++)
    {
        m_leds->transform(MD_MAX72XX::TSL);
        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME / 2);
    }
    for (i = 0; i < (3 * COL_SIZE * m_iNumDevices); i++)
    {
        m_leds->transform(MD_MAX72XX::TSR);
        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME / 2);
    }
    for (i = 0; i < (2 * ROW_SIZE); i++)
    {
        if (i / 4 == 0)
            m_leds->transform(MD_MAX72XX::TSU);
        else
            m_leds->transform(MD_MAX72XX::TSD);

        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME * 2);
    }
    m_leds->wraparound(MD_MAX72XX::OFF);

    /// Demonstrates the use of transform() to move animate font characters on the display.
    MD_MAX72XX::transformType_t PROGMEM t1[]
    {
        MD_MAX72XX::TINV,
        MD_MAX72XX::TRC, MD_MAX72XX::TRC, MD_MAX72XX::TRC, MD_MAX72XX::TRC,
        MD_MAX72XX::TINV,
        MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
        MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
        MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
        MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
        MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
        MD_MAX72XX::TSD, MD_MAX72XX::TSU, MD_MAX72XX::TSD, MD_MAX72XX::TSU,
        MD_MAX72XX::TFLR, MD_MAX72XX::TFLR, MD_MAX72XX::TFUD, MD_MAX72XX::TFUD
    };

    // draw character that will show transformations
    m_leds->clear();
    for (i = 0; i < m_leds->getDeviceCount(); i++)
        m_leds->setChar(((i + 1) * COL_SIZE) - 1, '0' + i);
    delay(DELAY_TIME);

    // run through all in-table transformations
    m_leds->wraparound(MD_MAX72XX::ON);
    for (i = 0; i < ARRAY_SIZE(t1); i++)
    {
        m_leds->transform(t1[i]);
        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME * 3);
    }
    m_leds->wraparound(MD_MAX72XX::OFF);

    /// more transformation example with 'o' character
    m_leds->control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::ON);
    for (i = 0; i < 8; i++)
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
        if (digitalRead(IN_BTN) == LOW)
            break;
        else
            delay(DELAY_TIME * 4);
    }
    m_leds->control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::OFF);
}

void CLedMarquee::ShowPacman()
{
    //TODO: try to use the animation engine of CLedEyes engine.
    static int idx;                        // display index (column). Static means this value is kept for the next iteration of main.loop()
    static int frame;                      // current animation frame
    static int deltaFrame; 
    static boolean bInit = true;
    const int MAX_FRAMES = 4;
    const uint8_t PROGMEM pacman[MAX_FRAMES][18] =  // ghost pursued by a pacman. Matrix of 4 rows * 18 cols. Don't use Warparound as the characters are intended to exit the display area
    {
        { 0xfe, 0x73, 0xfb, 0x7f, 0xf3, 0x7b, 0xfe, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0x7e, 0xff, 0xe7, 0xc3, 0x81, 0x00 },
        { 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0xff, 0xff, 0xe7, 0xe7, 0x42, 0x00 },
        { 0xfe, 0x73, 0xfb, 0x7f, 0xf3, 0x7b, 0xfe, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0xff, 0xff, 0xff, 0xe7, 0x66, 0x24 },
        { 0xfe, 0x7b, 0xf3, 0x7f, 0xf3, 0x7b, 0xfe, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c },
    };
    const uint8_t DATA_WIDTH = (sizeof(pacman[0]) / sizeof(pacman[0][0]));

    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
    // Initialise
    if (bInit)
    {
        m_leds->clear();
        idx = -DATA_WIDTH;
        frame = 0;
        deltaFrame = 1;
        bInit = false;

        // Lay out the initial dots
        for (uint8_t i = 0; i < m_iNumDevices; i++)
        {
            m_leds->setPoint(3, (i * COL_SIZE) + 5, true);
            m_leds->setPoint(4, (i * COL_SIZE) + 5, true);
            m_leds->setPoint(3, (i * COL_SIZE) + 6, true);
            m_leds->setPoint(4, (i * COL_SIZE) + 6, true);
        }
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
    bInit = (idx == m_leds->getColumnCount() + DATA_WIDTH);
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
    
    delay(DELAY_TIME);
}

void CLedMarquee::ScrollText()
{
    static enum { S_LOAD,
                  S_SHOW,
                  S_SPACE } state = S_LOAD;
    static uint8_t curLen, showLen;
    static uint8_t cBuf[COL_SIZE];

    // Now scroll the text
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF); // start manual control
    m_leds->transform(MD_MAX72XX::TSL);                   // scroll left along all devices

    // Now work out what's next using finite state machine to control what we do
    switch (state)
    {
        case S_LOAD: // Load the next character from the font table. To set new String, use SetText() method
            if (*m_msgText == '\0') //if we reached end of message or empty string, reset showing message pointer
            {
                m_msgText = m_msgTextIni;
                m_leds->clear(0);   //clears garbage previous to first char. Using clear() removes also last 2 chars of the message in device 0 only
                break;
            }

            // otherwise load next character
            showLen = m_leds->getChar(*m_msgText++, ARRAY_SIZE(cBuf), cBuf);
#ifdef DEBUG
Serial.print("Next char: ");
Serial.println(m_msgText);
#endif
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
#ifdef DEBUG
Serial.print("space...");
if (curLen >= showLen)
    Serial.println("---");
#endif                
            break;

        default:
            state = S_LOAD;
    }
    m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
    delay(DELAY_TIME * 2);
}

void CLedMarquee::BlinkEyes()
{
    typedef struct
    {
        emotion_t emotion;
        uint16_t timePause;  // show emotion in milliseconds
    } sampleItem_t;

    const sampleItem_t eSeq[] =
    {
        { E_NEUTRAL, 10000 },
        { E_BLINK, 1000 },
        { E_WINK, 1000 },
        { E_LOOK_L, 1000 },
        { E_LOOK_R, 1000 },
        { E_LOOK_U, 1000 },
        { E_LOOK_D, 1000 },
        { E_ANGRY, 1000 },
        { E_SAD, 1000 },
        { E_EVIL, 1000 },
        { E_EVIL2, 1000 },
        { E_SQUINT, 1000 },
        { E_DEAD, 1000 },
        { E_SCAN_UD, 1000 },
        { E_SCAN_LR, 1000 },
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