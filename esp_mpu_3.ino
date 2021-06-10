#include <Wire.h>
#define MPU6050 0x68
#define CONFIGACC 0x00 //(+/- 8g)0x10, (+/-2g)0x00, (+/-4g)0x08
#define CONFIGGYRO 0x00 //(250dps)0x00, (500dps)0x08, (1000dps)0b00010000
#define A_R 16384.0 // 32768/2
#define G_R 131 // 32768/250
#define RAD_A_DEG 57.295779

long gyro_cal_X , gyro_cal_Y , gyro_cal_Z;
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
float Acc[2];
float Gy[3];
float Angle[3];

long time_control;
float dt;

void setup() {
  Serial.begin(115200);
  init_mpu(); 
}
void init_mpu(){
  Wire.begin(4,5);
  config_mpu(); 
  while(!check_mpu()){init_mpu();}
  gyro_calibrator_mpu(2000);
}
void loop(){
  get_angles_mpu();
  }
void get_angles_mpu() {
  read_mpu();
  Acc[1] = atan(-1*(AcX/A_R)/sqrt(pow((AcY/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
  Acc[0] = atan((AcY/A_R)/sqrt(pow((AcX/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
  Gy[0] -= gyro_cal_X;
  Gy[1] -= gyro_cal_Y;
  Gy[2] -= gyro_cal_Z;
  
  Gy[0] = GyX/G_R;
  Gy[1] = GyY/G_R;
  Gy[2] = GyZ/G_R;

  dt = (millis() - time_control) / 1000.0;
  time_control = millis();

  Angle[0] = 0.98 *(Angle[0]+Gy[0]*dt) + 0.02*Acc[0];
  Angle[1] = 0.98 *(Angle[1]+Gy[1]*dt) + 0.02*Acc[1];
 
  Angle[2] = Angle[2]+Gy[2]*dt;
  Serial.print("Pitch: ");
  Serial.print(Angle[0]);
  Serial.print("\t");
  Serial.print("Roll: ");
  Serial.println(Angle[1]);
  delay(10);
}
void config_mpu(){
  Wire.beginTransmission(MPU6050);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);
// Config. Gyro. 
  Wire.beginTransmission(MPU6050);
  Wire.write(0x1B);
  Wire.write(CONFIGGYRO);
  Wire.endTransmission(true);
// Config. Acc. (+/- 8g)0x10, (+/-2g)0x00, (+/-4g)0x08
  Wire.beginTransmission(MPU6050);
  Wire.write(0x1C);
  Wire.write(CONFIGACC);
  Wire.endTransmission(true);
}
void find_mpu(){
  Wire.beginTransmission(MPU6050);
  int data = Wire.endTransmission(true);
  if(data == 0){
    Serial.print("Dispositivo encontrado no endereço: 0x");
    Serial.println(MPU6050, HEX);
  }
  else{
    Serial.println("Dispositivo não encontrado!");
  }
}
bool check_mpu(){
  find_mpu();  
  int data; 
  Wire.beginTransmission(MPU6050);   
  Wire.write(0x75);                      
  Wire.endTransmission(false);         
  Wire.requestFrom(MPU6050, 1);       
  data = Wire.read();    
  if(data == 104){
    Serial.println("MPU6050 Dispositivo respondeu OK! (104)");
    Wire.beginTransmission(MPU6050);   
    Wire.write(0x6B);                      
    Wire.endTransmission(false);         
    Wire.requestFrom(MPU6050, 1);       
    data = Wire.read();    
    if(data == 64) Serial.println("MPU6050 em modo SLEEP! (64)");
    else Serial.println("MPU6050 em modo ACTIVE!"); 
    return true;
  }
  else {
    Serial.println("Verifique dispositivo - MPU6050 NÃO disponível!");
    return false;
    }
}
void read_mpu(){
  Wire.beginTransmission(MPU6050);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050,6, true);
  AcX=Wire.read()<<8|Wire.read(); 
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  Wire.beginTransmission(MPU6050);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050,6,true); 
  GyX=Wire.read()<<8|Wire.read(); 
  GyY=Wire.read()<<8|Wire.read();
  GyZ=Wire.read()<<8|Wire.read();
}
void gyro_calibrator_mpu(int count_entries){
  for(int i=0; i<count_entries; i++){
    read_mpu();
    gyro_cal_X += GyX;
    gyro_cal_Y += GyY;
    gyro_cal_Z += GyZ;
    delay(1);
    Serial.println("calibrando . . .");
  }
  gyro_cal_X /= count_entries;
  gyro_cal_Y /= count_entries;
  gyro_cal_Z /= count_entries;
}
