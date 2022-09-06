#include "WebService.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#include <uri/UriBraces.h>
#include <uri/UriRegex.h>

#include "Animation/Animations.h"

const char* RADIATE_SETTINGS_PAGE = R"~(
<script type="importmap">
    {
      "imports": {
        "vue": "https://unpkg.com/vue@3/dist/vue.esm-browser.js"
      }
    }
</script>

<html lang="en">

<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <!-- Bootstrap CSS -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet"
        integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3" crossorigin="anonymous">

</head>

<body>
    <div id="app">

        <div class="container">
            <div class="row" v-if="radiateSettings">
                <div class="col-sm ">{{radiateSettings.name}}</div>
                <div class="col-sm">Color: <input class="form-control" type="color" v-model="tmpColor" /></div>
                <div class="col-sm">bgColor: <input class="form-control" type="color" v-model="tmpBgColor" /></div>
                <button class="col-sm" @click="saveSettings">Save</button>
            </div>
            <div class="row" v-else>
                <button class="col col-sm-12" @click="fetchTest">Load settings</button>
            </div>
        </div>
    </div>

    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js"
        integrity="sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p"
        crossorigin="anonymous"></script>
</body>

</html>

<script type="module">
    import { createApp } from 'vue'

    function componentToHex(c) {
        var hex = c.toString(16);
        return hex.length == 1 ? "0" + hex : hex;
    }

    function rgbToHex(r, g, b) {
        return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
    }

    function hexToRgb(hex) {
        var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
        return result ? {
            red: parseInt(result[1], 16),
            green: parseInt(result[2], 16),
            blue: parseInt(result[3], 16)
        } : null;
    }

    createApp({
        data() {
            return {
                message: 'Hello Vue!',
                radiateSettings: null,
                tmpColor: "#FF0000",
                tmpBgColor: "#00FF00"
            }
        },
        methods: {
            async fetchTest() {

                const response = await fetch('http://192.168.0.232/api/v1/animation/radiate/settings');
                const data = await response.json();
                this.radiateSettings = data;

                this.tmpColor = rgbToHex(
                    this.radiateSettings.color["red"],
                    this.radiateSettings.color["green"],
                    this.radiateSettings.color["blue"]);

                this.tmpBgColor = rgbToHex(
                    this.radiateSettings.bgColor["red"],
                    this.radiateSettings.bgColor["green"],
                    this.radiateSettings.bgColor["blue"]
                );
            },
            async saveSettings() {
                this.radiateSettings.color = hexToRgb(this.tmpColor);
                this.radiateSettings.bgColor = hexToRgb(this.tmpBgColor);

                const response = await fetch(
                    'http://192.168.0.232/api/v1/animation/radiate/settings', {
                    method: 'POST',
                    body: JSON.stringify(this.radiateSettings)
                }
                );

                this.fetchTest();
            }
        }
    }).mount('#app')
</script>
)~";

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

static Animations* animations;

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
  animations->getInputParams().color = colorFromRequestArguments();
  server.send(200, "text/plain", "Setting Color");
}

void updateBgColor() {
  animations->getInputParams().bgColor = colorFromRequestArguments();
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
  colorToJson(animations->getInputParams().color, color);
  doc["color"] = color;

  DynamicJsonDocument bgColor(256);
  colorToJson(animations->getInputParams().bgColor, bgColor);
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

  animations->getInputParams().color = jsonToColor(doc["color"]);
  animations->getInputParams().bgColor = jsonToColor(doc["bgColor"]);
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

void getIndex()
{
  server.send(200, "text/html", RADIATE_SETTINGS_PAGE);
}

void initWebService(Animations* _animations) {    

  animations = _animations;

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
  
  server.on("/", HTTP_GET, getIndex);
  server.on(UriRegex("^\\/api\\/v1\\/animation\\/([a-zA-Z]+)\\/settings"), HTTP_GET, getAnimationSettings);
  server.on(UriRegex("^\\/api\\/v1\\/animation\\/([a-zA-Z]+)\\/settings"), HTTP_POST, postAnimationSettings);
  server.onNotFound(handleNotFound);

  server.enableCORS(true);

  server.begin();
  DBG_OUTPUT_PORT.println("HTTP server started");
}