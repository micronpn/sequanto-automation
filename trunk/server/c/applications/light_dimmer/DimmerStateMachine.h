
#ifndef SEQUANTO_DIMMER_STATE_MACHINE_H_
#define SEQUANTO_DIMMER_STATE_MACHINE_H_

#include "StateMachine.h"

typedef enum _DimmerState
{
   DIMMER_INCREASING_ON = 1,
   DIMMER_INCREASING_OFF = 2,
   DIMMER_DECREASING_ON = 4,
   DIMMER_DECREASING_OFF = 8,
} DimmerState;

typedef enum _DimmerMessage
{
   DIMMER_HOLD,
   DIMMER_TOUCH,
   DIMMER_RESET,
} DimmerMessage;

class DimmerStateMachine : public StateMachine<DimmerState, DimmerMessage>
{
public:
   static const int MAX_LEVEL = 10;

private:
   int m_currentLevel;
   int m_savedLevel;
   
   void setState ( DimmerState _state );
   
public:
   DimmerStateMachine ();
   
   int currentLevel () const;
   void setCurrentLevel ( int _level );
   int savedLevel () const;
   void setSavedLevel ( int _level );
   
   void handle ( DimmerMessage _message );
};

class Dimmer : public DimmerStateMachine
{
public:
   static DimmerStateMachine instance;
};

#endif
