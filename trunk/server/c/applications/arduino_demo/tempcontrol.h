#ifndef TEMPCONTROL_H_
#define TEMPCONTROL_H_

extern SQBool IsInDayTime( unsigned long clock );
extern void TemperatureRegulation(void);

#ifdef __cplusplus
extern "C"
{
#endif

int get_currenttemp ( void );
void set_currenttemp( int _temp );
int read_temperature(void);
int read_voltage(void);
SQBool get_heaterstate ( void );
void set_heaterstate ( SQBool _state );

int get_tempsetpoint ( void );
void set_tempsetpoint ( int _temp );

int get_daytemp ( void );
void set_daytemp ( int _temp );
int get_dayclock ( void );
void set_dayclock ( int _clock );

SQBool get_nighttempused ( void );
void set_nighttempused ( SQBool _used );
int get_nighttemp ( void );
void set_nighttemp ( int _temp );
int get_nightclock ( void );
void set_nightclock ( int _clock );

#ifdef __cplusplus
}
#endif

#endif

