#include <M5Stack.h>
#include <time.h>
#include <Ticker.h>
#include <math.h>

#define pulsePin 35
#define volPin 36
#define freqPin 25
#define ampPin 26

Ticker heartRateSample;

//using structs may be execssive but it would allow me to track time and get a 
//proper slope for extrapolating and predicting current stress bpm.
struct heartRateStruct {
  int BPM;
  int time;
};

//int heartRate[50] = {NULL};
int samplesHR[10] = {NULL};
int babyVolume[50] = {NULL};
int stress[50] = {NULL};
int predictedHeartRate[50] = {NULL};
int errorValue[10] = {NULL};

struct heartRateStruct heartRate[50] = {NULL};



void setup() 
{
  M5.begin();
  M5.Power.begin();

  heartRateSample.attach_ms(1000, readHeartRate);

  //pinMode(pulsePin, INPUT);
  //pinMode(volPin, INPUT);
  //pinMode(freqPin, OUTPUT);
  //pinMode(ampPin, OUTPUT);
  
  M5.Lcd.print("Setup successful");
}



void readHeartRate() //Reads heart rate then stores it in an array, may change to struct to have time function
{

  int averageBPM = 0, averageClock = 0;

  for (int i = 50; i > 0; i--)
  {
    if (heartRate[i - 1].BPM != NULL)
    {
      heartRate[i] = heartRate[i - 1];
    }
  }

  for (int i = 0; i < 10000; i++)
  {
    //int pulseHigh = pulseIn(pulsePin, HIGH);
    //int pulseLow = pulseIn(pulsePin, LOW);
    //int pulseTotal= pulseHigh + pulseLow;
    int freq = 160; //1000000/pulseTotal; //should be between 0-240bpm, have to double check if its in bpm

    averageBPM = averageBPM + freq;
    averageClock = averageClock + clock();

  }

  heartRate[0].BPM = averageBPM/10000;
  heartRate[0].time = averageClock/10000;
  M5.Lcd.println(heartRate[0].time);
  M5.Lcd.println(heartRate[0].BPM);

  if (heartRate[1].time != NULL)
  {
    M5.Lcd.println(heartRate[0].time - heartRate[1].time);
  }

}



int readVolume() //Reads volume into an array, may change to struct to have time variable
{
  int volume = 0; // Will be changed from 0 to input pin reading
                  //Section above should include reading volume from pin and converts to between 0-100 and stores in volume then array
  for (int i = 50; i > 0; i--)
  {
    if (babyVolume[i - 1] != NULL)
    {
      babyVolume[i] = babyVolume[i - 1];
    }
  }

  babyVolume[0] = volume;
  return babyVolume[0];
}



void motorControl(int Frequency, int Amplitude) //All inputs should be between 0-255
{
  if (Frequency > 255 || Amplitude > 255 || Frequency < 0 || Amplitude < 0)
  {
    M5.Lcd.print("Invalid input (>255, <0)");
    return;
  }
  analogWrite(freqPin, 255 - Frequency);
  analogWrite(ampPin, 255 - Amplitude);
}



void errorCorrection()
{

}



int currentHeartRate() //Linear regression algorithm
{
  float time = 0, slope = 0, error = 0, intercept = 0;
  

  



  predictedHeartRate[0] = clock() * slope + error + intercept;
  return predictedHeartRate[0];
}



int stressLevel()
{
  for (int i = 50; i > 0; i--)
  {
    if (stress[i - 1] != NULL)
    {
      stress[i] = stress[i - 1];
    }
  }
  if (currentHeartRate() < 120)
  {
    stress[0] = readVolume() / 2;
  }
  else 
  {
    stress[0] = currentHeartRate() / 2.4;
  }
  //Need to figure out how amplitude and frequency of motors effects stress now
  //It may just search randomly I guess but this should be more informed atleast

}



void loop() 
{
  
  //stressLevel();
  

}
