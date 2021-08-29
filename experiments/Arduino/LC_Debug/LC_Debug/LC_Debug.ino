#include <LCD_I2C.h>

const uint8_t addr = 0x27;
const uint8_t rows = 4;
const uint8_t cols = 20;

LCD_I2C lcd = LCD_I2C(addr, rows, cols);

long       lastChanged = 0;
const long refreshInterval = 100;

String nextString = "";
String partString = "";
String currentDisplay = "BrainSimII Debugger ";
                            
void printScreen()
{
  lcd.clear();
  lcd.print(currentDisplay);  
}

void printMessageToLCD(String msg)
{
  while (nextString.length() > cols)
  {
    partString = nextString.substring(0, cols);
    lcd.addLine(partString);
    nextString = nextString.substring(cols, cols - nextString.length());
  }
  lcd.addLine(nextString);
}

void setup() 
{
  Serial.begin(9600);
  lcd.init();
  lcd.switchBacklight(true);
  lcd.print(currentDisplay);
}

void loop() 
{
  // at least refresh the entire screen sometimes
  if (millis() - lastChanged > refreshInterval)
  {  
    while (Serial.available() > 0) 
    {
      nextString = Serial.readString();
      printMessageToLCD(nextString);
    }
    lastChanged = millis();
  }
}
