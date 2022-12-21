#include <Statistical.h>
#include <M5Stack.h>
#include <time.h>
#include <Ticker.h>
#include <math.h>

#define pulsePin 35 //Input Pin
#define volPin 36 //Input Pin
#define freqPin 25 //Output Pin
#define ampPin 26 //Output Pin 
#define samplePeriod 250 //Milliseconds between samples
#define timeOffset 2000 //Milliseconds delay of prediction

Ticker heartRateSample;
Ticker errorCorrection;

//using structs may be execssive but it would allow me to track time and get a 
//proper slope for extrapolating and predicting current stress bpm.
//struct heartRateStruct {
//  float BPM;
//  uint8_t time;
//};

//struct heartRateStruct heartRate[50] = {NULL};

uint8_t heartRateTime[50] = {NULL};
float heartRateBPM[50] = {NULL};
//int heartRate[50] = {NULL};
//float samplesHR[10] = {NULL};
int babyVolume[50] = {NULL};
int stress[50] = {NULL};
float predictedHeartRate[50] = {NULL};
float errorValue[10] = {NULL};
int finalError = 0;



void setup() 
{
  M5.begin();
  M5.Power.begin();

  heartRateSample.attach_ms(samplePeriod, readHeartRate);
  errorCorrection.attach_ms(timeOffset, errorCorrection);

  //pinMode(pulsePin, INPUT);
  //pinMode(volPin, INPUT);
  //pinMode(freqPin, OUTPUT);
  //pinMode(ampPin, OUTPUT);
  
  M5.Lcd.print("Setup successful");
}



void readHeartRate() //Reads heart rate then stores it in an array, may change to struct to have time function
{

  float averageBPM = 0; 
  uint8_t averageClock = 0;

  for (int i = 50; i > 0; i--)
  {
    if (heartRateBPM[i - 1] != NULL)
    {
      heartRateBPM[i] = heartRateBPM[i - 1];
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

  heartRateBPM[0] = averageBPM/10000;
  heartRateTime[0] = averageClock/10000;
  //M5.Lcd.println(heartRateTime[0]);
  //M5.Lcd.println(heartRateBPM[0]);

  if (heartRateTime[1] != NULL)
  {
    //M5.Lcd.println(heartRateTime[0] - heartRateTime[1]);
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

  for (int i = 10; i > 0; i--)
  {
    if (errorValue[i - 1] != NULL)
    {
      errorValue[i] = errorValue[i - 1];
    }
  }

  errorValue[0] = heartRateBPM[0] - predictedHeartRate[0]

  for (int i = 0; i < 10; i++)
  {
    int q = 0, t = 0;

    if (errorValue[i] != NULL)
    {
      q = q + errorValue[i];
      t = t + 1;
    }

    if (i == 9)
    {
      finalError = q / t;
    }
  }
}



int currentHeartRate() //Linear regression algorithm
{

  Linear_Regression<uint8_t, float> Regression(heartRateTime, heartRateBPM, sizeof(heartRateTime) / sizeof(heartRateTime[0]));
  
  for (int i = 50; i > 0; i--)
  {
    if (predictedHeartRate[i - 1] != NULL)
    {
      predictedHeartRate[i] = predictedHeartRate[i - 1];
    }
  }
  
  predictedHeartRate[0] = (clock() + timeOffset) * Regression.Slope() + Regression.Offset() + finalError; //Needs error correction needs tweaking or maybe its not needed
  M5.Lcd.println(Regression.R2());
  M5.Lcd.print(predictedHeartRate[0]);
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
  if (heartRateBPM[0] <= 120 || readVolume < 50)
  {
    stress[0] = readVolume() / 2;
  }
  else 
  {
    stress[0] = heartRateBPM[0] / 2.4;
  }
  //Need to figure out how amplitude and frequency of motors effects stress now
  //It may just search randomly I guess but this should be more informed atleast

}



void loop() 
{
  currentHeartRate();
  //stressLevel();

  delay(5000);
  M5.Lcd.clear();
  M5.Lcd.setCursor(0,0);
}
