#include <M5Stack.h>

void setup()
{
  M5.begin();
  M5.Power.begin();
  ledcSetup(9, 1000, 8); //double check later but may require value in binary/hex
  ledcSetup(10, 1000, 8);
  ledcAttachPin(9, 9); //Amplitude Pin and Channel in that order
  ledcAttachPin(10, 10); //Frequency Pin and Channel in that order
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
  ledcWrite(9, Amplitude*2.55);
  ledcWrite(10, Frequency*2.55);
  M5.Lcd.print("Write Complete"); 
  return 0;
}

void loop()
{
  M5.Lcd.print("Loop Entered");
  controlSignal(10, 10); //Different values will be entered when this is called in the main program
}
