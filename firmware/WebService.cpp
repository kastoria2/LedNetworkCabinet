#include "WebService.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#include "Animations.h"

#define DEFAULT_SSID "den_24"
char ssid[80] = DEFAULT_SSID;

#define DEFAULT_PASSWORD "Wal1_den_24"
char password[80] = DEFAULT_PASSWORD;

#define DEFAULT_AP "MY_AP"
char ap[80] = DEFAULT_SSID;

#define DEFAULT_AP_PASSWORD "MY_AP_PASSWORD"
char ap_password[80] = DEFAULT_PASSWORD;

#define DEFAULT_HOSTNAME "esp32sd"
char hostname[80] = DEFAULT_HOSTNAME;

StaticJsonDocument<512> doc;

#define DBG_OUTPUT_PORT Serial
WebServer server(80);

void updateWebService(void) {
  server.handleClient();
}

void returnOK() {
  server.send(200, "text/plain", "");
}

void returnFail(String msg) {
  server.send(500, "text/plain", msg + "\r\n");
}

void printDirectory() {
  server.send(200, "text/plain", "zens");
}

void handleNotFound() {
  String message = "URI not found!\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  DBG_OUTPUT_PORT.print(message);
}

void updateColor() {

    String redStr = server.arg("red");
    String greenStr = server.arg("green");
    String blueStr = server.arg("blue");

    uint8_t red = redStr.toInt();
    uint8_t green = greenStr.toInt();
    uint8_t blue = blueStr.toInt();

    String msg = "Color received: ";
    msg += "red:";
    msg += redStr;
    msg += ", green:";
    msg += greenStr;
    msg += ", blue:";
    msg += blueStr;

    DBG_OUTPUT_PORT.println(msg);

    inputParams.color = NeoPixels::Color(red, green, blue);

    server.send(200, "text/plain", "Setting Color");
}

void initWebService(void) {    

  DBG_OUTPUT_PORT.begin(9600);
  DBG_OUTPUT_PORT.setDebugOutput(true);
  DBG_OUTPUT_PORT.println("[setup] entered");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  DBG_OUTPUT_PORT.print("Connecting to ");
  DBG_OUTPUT_PORT.println(ssid);
    
  // Wait for connection
  uint8_t i = 0;
  while ((WiFi.status() != WL_CONNECTED) && (i++ < 20)) { //wait 10 seconds
    delay(500);
  }
  if (i == 21) {
    DBG_OUTPUT_PORT.print("Could not connect to ");
    DBG_OUTPUT_PORT.println(ssid);
  
    // try making access point
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ap, ap_password);
    IPAddress myIP = WiFi.softAPIP();
    DBG_OUTPUT_PORT.print("IP address: ");
    DBG_OUTPUT_PORT.println(myIP);
  } else {
    DBG_OUTPUT_PORT.print("Connected! IP address: ");
    DBG_OUTPUT_PORT.println(WiFi.localIP());
  }
    
  server.on("/list", HTTP_GET, printDirectory);
  server.on("/color", HTTP_GET, updateColor);
  server.onNotFound(handleNotFound);

  server.begin();
  DBG_OUTPUT_PORT.println("HTTP server started");
}