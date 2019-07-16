#include <timer.h> //info at https://github.com/contrem/arduino-timer
#include <CircularBuffer.h> //info at https://github.com/rlogiacco/CircularBuffer

unsigned int RRThisSecond=0, CurrentLevel=0, RainRatePerMinute=0, temp=0, x;
unsigned int Flood[4] = {47,49,51,53}; //Red , Orange, Yellow, Green
unsigned int Landslide[4] = {39,41,43,45}; //Red, Orange, Yellow, Green
int Alarm = 50, Light = 52;//Red1=3, Orange1=4, Green=5, Yellow=6, Orange=7, Red=8, strobe=9, alarm_trigger=10, alarm=11; //Pin numbers
float k=0.2, Depth; //k = Depth formula constant
CircularBuffer<int,60> RainRate;//declare RainRate as int with CircularBuffer

auto TimerSecond = timer_create_default(); //set non-blocking timer
auto TimerMinute = timer_create_default();

void setup() {
  Serial.begin(9600);
  TimerSecond.every(1000,StoreMeasurement);
  TimerMinute.every(60000,Calc_Landslide);
  pinMode(Flood, OUTPUT);
  pinMode(Alarm, OUTPUT);pinMode(Light, OUTPUT); //pinMode(strobe, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), SensorDetect, FALLING); //setup interrupt on pin 2 - Rain Meter tested
}

void StoreMeasurement(){
//Serial.println("Timer OK"); //debug purposes
RainRate.push(RRThisSecond); //Shift buffer

Update_Rate(); //update Rate for the past 60 seconds

RRThisSecond = 0; //Return count to 0
  
}

void SensorDetect(){ //tested
//  Serial.println(RainRatePerMinute); //debug purposes
  RRThisSecond++;
  CurrentLevel++;
  Serial.println(RRThisSecond);
}

void Update_Rate(){ //updates rate in the last 60 seconds
  for (x=0;x<60;x++){
    temp = temp + RainRate[x];  
  }
  RainRatePerMinute = temp;
  Serial.println(RainRatePerMinute);
  temp = 0;
}

void Show_RainRateLevel(){ //Reflect Rain Rate
  if (RainRatePerMinute < 10){
//    digitalWrite(strobe, LOW);
    digitalWrite(Flood[3], HIGH); digitalWrite(Flood[2], LOW); digitalWrite(Flood[1], LOW); digitalWrite(Flood[0], LOW);//Green 
  }
  else if(RainRatePerMinute >= 10 && RainRatePerMinute < 15){
    digitalWrite(Flood[3], LOW); digitalWrite(Flood[2], HIGH); digitalWrite(Flood[1], LOW); digitalWrite(Flood[0], LOW);//Yellow
  }
  else if(RainRatePerMinute >= 15 && RainRatePerMinute < 20){
    digitalWrite(Flood[3], LOW); digitalWrite(Flood[2], LOW); digitalWrite(Flood[1], HIGH); digitalWrite(Flood[0], LOW);//Orange
  }
  else{
    digitalWrite(Flood[3], LOW); digitalWrite(Flood[2], LOW); digitalWrite(Flood[1], LOW); digitalWrite(Flood[0], HIGH);//Red
  }
  
}

void Calc_Landslide(){
  //Calculate Depth
  Depth = k*RainRatePerMinute;

  //Reflect Depth
  if (Depth < 0.5){
//    digitalWrite(strobe, LOW);
    digitalWrite(Landslide[3], HIGH); digitalWrite(Landslide[2], LOW); digitalWrite(Landslide[1], LOW); digitalWrite(Landslide[0], LOW);//Green 
  }
  else if(Depth >= 0.5 && Depth < 1){
    digitalWrite(Landslide[3], LOW); digitalWrite(Landslide[2], HIGH); digitalWrite(Landslide[1], LOW); digitalWrite(Landslide[0], LOW);//Yellow
  }
  else if(Depth >= 1 && Depth < 1.5){
    digitalWrite(Landslide[3], LOW); digitalWrite(Landslide[2], LOW); digitalWrite(Landslide[1], HIGH); digitalWrite(Landslide[0], LOW);//Orange
  }
  else{
    digitalWrite(Landslide[3], LOW); digitalWrite(Landslide[2], LOW); digitalWrite(Landslide[1], LOW); digitalWrite(Landslide[0], HIGH);//Red
    digitalWrite(Light, HIGH); digitalWrite(Alarm, HIGH);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  TimerSecond.tick();
  TimerMinute.tick();
  Show_RainRateLevel();

  
}
