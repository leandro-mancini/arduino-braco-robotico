#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <ArduinoJson.h>

const char* ssid = "Leandro & Debora";
const char* password = "miguel061321";

ESP8266WebServer server(80);

const int led = 2;

void setup(void){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/ANGULO", handleangulo);
  server.on("/BASE", handlebase);
  server.on("/ALTURA", handlealtura);
  server.on("/GARRA", handlegarra);
  server.on("/LIGAR", handleLigar);
  server.on("/DESLIGAR", handleDesligar);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  Wire.begin(D1, D2);
}

void loop(void){
  server.handleClient();
}

void handleLigar() {
  Serial.print("Ligar");

  Wire.beginTransmission(8);
  Wire.write("LIGAR");
  Wire.endTransmission();

  server.send(200, "application/json");
}

void handleDesligar() {
  Serial.print("Desligar");

  Wire.beginTransmission(8);
  Wire.write("DESLIGAR");
  Wire.endTransmission();

  server.send(200, "application/json");
}

void handleangulo() {  
  Serial.print("Angulo: ");
  Serial.println(server.arg(0));

  if (server.arg(0) == "up") {
    Wire.beginTransmission(8);
    Wire.write("{\"angulo\":\"up\"}");
    Wire.endTransmission();
  } else if (server.arg(0) == "down") {
    Wire.beginTransmission(8);
    Wire.write("{\"angulo\":\"down\"}");
    Wire.endTransmission();
  }
  
  server.send(200, "application/json");
}

void handlebase() {
  Serial.print("Base: ");
  Serial.println(server.arg(0));

  if (server.arg(0) == "left") {
    Wire.beginTransmission(8);
    Wire.write("{\"base\":\"left\"}");
    Wire.endTransmission();
  } else if (server.arg(0) == "right") {
    Wire.beginTransmission(8);
    Wire.write("{\"base\":\"right\"}");
    Wire.endTransmission();
  }
  
  server.send(200, "application/json");
}

void handlealtura() {
  Serial.print("Altura: ");
  Serial.println(server.arg(0));

  if (server.arg(0) == "up") {
    Wire.beginTransmission(8);
    Wire.write("{\"altura\":\"up\"}");
    Wire.endTransmission();
  } else if (server.arg(0) == "down") {
    Wire.beginTransmission(8);
    Wire.write("{\"altura\":\"down\"}");
    Wire.endTransmission();
  }
  
  server.send(200, "application/json");
}

void handlegarra() {
  Serial.print("Garra: ");
  Serial.println(server.arg(0));

  if (server.arg(0) == "left") {
    Wire.beginTransmission(8);
    Wire.write("{\"garra\":\"left\"}");
    Wire.endTransmission();
  } else if (server.arg(0) == "right") {
    Wire.beginTransmission(8);
    Wire.write("{\"garra\":\"right\"}");
    Wire.endTransmission();
  }
  
  server.send(200, "application/json");
}

