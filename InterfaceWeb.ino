#include <ESP8266WiFi.h>

const char* ssid = "nome_da_sua_rede_wifi";
const char* password = "senha_da_sua_rede_wifi";


IPAddress ip(192, 168, 43, 50);
IPAddress gateway(192, 168, 43, 1);
IPAddress subnet(255, 255, 255, 0);
WiFiServer server(80);

void setup() {
   
  //Conectando rede Wifi
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  //Verificando se esta conectado,
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  //Iniciando o servidor Web
  server.begin();
}

//Funcao que sera executada indefinidamente enquanto o ESP8266 estiver ligado.
void loop() {
  //Verificando se o servidor esta pronto.
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  //Verificando se o servidor recebeu alguma requisicao
  while (!client.available()) {
    delay(1);
  }

  //Obtendo a requisicao vinda do browser
  String req = client.readStringUntil('\r');
  client.flush();

  //Iniciando o buffer que ira conter a pagina HTML que sera enviada para o browser.
  String buf = "";

  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  buf += "<head><title>Drone de Segurança</title><style> body{background-color: rgb(247, 247, 247);} h1{color: rgb(0, 0, 0) } img {width: 90px; height: 90px; display:inline-block}</style></head>";
  buf += "<div style=background-color: rgb(255, 126, 20);><h1>Modos de Voo</h1></div>";
  buf += "<a href=\"?function=up\"><button><img src='https://image.flaticon.com/icons/png/512/120/120891.png'></button></a>";
  buf += "<a href=\"?function=down\"><button><img src=https://img-premium.flaticon.com/png/512/120/120890.png?token=exp=1623199028~hmac=be09e81e3bd7668704655a3bb70e4f3c'></button></a>";
  buf += "<div style='position: fixed; left: 0; bottom: 0; width: 100%; background-color: rgb(255, 126, 20); text-align: center;''>&#169;</div>";
  buf += "</html>\n";

  //Enviando para o browser a 'pagina' criada.
  client.print(buf);
  client.flush();

  //Analisando a requisicao recebida para decidir se liga ou desliga o LED
  if (req.indexOf("up") != -1)
  {
    digitalWrite(2, 0);
  }
  else if (req.indexOf("down") != -1)
  {
    digitalWrite(2, 1);
  }
  else
  {
    //Requisicao invalida!
    client.stop();
  }
}
