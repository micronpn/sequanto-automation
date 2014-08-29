#include "common.h"

boolean MainMenu( STATE_EVENT _event , char _ch, long _l, void **_pSave)
{
  static int secs = 0;
  switch (_event)
  {
    case STATE_ENTRY:
      Cls();
      DisplayCommand(4);
      DisplayCommand(19);
      PutString(1, 1, "1: Alarms       ");
      PutString(2, 1, "2:              ");
      PutString(3, 1, "3:              ");
      PutString(4, 1, "4: Settings     ");
      secs = 0;
      break;
    case KEY_RELEASED:
      if (_ch == '1')
      {
      }
      else if (_ch == '2')
      {
      }
      else if (_ch == '3')
      {
      }
      else if (_ch == '4')
      {
      }
      else if (_ch == '*')
      {
        statemanager.ChangeState(Idle);
      }
      break;
    case SECOND_TICK:
      if (secs == 15)
      {
        statemanager.ChangeState(Idle);
      }
      else
      {
        secs++;
      }
      break;
    default:
      return false;
  }
  return true;
}


