#include <M5Stack.h>

void setup()
{
  M5.begin();
  M5.Power.begin();

  M5.Lcd.print("Setup Successful");
}

int controlSignal(int Amplitude, int Frequency)
{
  M5.Lcd.print("Function Entered");
  if(Amplitude > 100 || Frequency > 100 || Amplitude < 0 || Frequency < 0)
  {
    M5.Lcd.print("Invalid parameters inputed (>100 or <0)");
    return 0;
  }
  analogWrite(9, Amplitude*2.55);
  analogWrite(10, Frequency*2.55);
  M5.Lcd.print("Write Complete"); 
  return 0;
}

void loop()
{
  M5.Lcd.print("Loop Entered");
  controlSignal(10, 10); //Different values will be entered when this is called in the main program
}
