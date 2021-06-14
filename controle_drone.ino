#include <ESP8266WiFi.h>
#include <Wire.h>
#define MPU6050 0x68 // ENDEREÇO DO MPU6050
#define CONFIGACC 0x00 //  (+/-2g)0x00, (+/-4g)0x08, (+/- 8g)0x10,
#define CONFIGGYRO 0x00 // (250dps)0x00, (500dps)0x08, (1000dps)0x10
#define A_R 16384.0 // 32768/2
#define G_R 131 // 32768/250
#define RAD_A_DEG 57.295779 // 1 radiano = 57.295779°

#define sleepHB1    D4   //Habilita a saída do Modulo DRV8833V 
#define sleepHB2    D3   //Habilita a saída do Modulo DRV8833V 

long gyro_cal_X , gyro_cal_Y , gyro_cal_Z; // Declaração dos eixos de calibragem do giroscópio
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ; // Declaração das variáveis que vão receber os dados do sensor MPU
float Acc[2]; // D
float Gy[3];
float Angle[3];

long time_control;
float dt;

const char* ssid     = "LUCAS-BAESSE";
const char* password = "123456@lcs";


int motorEC = D5; //Motor Esquerda Cima 
int motorEB = D6; //Motor Esquerda Baixo 
int motorDC = D7; //Motor Direita Cima
int motorDB = D8; //Motor Direita Baixo
 
int velocidadeGeral =   0; //Velocidade Geral 
int VelocidadeMotorEC = 0; //Velocidade Motor Esquerda Cima
int VelocidadeMotorEB = 0; //Velocidade Motor Esquerda Baixo
int VelocidadeMotorDC = 0; //Velocidade Motor Direita Cima
int VelocidadeMotorDB = 0; //Velocidade Motor Direita Baixo

//CONFIGURAÇÃO DO WEB SERVER
IPAddress ip(192, 168, 43, 50);
IPAddress gateway(192, 168, 43, 1);
IPAddress subnet(255, 255, 255, 0);
WiFiServer server(80);
//FIM DA CONFIGURAÇÃO DO WEB SERVER

void setup() {
  
  Serial.begin(115200);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  server.begin();
  
  pinMode(motorEC, OUTPUT); //Motor Esquerda Cima 
  pinMode(motorEB, OUTPUT); //Motor Esquerda Baixo 
  pinMode(motorDC, OUTPUT); //Motor Direita Cima 
  pinMode(motorDB, OUTPUT); //Motor Direita Baixo  
  pinMode(sleepHB1,OUTPUT); //Sleep Habilita Saída
  pinMode(sleepHB2,OUTPUT); //Sleep Habilita Saída 

  digitalWrite(sleepHB1,HIGH); 
  digitalWrite(sleepHB2,HIGH);
  init_mpu();
  //wifi_connect();
}

/*
  void wifi_connect() {
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
} */


void ControlarMotorEC( int velocidadeGeral, int VelocidadeEC, int motorEC ){

  analogWrite(motorEC, VelocidadeMotorEC);
  //digitalWrite(sleep, HIGH); //habilita
  
}

void ControlarMotorEB(int velocidadeGeral, int VelocidadeEB, int motorEB ){

  analogWrite(motorEB, VelocidadeMotorEB);
  //digitalWrite(sleep, HIGH); //habilita
  
}

void ControlarMotorDC(int velocidadeGeral, int VelocidadeDC, int motorDC ){
  
  analogWrite(motorDC, VelocidadeMotorDC);
  //digitalWrite(sleep, HIGH); //habilita
  
}

