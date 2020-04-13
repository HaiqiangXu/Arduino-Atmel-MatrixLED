#include "CLedGame.h"

// Protected derived virtual methods
void CLedGameSnake::RefreshAnimation()
{
    CoordinateXY* currItem;

    m_leds->update(MD_MAX72XX::OFF);
    for (uint8_t i = 0; i < m_iCurrentLevel; i++)
    {
        currItem = m_Snake->get(i);
        m_leds->setPoint(currItem->x, currItem->y, true);
#ifdef DEBUG
Serial.println("x " + String(i) + ": " + String(currItem->x) + " | y" + String(i) + ": " + String(currItem->y));
#endif
    }
    m_leds->setPoint(m_Egg.x, m_Egg.y, true);
    m_leds->update(MD_MAX72XX::ON);

    //TODO: set delay depending to difficulty level?
    delay(1000);
    
    m_leds->clear();
}

void CLedGameSnake::GameCalculate()
{
#ifdef DEBUG
Serial.print("Previous direction: " + String((int)m_lastDirection));
#endif

    SetNextSnakePos(m_lastDirection);               //set last direction by default
    if (m_lastDirectionX != EDirection::None)
    {
        if (m_lastDirection == m_lastDirectionX ||  //accept any direction (3 possibilities) except the opposite direction
            m_lastDirection == EDirection::Up ||
            m_lastDirection == EDirection::Down)
        {
            SetNextSnakePos(m_lastDirectionX);
            m_lastDirection = m_lastDirectionX;
        }
    }
    else if (m_lastDirectionY != EDirection::None)
    {
        if (m_lastDirection == m_lastDirectionY ||
            m_lastDirection == EDirection::Left ||
            m_lastDirection == EDirection::Right)
        {
            SetNextSnakePos(m_lastDirectionY);
            m_lastDirection = m_lastDirectionY;
        }
    }

#ifdef DEBUG
Serial.println(". Set direction: " + String((int)m_lastDirection));
#endif
}

// Private methods
void CLedGameSnake::SetNewEgg()
{
    //TODO: control to not overlay existing snake dot
#ifndef DEBUG
    m_Egg = CoordinateXY { TrueRandom.random(ROW_SIZE - 1), TrueRandom.random((COL_SIZE * m_iNumDevices) - 1) };
#endif
    m_iCurrentLevel++;
}

void CLedGameSnake::SetNextSnakePos(EDirection direction)
{
    CoordinateXY *firstItem, *newItem;

    ////Go up 1 level
    // if (m_Snake[m_iCurrentLevel].x == m_Egg.x &&
    //     m_Snake[m_iCurrentLevel].y == m_Egg.y && 
    //     m_iCurrentLevel < MAX_SNAKE_LENGTH)
    // {
    //     m_Snake[m_iCurrentLevel].x = m_Egg.x;
    //     m_Snake[m_iCurrentLevel].y = m_Egg.y;

    //     SetNewEgg();
    // }
    
    newItem = m_Snake->pop();       //remove last item and destroy it
    delete newItem;
    firstItem = m_Snake->get(0);
    newItem = new CoordinateXY {firstItem->x, firstItem->y};
    switch (direction)
    {
        case EDirection::Left:
            newItem->y++;
            break;

        case EDirection::Right:
            newItem->y--;
            break;

        case EDirection::Up:
            newItem->x--;
            break;

        case EDirection::Down:
            newItem->x++;
            break;

        default:
            break;
    }
    m_Snake->unshift(newItem);  //set as first item

    //check if snake is inside borders TODOOOO: and don't get against the own snake
    if ( (newItem->y >= (COL_SIZE * m_iNumDevices)) ||
         (newItem->y < 0) ||
         (newItem->x >= ROW_SIZE) ||
         (newItem->x < 0) )
    {
        ResetGame();
    }
}

void CLedGameSnake::ResetGame()
{
    CoordinateXY* item;
    m_iCurrentLevel = 3;
    for (uint8_t i = 0; i < m_Snake->size(); i++)
    {
        item = m_Snake->shift();      //destroy all objects inside list
        delete item;
    }
    m_Snake->clear();

#ifdef DEBUG
    m_Snake->add(new CoordinateXY { 4, 12 });
    m_Snake->add(new CoordinateXY { 4, 11 });
    m_Snake->add(new CoordinateXY { 4, 10 });
#else
    m_Snake->add(new CoordinateXY { TrueRandom.random(ROW_SIZE - 1), TrueRandom.random((COL_SIZE * m_iNumDevices) - 1) });
    m_Snake->add(new CoordinateXY { TrueRandom.random(ROW_SIZE - 1), TrueRandom.random((COL_SIZE * m_iNumDevices) - 1) });
    m_Snake->add(new CoordinateXY { TrueRandom.random(ROW_SIZE - 1), TrueRandom.random((COL_SIZE * m_iNumDevices) - 1) });
#endif
}