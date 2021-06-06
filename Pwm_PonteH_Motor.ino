#include <ESP8266WiFi.h>

const char* ssid     = "LUCAS-BAESSE";
const char* password = "123456@lcs";

#define sleep   D4  //controle sleep
int motorEC    = D5;
int motorEB    = D6; 
int velocidade = 100; 

void setup() {
  
  Serial.begin(9600);
  pinMode(motorEC,   OUTPUT);
  pinMode(motorEB, OUTPUT);
  pinMode(sleep,     OUTPUT);

  digitalWrite(sleep, HIGH);

  
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

void loop() {
  
  analogWrite(motorEC, 300);
  digitalWrite(sleep, HIGH); //habilita
  
  analogWrite(motorEB, 300);
  digitalWrite(sleep, HIGH); //habilita

  delay(3000);
 
}
