#include "CLedEyes.h"
#include "CLedEyesData.h"

#pragma region Public methods

// initialize other stuff after libraries have started
void CLedEyes::begin(MD_MAX72XX *leds, uint8_t moduleStart)
{
    m_leds = leds;
    _sd = moduleStart;

    setAnimation(E_NEUTRAL, false);
};

// Animate the eyes. Return true if there is no animation happening
bool CLedEyes::runAnimation(void)
{
    static animFrame_t thisFrame;

    switch (m_animState)
    {
        case S_IDLE:                    // no animation running - wait for a new one or blink if time to do so
            // fall through and try for an animation
        case S_RESTART:                 // back to start of current animation
            if (m_nextEmotion != E_NONE)// check if we have an animation in the queue
            {
                _timeLastAnimation = millis();

                // set up the next animation
                loadSequence(m_nextEmotion);
                m_nextEmotion = E_NONE;
                m_animState = S_ANIMATE;
            }
            else if (_autoBlink)        // check if we should be blinking
            {
                if (((millis() - _timeLastAnimation) >= _timeBlinkMinimum) && (random(1000) > 700))
                {
                    setAnimation(E_BLINK, true);
                    m_animState = S_RESTART;
                }
            }
            break;

        case S_ANIMATE: // process the next frame for this sequence
            memcpy_P(&thisFrame, &_animEntry.seq[m_animIndex], sizeof(animFrame_t));// Load the idx'th frame from PROGMEM sequence to normal memory

            //Draw eyes
            MD_MAX72XX::fontType_t *savedFont;
            m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
            m_leds->setFont(_RobotEyes_Font);

            // Load the data and show it
            m_leds->clear(_sd, _sd + 1); // clear out display modules
            loadEye(_sd + LEFT_MODULE_OFFSET, thisFrame.eyeData[LEFT_EYE_INDEX]);
            loadEye(_sd + RIGHT_MODULE_OFFSET, thisFrame.eyeData[RIGHT_EYE_INDEX]);

            m_leds->setFont(m_leds->getFont());
            m_leds->control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
            if (m_animReverse)
                m_animIndex--;
            else
                m_animIndex++;

            _timeStartPause = millis();
            m_animState = S_PAUSE;
            break;

        case S_PAUSE: // pause this frame for the required time
            if ((millis() - _timeStartPause) < thisFrame.timeFrame)
                break;

            // check if this is the end of animation
            if ((!m_animReverse && m_animIndex >= _animEntry.size) ||
                (m_animReverse && m_animIndex < 0))
            {
                if (_autoReverse) // set up the same emotion but in reverse
                {
                    m_nextEmotion = _animEntry.emotion;
                    m_animReverse = true;  // set this flag for the restart state
                    _autoReverse = false; // clear the flag for this animation sequence
                    m_animState = S_RESTART;
                }
                else
                    m_animState = S_IDLE;
            }
            else
                m_animState = S_ANIMATE;
            break;

        default: // something is wrong - reset the FSM
            m_animState = S_IDLE;
            break;
    }

    return (m_animState == S_IDLE);
};

#pragma endregion

// Protected methods
void CLedEyes::loadEye(uint8_t module, uint8_t ch)
{
    uint8_t buf[EYE_COL_SIZE];
    uint8_t size;

    size = m_leds->getChar(ch, EYE_COL_SIZE, buf);
    for (uint8_t i = 0; i < EYE_COL_SIZE; i++)
    {
        m_leds->setColumn(module, i, buf[i]);
    }
}

// Load the next emotion from the static data. 
uint8_t CLedEyes::loadSequence(emotion_t e)
{
    // run through the lookup table to find the sequence data
    for (uint8_t i = 0; i < ARRAY_SIZE(lookupTable); i++)
    {
        memcpy_P(&_animEntry, &lookupTable[i], sizeof(animTable_t));
        if (_animEntry.emotion == e)
            break;
    }

    // set up the current index depending on direction of animation
    if (m_animReverse)
        m_animIndex = _animEntry.size - 1;
    else
        m_animIndex = 0;

    return (_animEntry.size);
}
