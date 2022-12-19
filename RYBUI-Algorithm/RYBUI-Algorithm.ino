#include <M5Stack.h>

#Define pulsePin 13;
#Define freqPin 23;
#Define ampPin 19;
#Define volPin NULL;

//using structs may be execssive but it would allow me to track time and get a 
//proper slope for extrapolating and predicting current stress bpm.
struct heartRateStruct {
  int BPM
  int time
};

//int heartRate[50] = NULL;
int babyVolume[50] = NULL;
int stress[50] = NULL;
heartRate[50] = NULL;

void setup() 
{
  M5.begin();
  M5.Power.begin();
  Serial.begin(9600);

  M5.Lcd.print("Setup successful");
}

int readHeartRate() //Reads heart rate then stores it in an array, may change to struct to have time function
{
  int pulseHigh = pulseIn(pulsePin, HIGH);
  int pulseLow = pulseIn(pulsePin, LOW);
  int pulseTotal= pulseHigh + pulseLow;
  int freq = 1000000/pulseTotal; //should be between 0-240bpm, have to double check if its in bpm
  for (i = 50; i >= 0; i++)
  {
    if (heartRate[i] != NULL)
    {
      heartRate[i] = heartRate[i - 1];
    }
  }
  heartRate[0] = freq; //returns in bpm I think
  return heartRate[0];
}

int readVolume() //Reads volume into an array, may change to struct to have time variable
{
  //Section above should include reading volume from pin and converts to between 0-100 and stores in volume then array
  for (i = 50; i >= 0; i++)
  {
    if (babyVolume[i] != NULL)
    {
      babyVolume[i] = babyVolume[i - 1];
    }
  }
  babyVolume[0] = volume;
  return babyVolume[0];
}

void serialPrintCurrentHR() //serial port command, probably useless but might come in handy
{
  Serial.println(heartRate[0]);
}

void updateMotor(int Frequency, int Amplitude) //All inputs should be between 0-255
{
  if (Frequency > 255 || Amplitude > 255 || Frequency < 0 || Amplitude < 0)
  {
    M5.Lcd.print("Invalid input (>255, <0)");
    return;
  }
  analogWrite(freqPin, 255 - Frequency);
  analogWrite(ampPin, 255 - Amplitude);
}

void currentHeartRate() //returns in bpm
{
  readHeartRate()
}

int stressAlgorithm(int stress)
{
  if(currentHeartRate() < 120)
  {
    stress = readVolume(volPin) / 2;
  }
  else if
  {
    stress = currentHeartRate() / 2.4;
  }
  //Need to figure out how amplitude and frequency of motors effects stress now
  //It may just search randomly I guess but this should be more informed atleast

}

void loop() 
{
  if ()

  serialPrintCurrentHR();

}
