#include "WebService.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#include <uri/UriBraces.h>
#include <uri/UriRegex.h>

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
  server.send(200, "text/plain", "zens_2022-09-03");
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

uint32_t colorFromRequestArguments() {
  String redStr = server.arg("red");
  String greenStr = server.arg("green");
  String blueStr = server.arg("blue");

  return NeoPixels::Color(redStr.toInt(), greenStr.toInt(), blueStr.toInt());
}

void updateColor() {
  inputParams.color = colorFromRequestArguments();
  server.send(200, "text/plain", "Setting Color");
}

void updateBgColor() {
  inputParams.bgColor = colorFromRequestArguments();
  server.send(200, "text/plain", "Setting Color");
}

void postTest() {
  String colorName = server.pathArg(0);

  String bodyTxt = server.arg("plain");
  server.send(200, "text/plain", colorName + bodyTxt);
}

void colorToJson(uint32_t source, DynamicJsonDocument& doc){
  doc.clear();

  doc["red"] = (source & 0x00ff0000) >> 16;
  doc["green"] = (source & 0x0000ff00) >> 8;
  doc["blue"] = (source & 0x000000ff);
}

void getRadiateAnimationSettings(DynamicJsonDocument& doc) {

  doc.clear();

  doc["name"] = "radiate";

  DynamicJsonDocument color(256);
  colorToJson(inputParams.color, color);
  doc["color"] = color;

  DynamicJsonDocument bgColor(256);
  colorToJson(inputParams.bgColor, bgColor);
  doc["bgColor"] = bgColor;
}

uint32_t jsonToColor(const DynamicJsonDocument& doc)
{
  uint32_t ret = 0;
  ret += int(doc["red"]) << 16;
  ret += int(doc["green"]) << 8;
  ret += int(doc["blue"]);

  return ret;
}

void getAnimationSettings() {
  String animationName = server.pathArg(0);

  char buffer[1024];
  DynamicJsonDocument respBody(1024);
  if(animationName.equalsIgnoreCase("radiate"))
  {
    getRadiateAnimationSettings(respBody);
    serializeJson(respBody, buffer, 1024);
    server.send(200, "text/plain", buffer);
  }
  else
  {
    server.send(404, "text/plain", "Unknown animation: " + animationName);
  }
}

void setRadiateAnimationSettings(const String& body)
{
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, body);

  inputParams.color = jsonToColor(doc["color"]);
  inputParams.bgColor = jsonToColor(doc["bgColor"]);
}

void postAnimationSettings() {
  String animationName = server.pathArg(0);

  if(animationName.equalsIgnoreCase("radiate"))
  {
    setRadiateAnimationSettings(server.arg("plain"));
    server.send(200);
  }
  else
  {
    server.send(404, "text/plain", "Unknown animation: " + animationName);
  }
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
    
  server.on(UriRegex("^\\/api\\/v1\\/animation\\/([a-zA-Z]+)\\/settings"), HTTP_GET, getAnimationSettings);
  server.on(UriRegex("^\\/api\\/v1\\/animation\\/([a-zA-Z]+)\\/settings"), HTTP_POST, postAnimationSettings);
  server.onNotFound(handleNotFound);

  server.begin();
  DBG_OUTPUT_PORT.println("HTTP server started");
}