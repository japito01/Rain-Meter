//Last edited by jflores 22/06/2019

#include <MPU6050_tockn.h>
#include <Wire.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

MPU6050 mpu6050(Wire);
RH_ASK driver;

long timer = 0;
float Temp, AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float AccAngleX, AccAngleY, GyroAngleX, GyroAngleY, GyroAngleZ;
float PrevAccX, PrevAccY, PrevAccZ; 
float DiffAccX, DiffAccY, DiffAccZ;
String StringAccX = "accX : ", StringAccY = "\taccY : ", StringAccZ = "\taccZ : ";

//Change to desired tolerance levels
float ToleranceX = 0.05, ToleranceY = 0.05, ToleranceZ = 0.05 ;
//Change to desired temp limit
float TempLimit = 40;
//Change to desired transmit delay in ms
int DelayTx = 750;


void setup() {
  Serial.begin(9600);
  if (!driver.init())
      Serial.println("init failed");
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {
  mpu6050.update();


  //if statement for reading and output
  if(millis() - timer > 1000){
    
   
    Temp = mpu6050.getTemp();
    AccX = mpu6050.getAccX();
    AccY = mpu6050.getAccY();
    AccZ = mpu6050.getAccZ();

    //arrays can be used
    DiffAccX = AccX - PrevAccX;
    DiffAccY = AccY - PrevAccY;
    DiffAccZ = AccZ - PrevAccZ;

    abs(DiffAccX);
    abs(DiffAccY);
    abs(DiffAccZ);
    
    if( (DiffAccX > ToleranceX) || (DiffAccY > ToleranceY) || (DiffAccZ > ToleranceZ) ){  //Output to serial once reading difference is greater than tolerance
 
      //Prepare string for sending
      String Send = StringAccX + AccX + StringAccY + AccY + StringAccZ + AccZ;

      //Send actual message
      Serial.println(Send.c_str()); //Output to serial interface | For debugging purposes
      const char *msg = Send.c_str();
      driver.send((uint8_t *)msg, strlen(msg));
      driver.waitPacketSent();
      delay(DelayTx); //Higher delay greatly decreases chances of Rx not receiving data
   }   

    if (Temp > TempLimit){ //Output Temperature and Warning if Temp reaches assigned Limit
      Serial.print("Sensor Overheat Warning");
      Serial.print("temp : ");Serial.println(Temp);
    }
    timer = millis();



    PrevAccX = AccX; 
    PrevAccY = AccY;
    PrevAccZ = AccZ;
  }//end of if statement


  

}
