#ifndef CLOCK_H_
#define CLOCK_H_

struct ALARM
{
  void (*pAlarm)(struct ALARM *pAlarm);
  boolean enabled;
  boolean set;
  byte hour;
  byte minute;
  byte second;
};

void SetupClock();
unsigned long Now();
boolean CheckAlarm(struct ALARM *pAlarm);
boolean UpdateClock();
int GetClockString(char *buffer);

#ifdef __cplusplus
extern "C"
{
#endif

int clock_now( void );
char* clock_nowtext ( void );

int get_weekday ( void );
void set_weekday ( int _weekday );
int get_hour ( void );
void set_hour ( int _hour );
int get_minute ( void );
void set_minute ( int _minute );
int get_second ( void );
void set_second ( int _second );

#ifdef __cplusplus
}
#endif

#endif
