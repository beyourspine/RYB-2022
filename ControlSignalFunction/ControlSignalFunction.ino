#include <M5Stack.h>

void setup()
{
  M5.begin();
  M5.Power.begin();
  Serial.begin(9600);

  M5.Lcd.print("Setup Successful");
}

int controlSignal(int Amplitude, int Frequency)
{
  M5.Lcd.print("Function Entered");
  if(Amplitude > 100 || Frequency > 100 || Amplitude < 0 || Frequency < 0) //This will be changed to 0-255 as the duty cycle is truncated causing a slight deviation from the desired value
  {
    Serial.println("Invalid parameters inputed (>100 or <0)");
    return 0;
  }
  analogWrite(19, (100-Amplitude)*2.55); //This will be changed to 0-255 instead of in increments from 0-100
  analogWrite(23, (100-Frequency)*2.55); //This will be changed to 0-255 instead of in increments from 0-100 
  M5.Lcd.print("Write Complete"); 
  return 0;
}

bool myTurn = 1;
int Amplitude = 0, Frequency = 0;

void loop()
{
  
  int input = Serial.parseInt();
  
  if(input > 0)
  {
    if(input > 100 || input < 0)
  {
    Serial.println("Invalid parameters inputed (>100 or <0)");
    return;
  }
    if(myTurn)
    {
      Frequency = input;
      myTurn = 0;
      Serial.println("New frequency set! Now waiting for Amplitude");
    }
    else
    {
      Amplitude = input;
      myTurn = 1;
      Serial.println("New Amplitude set! Now waiting for Frequency");
    }
    Serial.print("Amplitude DutyCycle: ");
    Serial.print( (100-Amplitude)*2.55);
    Serial.print(", Frequency DutyCycle:");
    Serial.println( (100-Frequency)*2.55);

    
    
    M5.Lcd.print("Loop Entered");
    controlSignal(Amplitude, Frequency); //Different values will be entered when this is called in the main program
    
  }
}