void ControlarMotorDB(int velocidadeGeral, int VelocidadeDB, int motorDB ){

  analogWrite(motorDB, VelocidadeMotorDB);
  //digitalWrite(sleep, HIGH); //habilita
  
}
void desliga_motores(){
  ControlarMotorEC( 0, 0 , motorEC );
  ControlarMotorEB( 0, 0 , motorEB );
  ControlarMotorDC( 0, 0, motorDC ); 
  ControlarMotorDB( 0, 0, motorDB ); 
}
void loop() {
 /* 
    INÍCIO DAS TOMADAS DE DECISÃO COM OS DADOS RECEBIDOS DO MPU:
    SE O ÂNGULO DE INCLINAÇÃO DE UM DOS EIXOS FOR MAIOR OU IGUAL 
    A 5 OU -5, ENTÃO A POTÊNCIA DOS MOTORES É REAJUSTADA COM VALOR
    PROPORCIONAL À INCLINAÇÃO 
 */
  if(velocidadeGeral > 0){
    get_angles_mpu();
    if(Angle[0] >= 5 || Angle[1] >= 5 || Angle[0] <= -5 || Angle[1] <= -5){
      if(Angle[0] >= 5 && !(Angle[1] >= 5 || Angle[1] <= -5)){
        // DB e DC
        if(velocidadeGeral + Angle[0] < 1000){
            ControlarMotorDC( 0, VelocidadeMotorDC + Angle[0], motorDC );
            ControlarMotorDB( 0, VelocidadeMotorDB + Angle[0], motorDB );
          }
        else{
          if(velocidadeGeral - Angle[0] > 0){
            ControlarMotorEB( 0, VelocidadeMotorEB - Angle[0], motorEB );
            ControlarMotorEC( 0, VelocidadeMotorEC - Angle[0], motorEC );
            }
           else{
            desliga_motores();
           }
          }
      }
      else if(Angle[0] <= -5 && !(Angle[1] >= 5 || Angle[1] <= -5)){
        // EB e EC
        if(velocidadeGeral + Angle[0] < 1000){
            ControlarMotorEB( 0, VelocidadeMotorEB + Angle[0], motorEB );
            ControlarMotorEC( 0, VelocidadeMotorEC + Angle[0], motorEC );
          }
        else{
          if(velocidadeGeral - Angle[0] > 0){
            ControlarMotorDC( 0, VelocidadeMotorDC - Angle[0], motorDC );
            ControlarMotorDB( 0, VelocidadeMotorDB - Angle[0], motorDB );
            }
           else{
            desliga_motores();
           }
          }
      }
      else if(Angle[1] <= -5 && !(Angle[0] >= 5 || Angle[0] <= -5)){
        // DB e EB
        if(velocidadeGeral + Angle[1] < 1000){
            ControlarMotorEB( 0, VelocidadeMotorEB + Angle[1], motorEB );
            ControlarMotorDB( 0, VelocidadeMotorDB + Angle[1], motorDB );
          }
        else{
          if(velocidadeGeral - Angle[1] > 0){
            ControlarMotorDC( 0, VelocidadeMotorDC - Angle[1], motorDC );
            ControlarMotorEC( 0, VelocidadeMotorEC - Angle[1], motorEC );
            }
           else{
            desliga_motores();
           }
          }
      }
      else if(Angle[1] >= 5 && !(Angle[0] >= 5 || Angle[0] <= -5)){
        // DC e EC
        if(velocidadeGeral + Angle[1] < 1000){
            ControlarMotorDC( 0, VelocidadeMotorDC + Angle[1], motorDC );
            ControlarMotorEC( 0, VelocidadeMotorEC + Angle[1], motorEC );
          }
        else{
          if(velocidadeGeral - Angle[1] > 0){
            ControlarMotorDB( 0, VelocidadeMotorDB - Angle[1], motorDB );
            ControlarMotorEB( 0, VelocidadeMotorEB - Angle[1], motorEB );
            }
           else{
            desliga_motores(); 
           }
          }
      }
      else if(Angle[0] >= 5 && Angle[1] >= 5){
        //DC
        int modulo = sqrt(pow(Angle[0],2)+pow(Angle[1],2));
        if(velocidadeGeral + modulo < 1000){
            ControlarMotorDC( 0, VelocidadeMotorDC + modulo, motorDC );
          }
        else{
          if(velocidadeGeral - modulo > 0){
            ControlarMotorEB( 0, VelocidadeMotorEB - modulo, motorEB );
            ControlarMotorDB( 0, VelocidadeMotorDB - modulo, motorDB );
            ControlarMotorEC( 0, VelocidadeMotorEC - modulo, motorEC );
            }
           else{
            desliga_motores(); 
           }

          }
        }
      else if(Angle[0] <= -5 && Angle[1] >= 5){
        //EC
        int modulo = sqrt(pow(Angle[0],2)+pow(Angle[1],2));
        if(velocidadeGeral + modulo < 1000){
            ControlarMotorEC( 0, VelocidadeMotorEC + modulo, motorEC );
            
          }
        else{
          if(velocidadeGeral - modulo > 0){
            ControlarMotorDC( 0, VelocidadeMotorDC - modulo, motorDC );
            ControlarMotorDB( 0, VelocidadeMotorDB - modulo, motorDB );
            ControlarMotorEB( 0, VelocidadeMotorEB - modulo, motorEB );
            }
           else{
            desliga_motores(); 
           }
          }
        }
      else if(Angle[1] <= -5 && Angle[0] >= 5){
        //DB
        int modulo = sqrt(pow(Angle[0],2)+pow(Angle[1],2));
        if(velocidadeGeral + modulo < 1000){
            ControlarMotorDB( 0, VelocidadeMotorDB + modulo, motorDB );
            
          }
        else{
          if(velocidadeGeral - modulo > 0){
            ControlarMotorDC( 0, VelocidadeMotorDC - modulo, motorDC );
            ControlarMotorEB( 0, VelocidadeMotorEB - modulo, motorEB );
            ControlarMotorEC( 0, VelocidadeMotorEC - modulo, motorEC );
            }
           else{
            desliga_motores();
           }
          }
        }
      else{/*Angle[1] <= -5 && Angle[0] <= 0*/
        //EB
        int modulo = sqrt(pow(Angle[0],2)+pow(Angle[1],2));
        if(velocidadeGeral + modulo < 1000){
            ControlarMotorEB( 0, VelocidadeMotorEB + modulo, motorEB );
          }
        else{
          if(velocidadeGeral - modulo > 0){
            ControlarMotorDC( 0, VelocidadeMotorDC - modulo, motorDC );
            ControlarMotorDB( 0, VelocidadeMotorDB - modulo, motorDB );
            ControlarMotorEC( 0, VelocidadeMotorEC - modulo, motorEC );
            }
           else{
            desliga_motores();
           }
          }
       }
       print_angles();
       print_velocidade();
      delay(1000);
      }
   }
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  while (!client.available()) {
    delay(1);
  }

  String req = client.readStringUntil('\r');
  client.flush();

  String buf = "";

  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  buf += "<head><title>Drone de Segurança</title><style> body{background-color: rgb(247, 247, 247);} h1{color: rgb(0, 0, 0) } img {width: 90px; height: 90px; display:inline-block}</style></head>";
  buf += "<div style=background-color: rgb(255, 126, 20);><h1>Modos de Voo</h1></div>";
  buf += "<a href=\"?function=up\"><button><img src='https://image.flaticon.com/icons/png/512/120/120891.png'></button></a>";
  buf += "<a href=\"?function=down\"><button><img src=https://img-premium.flaticon.com/png/512/120/120890.png?token=exp=1623199028~hmac=be09e81e3bd7668704655a3bb70e4f3c'></button></a>";
  buf += "<div style='position: fixed; left: 0; bottom: 0; width: 100%; background-color: rgb(255, 126, 20); text-align: center;''>&#169;</div>";
  buf += "</html>\n";
  
  client.print(buf);
  client.flush();

  //RESPONDE À REQUISIÇÃO DO COMANDO DO SITE. 

  if (req.indexOf("up") != -1) {
    if(velocidadeGeral <= 1000 || velocidadeGeral >= 0){
        velocidadeGeral += 200;
        ControlarMotorEC( 0, VelocidadeMotorEC + 200, motorEC );
        ControlarMotorEB( 0, VelocidadeMotorEB + 200, motorEB );
        ControlarMotorDC( 0, VelocidadeMotorDC + 200, motorDC ); 
        ControlarMotorDB( 0, VelocidadeMotorDB + 200, motorDB );        
      }
  else if (req.indexOf("down") != -1) {
    if(velocidadeGeral > 200){
        velocidadeGeral -= 200;
        ControlarMotorEC( 0, VelocidadeMotorEC - 200, motorEC );
        ControlarMotorEB( 0, VelocidadeMotorEB - 200, motorEB );
        ControlarMotorDC( 0, VelocidadeMotorDC - 200, motorDC ); 
        ControlarMotorDB( 0, VelocidadeMotorDB - 200, motorDB );        
      }
      else {
        ControlarMotorEC( 0, 0 , motorEC );
        ControlarMotorEB( 0, 0 , motorEB );
        ControlarMotorDC( 0, 0, motorDC ); 
        ControlarMotorDB( 0, 0, motorDB ); 
      }
      print_velocidade();
  }
}
  else {
    //REQUISIÇÃO INVALIDA 
    client.stop();
  }
}
void init_mpu(){
  Wire.begin(4,5);
  config_mpu(); 
  while(!check_mpu()){init_mpu();}
  gyro_calibrator_mpu(2000);
  delay(1000);
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
void print_angles(){
  Serial.print("Pitch: ");
  Serial.print(Angle[0]);
  Serial.print("\t");
  Serial.print("Roll: ");
  Serial.println(Angle[1]); 
}
void print_velocidade(){
  Serial.print("EC: ");
  Serial.print(motorEC);
  Serial.print("DC: ");
  Serial.print(motorDC);
  Serial.print("EB: ");
  Serial.print(motorEB);
  Serial.print("DB: ");
  Serial.println(motorDB);
}
