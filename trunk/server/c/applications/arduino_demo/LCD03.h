#ifndef LCD03_H_
#define LCD03_H_

void PutChar(char ch);
void PutString(const char *text);
void SetCursorPosition(int line, int column);
void PutString(int line, int column, const char *text);
void DisplayCommand(byte command);
void Cls();

char GetKeyPressed();


#endif
