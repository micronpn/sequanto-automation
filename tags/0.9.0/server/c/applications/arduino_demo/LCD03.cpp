#include "common.h"
#include "Wire.h"
#include "LCD03.h"

const int address = 0xC6 >> 1;

void PutChar(char ch)
{
  Wire.beginTransmission(address);
  Wire.send(0);
  Wire.send(ch);
  Wire.endTransmission();
}

void PutString(const char *text)
{
  const char *p = text;
  while (*p != 0)
  {
    PutChar(*p);
    p++;
  }
}

void SetCursorPosition(int line, int column)
{
  Wire.beginTransmission(address);
  Wire.send(0);
  Wire.send(3);
  Wire.send((byte)line);
  Wire.send((byte)column);
  Wire.endTransmission();
}

void PutString(int line, int column, const char *text)
{
  SetCursorPosition(line, column);
  PutString(text);
}

void DisplayCommand(byte command)
{
  Wire.beginTransmission(address);
  Wire.send(0);
  Wire.send(command);
  Wire.endTransmission();
}

void Cls()
{
  DisplayCommand(12);
  DisplayCommand(1);
}

char GetKeyPressed()
{
  DisplayCommand(0);
  Wire.requestFrom(address, 3);
  while (Wire.available() < 3);
  Wire.receive();	// remove 
  byte second = Wire.receive();
  byte third = Wire.receive();
  DisplayCommand(0);
  if (second != 0)
  {
    switch(second)
    {
      case 0x01: return '1';
      case 0x02: return '2';
      case 0x04: return '3';
      case 0x08: return '4';
      case 0x10: return '5';
      case 0x20: return '6';
      case 0x40: return '7';
      case 0x80: return '8';
      default: return 0;
    }
  }
  else if (third != 0)
  {
    switch(third)
    {
      case 0x01: return '9';
      case 0x02: return '*';
      case 0x04: return '0';
      case 0x08: return '#';
      default: return 0;
    }
  }
  return 0;
}


