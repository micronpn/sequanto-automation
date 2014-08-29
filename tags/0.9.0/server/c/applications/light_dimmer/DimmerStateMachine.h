
#ifndef SEQUANTO_DIMMER_STATE_MACHINE_H_
#define SEQUANTO_DIMMER_STATE_MACHINE_H_

#include "StateMachine.h"

typedef enum _DimmerState
{
   DIMMER_UP = 1,
   DIMMER_DOWN = 2,
   DIMMER_UP_PASSIVE = 3,
   DIMMER_DOWN_PASSIVE = 4,
} DimmerState;

typedef enum _DimmerMessage
{
   DIMMER_START_HOLD = 1,
   DIMMER_HOLD = 2,
   DIMMER_STOP_HOLD = 3,
   DIMMER_TOUCH = 4,
   DIMMER_RESET = 5,
} DimmerMessage;

class DimmerStateMachine : public StateMachine<DimmerState, DimmerMessage>
{
public:
   static const int MAX_LEVEL = 10;

private:
   int m_currentLevel;
   int m_savedLevel;
   bool m_sendOutput;
   bool m_lightOn;
   
   void setState ( DimmerState _state );
   
public:
   DimmerStateMachine ();
   
   int currentLevel () const;
   int getCurrentLevelAutomation ();
   void setCurrentLevel ( int _level );
   int savedLevel () const;
   void setSavedLevel ( int _level );
   bool sendOutput ();
   
   void handle ( DimmerMessage _message );
};

class Dimmer : public DimmerStateMachine
{
public:
   static DimmerStateMachine instance;
};

#endif
