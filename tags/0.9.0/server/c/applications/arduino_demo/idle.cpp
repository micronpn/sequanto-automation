#include "common.h"

boolean loggedIn = false;

boolean Idle( STATE_EVENT _event, char _ch, long _l, void **_pSave )
{
  static int secs = 0;
  switch (_event)
  {
    case STATE_ENTRY:
      Cls();
      DisplayCommand(4);
      DisplayCommand(20);
      PutString(1, 1, "Sequanto HouseKeeper");
      SetCursorPosition(3, 1);
      secs = 0;
      if (!loggedIn)
      {
        SetCursorPosition(4, 1);
        PutString("Locked.      Press #");
      }
      break;
    case KEY_RELEASED:
      if (_ch == '#')
      {
        if (loggedIn)
        {
          statemanager.ChangeState(MainMenu);
        }
        else
        {
          statemanager.ChangeState(Login);
        }
      }
      break;
    case SECOND_TICK:
      secs++;
      if (loggedIn && secs >= 15)
      {
        loggedIn = false;
        SetCursorPosition(4, 1);
        PutString("Locked.      Press #");
      }
      else 
      break;
    default:
      return false;
  }
  return true;
}

void ShowCodeInput(char *input)
{
  SetCursorPosition(3, 8);
  char *p = input;
  while (*p != 0)
  {
    PutChar('*');
    p++;
  }
}

boolean Login( STATE_EVENT _event , char _ch, long _l, void **_pSave)
{
  static int secs = 0;
  static char input[5];
  switch (_event)
  {
    case STATE_ENTRY:
      Cls();
      input[0] = 0;
      DisplayCommand(6);
      DisplayCommand(19);
      PutString(1, 1, "    Please enter    ");
      PutString(2, 1, "     your code:     ");
      SetCursorPosition(3, 8);
      secs = 0;
      break;
    case KEY_PRESSED:
      secs = 0;
      if (_ch == '*')
      {
      }
      else if (_ch == '#')
      {
        int l = strlen(input);
        if (l == 4)
        {
          if (strcmp(input, "1234") == 0)
          {
            loggedIn = true;
            statemanager.ChangeState(MainMenu);
          }
          else
          {
            statemanager.ChangeState(LoginFailed);
          }
        }
      }
      else
      {
        int l = strlen(input);
        if (l < 4)
        {
          input[l++] = _ch;
          input[l] = 0;
          ShowCodeInput(input);
        }
      }
      break;
    case SECOND_TICK:
      if (secs == 10)
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

boolean LoginFailed( STATE_EVENT _event , char _ch, long _l, void **_pSave)
{
  static int secs = 0;
  switch (_event)
  {
    case STATE_ENTRY:
      Cls();
      DisplayCommand(4);
      PutString(1, 1, "     Wrong Code!    ");
      secs = 0;
      break;
    case SECOND_TICK:
      if (secs == 3)
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
