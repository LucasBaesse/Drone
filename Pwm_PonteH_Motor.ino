#include <ESP8266WiFi.h>

const char* ssid     = "LUCAS-BAESSE";
const char* password = "123456@lcs";

#define sleepHB1    D4   //Habilita a saída do Modulo DRV8833V 
#define sleepHB2    D3   //Habilita a saída do Modulo DRV8833V 

int motorEC = D5; //Motor Esquerda Cima 
int motorEB = D6; //Motor Esquerda Baixo 
int motorDC = D7; //Motor Direita Cima
int motorDB = D8; //Motor Direita Baixo
 
int velocidadeGeral =   0; //Velocidade Geral 
int VelocidadeMotorEC = 650; //Velocidade Motor Esquerda Cima
int VelocidadeMotorEB = 650; //Velocidade Motor Esquerda Baixo
int VelocidadeMotorDC = 650; //Velocidade Motor Direita Cima
int VelocidadeMotorDB = 650; //Velocidade Motor Direita Baixo


void setup() {
  
  Serial.begin(9600);
  pinMode(motorEC, OUTPUT); //Motor Esquerda Cima 
  pinMode(motorEB, OUTPUT); //Motor Esquerda Baixo 
  pinMode(motorDC, OUTPUT); //Motor Direita Cima 
  pinMode(motorDB, OUTPUT); //Motor Direita Baixo  
  pinMode(sleepHB1,OUTPUT); //Sleep Habilita Saída
  pinMode(sleepHB2,OUTPUT); //Sleep Habilita Saída 

  digitalWrite(sleepHB1,HIGH); 
  digitalWrite(sleepHB2,HIGH);

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

void loop() {

  ControlarMotorEC( 0, VelocidadeMotorEC, motorEC );
  ControlarMotorEB( 0, VelocidadeMotorEB, motorEB );
  ControlarMotorDC( 0, VelocidadeMotorDC, motorDC ); 
  ControlarMotorDB( 0, VelocidadeMotorDB, motorDB ); 

  delay(3000);
 
}
