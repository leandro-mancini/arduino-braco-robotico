#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <ArduinoJson.h>

IPAddress    apIP(192,168,0,100);  // Defining a static IP address: local & gateway
                                    // Default IP in AP mode is 192.168.4.1
                                    
/* This are the WiFi access point settings. Update them to your likin */
const char *ssid = "Braço Robótico";
const char *password = "123456789";

// Define a web server at port 80 for HTTP
ESP8266WebServer server(80);

const int led = 2;

void handleReset() {
  Serial.print("Reset");

  Wire.beginTransmission(8);
  Wire.write("RESET");
  Wire.endTransmission();
  
  server.send(200, "application/json");
}

void handleGravar() {
  Serial.print("Gravar");

  Wire.beginTransmission(8);
  Wire.write("GRAVAR");
  Wire.endTransmission();
  
  server.send(200, "application/json");
}

void handleplay() {
  Serial.print("Play");

  Wire.beginTransmission(8);
  Wire.write("PLAY");
  Wire.endTransmission();
  
  server.send(200, "application/json");
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
  textoHTML =+ "<meta charset=\"utf-8\" />";
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

  textoHTML += "function gravar() {";
  textoHTML += "var xhttp = new XMLHttpRequest();";
  textoHTML += "xhttp.open('GET', 'GRAVAR', true);";
  textoHTML += "xhttp.send(null);";
  textoHTML += "}";

  textoHTML += "function play() {";
  textoHTML += "var xhttp = new XMLHttpRequest();";
  textoHTML += "xhttp.open('GET', 'PLAY', true);";
  textoHTML += "xhttp.send(null);";
  textoHTML += "}";

  textoHTML += "function reset() {";
  textoHTML += "var xhttp = new XMLHttpRequest();";
  textoHTML += "xhttp.open('GET', 'RESET', true);";
  textoHTML += "xhttp.send(null);";
  textoHTML += "}";

  textoHTML += "function controleRemoto() {";
  textoHTML += "isControle = !isControle;";
  
  textoHTML += "if (!isControle) {";
  textoHTML += "var xhttp = new XMLHttpRequest();";
  textoHTML += "xhttp.open('GET', 'DESLIGAR', true);";
  textoHTML += "xhttp.send(null);";
  
  textoHTML += "document.getElementById('controle').innerHTML = 'Ligar';";
  textoHTML += "} else {";
  textoHTML += "var xhttp = new XMLHttpRequest();";
  textoHTML += "xhttp.open('GET', 'LIGAR', true);";
  textoHTML += "xhttp.send(null);";
  
  textoHTML += "document.getElementById('controle').innerHTML = 'Desligar';";
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
  textoHTML += "<button onclick=\"reset()\" id=\"reset\" style=\"margin:0 30px;\">Reset</button>";
  textoHTML += "<button onclick=\"controleRemoto()\" id=\"controle\">Ligar</button>";
  textoHTML += "<button onclick=\"gravar()\" id=\"gravar\" style=\"margin:0 30px;\">Gravar</button>";
  textoHTML += "<button onclick=\"play()\" id=\"play\">Play</button>";
  textoHTML += "</div>";
  textoHTML += "</div>";
  
  textoHTML += "</body>";
  textoHTML += "</html>";
   
  server.send(200, "text/html", textoHTML);
  digitalWrite(led, 0);
}

void handleNotFound() {
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

void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

  Wire.begin(D1, D2);
  
  Serial.begin(9600);
  Serial.println();
  Serial.println("Configuring access point...");

  //set-up the custom IP address
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00  
  
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
  server.on("/", handleRoot);
  server.on("/ANGULO", handleangulo);
  server.on("/BASE", handlebase);
  server.on("/ALTURA", handlealtura);
  server.on("/GARRA", handlegarra);
  server.on("/LIGAR", handleLigar);
  server.on("/DESLIGAR", handleDesligar);
  server.on("/GRAVAR", handleGravar);
  server.on("/PLAY", handleplay);
  server.on("/RESET", handleReset);

  server.onNotFound(handleNotFound);
  
  server.begin();
  
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