void handleRoot() {
  digitalWrite(led, 1);

  String textoHTML;

  textoHTML += "<!DOCTYPE html>";
  textoHTML += "<html>";
  textoHTML += "<head>";
  textoHTML += "<title>Braço Robótico</title>";
  textoHTML += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/>";
  textoHTML += "<style>";
  textoHTML += "body { margin: 0; overflow: hidden; } .btn-control { height: 100vh; display: flex; align-items: center; } .btn-power { display: flex; position: fixed; bottom: 5%; left: 0; width:100%; }";
  textoHTML += "</style>";
  textoHTML += "</head>";
  textoHTML += "<body>";
  textoHTML += "<script>";

  textoHTML += "var isControle = false;";
  
  textoHTML += "function anguloup() {";
  textoHTML += "var xhttp = new XMLHttpRequest();";
  textoHTML += "xhttp.open('GET', 'ANGULO?value=up', true);";
  textoHTML += "xhttp.send(null);";
  textoHTML += "}";
  
  textoHTML += "function angulodown() {";
  textoHTML += "var xhttp = new XMLHttpRequest();";
  textoHTML += "xhttp.open('GET', 'ANGULO?value=down', true);";
  textoHTML += "xhttp.send(null);";
  textoHTML += "}";
  
  textoHTML += "function baseleft() {";
  textoHTML += "var xhttp = new XMLHttpRequest();";
  textoHTML += "xhttp.open('GET', 'BASE?value=left', true);";
  textoHTML += "xhttp.send(null);";
  textoHTML += "}";
  
  textoHTML += "function baseright() {";
  textoHTML += "var xhttp = new XMLHttpRequest();";
  textoHTML += "xhttp.open('GET', 'BASE?value=right', true);";
  textoHTML += "xhttp.send(null);";
  textoHTML += "}";

  textoHTML += "function alturaup() {";
  textoHTML += "var xhttp = new XMLHttpRequest();";
  textoHTML += "xhttp.open('GET', 'ALTURA?value=up', true);";
  textoHTML += "xhttp.send(null);";
  textoHTML += "}";
  
  textoHTML += "function alturadown() {";
  textoHTML += "var xhttp = new XMLHttpRequest();";
  textoHTML += "xhttp.open('GET', 'ALTURA?value=down', true);";
  textoHTML += "xhttp.send(null);";
  textoHTML += "}";
  
  textoHTML += "function garraleft() {";
  textoHTML += "var xhttp = new XMLHttpRequest();";
  textoHTML += "xhttp.open('GET', 'GARRA?value=left', true);";
  textoHTML += "xhttp.send(null);";
  textoHTML += "}";
  
  textoHTML += "function garraright() {";
  textoHTML += "var xhttp = new XMLHttpRequest();";
  textoHTML += "xhttp.open('GET', 'GARRA?value=right', true);";
  textoHTML += "xhttp.send(null);";
  textoHTML += "}";

  textoHTML += "function controleRemoto() {";
  textoHTML += "isControle = !isControle;";
  
  textoHTML += "if (!isControle) {";
  textoHTML += "var xhttp = new XMLHttpRequest();";
  textoHTML += "xhttp.open('GET', 'DESLIGAR', true);";
  textoHTML += "xhttp.send(null);";
  
  textoHTML += "document.getElementById('controle').innerHTML = 'Ligar controle remoto';";
  textoHTML += "} else {";
  textoHTML += "var xhttp = new XMLHttpRequest();";
  textoHTML += "xhttp.open('GET', 'LIGAR', true);";
  textoHTML += "xhttp.send(null);";
  
  textoHTML += "document.getElementById('controle').innerHTML = 'Desligar controle remoto';";
  textoHTML += "}";
  textoHTML += "}";
  
  textoHTML += "</script>";

  textoHTML += "<div class=\"btn-control\">";
  textoHTML += "<div style=\"text-align:center;width:50%;\">";
    textoHTML += "<button onclick=\"anguloup()\">ANGULO_UP</button><br><br>";
    textoHTML += "<button onclick=\"baseleft()\">BASE_LEFT</button>";
    textoHTML += "<button onclick=\"baseright()\">BASE_RIGHT</button><br><br>";
    textoHTML += "<button onclick=\"angulodown()\">ANGULO_DOWN</button>";
  textoHTML += "</div>";
  
  textoHTML += "<div style=\"text-align:center;width:50%;\">";
    textoHTML += "<button onclick=\"alturaup()\">ALTURA_UP</button><br><br>";
    textoHTML += "<button onclick=\"garraleft()\">GARRA_LEFT</button>";
    textoHTML += "<button onclick=\"garraright()\">GARRA_RIGHT</button><br><br>";
    textoHTML += "<button onclick=\"alturadown()\">ALTURA_DOWN</button>";
  textoHTML += "</div>";
  textoHTML += "</div>";

  textoHTML += "<div class=\"btn-power\">";
  textoHTML += "<div style=\"text-align:center;width:100%;\">";
  textoHTML += "<button onclick=\"controleRemoto()\" id=\"controle\">Ligar controle remoto</button>";
  textoHTML += "</div>";
  textoHTML += "</div>";
  
  textoHTML += "</body>";
  textoHTML += "</html>";
   
  server.send(200, "text/html", textoHTML);
  digitalWrite(led, 0);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}
