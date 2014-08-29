#ifndef STATEMANAGER_H_
#define STATEMANAGER_H_

enum STATE_EVENT
{
  STATE_ENTRY,
  STATE_LEAVE,
  KEY_PRESSED,
  KEY_REPEATED,
  KEY_RELEASED,
  SECOND_TICK,
  MINUTE_TICK
};

typedef boolean (*STATE_FCN)( STATE_EVENT _event, char _ch, long _l, void **_pSave );

class StateManager
{
  STATE_FCN m_currentState;
  STATE_FCN m_newState;
  STATE_FCN m_lastState;
  void *m_save;

  public:

  StateManager(STATE_FCN initial) : m_currentState(null), m_newState(initial), m_lastState(null), m_save(null)
  {
  }

  void ChangeState(STATE_FCN newState);
  void HandleKeyPressed(char key);
  void HandleKeyRepeated(char key, unsigned long time);
  void HandleKeyReleased(char key, unsigned long time);
  void HandleSecondTick();
  void HandleMinuteTick();
  void OnIdle();
};

extern StateManager statemanager;

#endif
