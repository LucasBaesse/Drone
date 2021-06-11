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

IPAddress ip(192, 168, 43, 50);
IPAddress gateway(192, 168, 43, 1);
IPAddress subnet(255, 255, 255, 0);
WiFiServer server(80);

void setup() {
  
  Serial.begin(9600);
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

  ControlarMotorEC( 0, VelocidadeMotorEC, motorEC );
  ControlarMotorEB( 0, VelocidadeMotorEB, motorEB );
  ControlarMotorDC( 0, VelocidadeMotorDC, motorDC ); 
  ControlarMotorDB( 0, VelocidadeMotorDB, motorDB );   
  }
    else if (req.indexOf("down") != -1) {
      
    ControlarMotorEC( 0, VelocidadeMotorEC, motorEC );
    ControlarMotorEB( 0, VelocidadeMotorEB, motorEB );
    ControlarMotorDC( 0, VelocidadeMotorDC, motorDC ); 
    ControlarMotorDB( 0, VelocidadeMotorDB, motorDB ); 
  }
  else {
    //REQUISIÇÃO INVALIDA 
    client.stop();
  }
}
