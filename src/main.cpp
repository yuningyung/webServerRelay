#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "U+Net43B0";
const char* password = "DD9D033347";
const int relay = 15;

ESP8266WebServer server(80);

void handleRoot();
void handleNotFound();
void RelayOn();
void RelayOff();

void setup() {
  // put your setup code here, to run once:
  pinMode(relay, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());

  if(MDNS.begin("miniwifi")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  
  server.on("/inline", []() {
    server.send(200, "text/plain", "Hello from the inline function\n");
  });

  server.on("/RelayOn", RelayOn);
  server.on("/RelayOff", RelayOff);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  MDNS.update();
  server.handleClient();
}

void handleRoot() {
  String message = (server.method() == HTTP_GET)?"GET":"POST";
  message += " " + server.uri() + "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + " : " + server.arg(i) + "\n";
  }
  message += "\nHello from ESP8266!\n";
  server.send(200, "text/plain", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}

void RelayOn() {
  digitalWrite(relay, HIGH);
  String message = "<html><head><meta charset=\"utf-8\"><title>Relay On</title></head>"
                  "<body>"
                  "<head>Relay On<head>"
                  "릴레이가 켜졌습니다."
                  "</body></html>";
  server.send(200, "text/html", message);
}

void RelayOff() {
  digitalWrite(relay, LOW);
  String message = "<html><head><meta charset=\"utf-8\"><title>Relay Off</title></head>"
                  "<body>"
                  "<head>Relay Off<head>"
                  "릴레이가 꺼졌습니다."
                  "</body></html>";
  server.send(200, "text/html", message);
}