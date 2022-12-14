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
            <div class="row" v-if="animationsView">
                <select class="form-select" id="animationSelectList">
                    <option v-for="animation in animationsView" :key="animation.name" :selected="animation.selected" :value="animation.index">{{animation.name}}</option>
                  </select>
            </div>

            <div class="row" v-if="status">
                <div class="col-sm "><div class="d-flex align-items-stretch">{{status.Animation}}</div></div>
                <div class="col-sm">Color: <input class="form-control" type="color" v-model="tmpColor" /></div>
                <div class="col-sm">bgColor: <input class="form-control" type="color" v-model="tmpBgColor" /></div>
                <button class="col-sm" @click="saveStatus">Save</button>
            </div>
            <div class="row" v-else>
                <div class="col col-sm-12">Loading...</div>
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
        let hex = c.toString(16);
        return hex.length == 1 ? "0" + hex : hex;
    }

    function rgbToHex(r, g, b) {
        return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
    }

    function hexToRgb(hex) {
        let result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
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
                status: null,
                animationsView: null,
                tmpColor: "#FF0000",
                tmpBgColor: "#00FF00"
            }
        },
        async mounted() {
            await this.fetchAnimations();
            await this.fetchStatus();
        },
        methods: {
            async fetchAnimations() {
                const response = await fetch('http://192.168.0.232/api/v1/animations');
                const animationsJson = await response.json();

                let tmpAnimations = [];
                for(let i = 0; i < animationsJson["Animations"].length; i++)
                {
                    let tmpAnimation = animationsJson["Animations"][i];
                    
                    tmpAnimations.push({
                        index: i,
                        selected: false,
                        name: tmpAnimation.name
                    });
                }

                this.animationsView = tmpAnimations;
            },
            async fetchStatus() {

                const response = await fetch('http://192.168.0.232/api/v1/status');
                const data = await response.json();
                this.status = data;

                this.tmpColor = rgbToHex(
                    this.status.Settings.Global.color.red,
                    this.status.Settings.Global.color.green,
                    this.status.Settings.Global.color.blue);

                this.tmpBgColor = rgbToHex(
                    this.status.Settings.Global.bgColor.red,
                    this.status.Settings.Global.bgColor.green,
                    this.status.Settings.Global.bgColor.blue
                );
            },
            async saveStatus() {
                this.status.Settings.Global.color = hexToRgb(this.tmpColor);
                this.status.Settings.Global.bgColor = hexToRgb(this.tmpBgColor);

                let selectedAnimationIndex = document.getElementById("animationSelectList").value;
                this.status.Animation = this.animationsView[selectedAnimationIndex].name;

                const response = await fetch(
                    'http://192.168.0.232/api/v1/status', {
                    method: 'POST',
                    body: JSON.stringify(this.status)
                }
                );

                this.fetchStatus();
            }
        },
        watch: {
            'status': {
                handler: function (newStatus) {
                    for(let animation of this.animationsView)
                    {
                        animation.selected = (animation.name === newStatus.Animation);
                    }
                }
            },
        },
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

#define BUFFER_SIZE 2048
char JSON_BUFFER[BUFFER_SIZE];

#define DBG_OUTPUT_PORT Serial
WebServer server(80);

static Animations* animations;

void updateWebService(void) {
  server.handleClient();
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

void colorToJson(uint32_t source, JsonDocument& doc){
  doc["red"] = (source & 0x00ff0000) >> 16;
  doc["green"] = (source & 0x0000ff00) >> 8;
  doc["blue"] = (source & 0x000000ff);
}

void getAnimations()
{
  StaticJsonDocument<BUFFER_SIZE> doc;
  animations->getJsonAnimations(doc);

  serializeJson(doc, JSON_BUFFER, BUFFER_SIZE);
  server.send(200, "text/json", JSON_BUFFER);
}

void getInputParameters(JsonObject& doc) {

  StaticJsonDocument<265> color;
  colorToJson(animations->getInputParams().color, color);
  doc["color"] = color;

  StaticJsonDocument<265> bgColor;
  colorToJson(animations->getInputParams().bgColor, bgColor);
  doc["bgColor"] = bgColor;
}

uint32_t jsonToColor(const JsonObject& doc)
{
  uint32_t ret = 0;
  ret += int(doc["red"]) << 16;
  ret += int(doc["green"]) << 8;
  ret += int(doc["blue"]);

  return ret;
}

void getAnimation() {
  String animationName = server.pathArg(0);

  StaticJsonDocument<BUFFER_SIZE> doc;

  JsonObject globalSettings = doc.createNestedObject("Global");
  JsonObject animationSettings = doc.createNestedObject("Animation");

  getInputParameters(globalSettings);

  serializeJson(doc, JSON_BUFFER, BUFFER_SIZE);
  server.send(200, "text/json", JSON_BUFFER);

  // if(animationName.equalsIgnoreCase("radiate"))
  // {
  //   getRadiateAnimationSettings(doc);
  //   serializeJson(doc, JSON_BUFFER, BUFFER_SIZE);
  //   server.send(200, "text/json", JSON_BUFFER);
  // }
  // else
  // {
  //   server.send(404, "text/plain", "Unknown animation: " + animationName);
  // }
}

void getStatus()
{
  StaticJsonDocument<1024> doc;
  doc["Animation"] = animations->getCurrentAnimation()->getDisplayName().c_str();
  JsonObject settingsObj = doc.createNestedObject("Settings");
  JsonObject globalSettingsObj = settingsObj.createNestedObject("Global");

  getInputParameters(globalSettingsObj);

  serializeJson(doc, JSON_BUFFER, BUFFER_SIZE);
  server.send(200, "text/json", JSON_BUFFER);
}

void setInputParams(const JsonObject& jsonObj)
{
  animations->getInputParams().color = jsonToColor(jsonObj["color"]);
  animations->getInputParams().bgColor = jsonToColor(jsonObj["bgColor"]);
}

void setAnimation(const String& name)
{
  animations->selectAnimation(name);
}

void postStatus()
{
  StaticJsonDocument<2048> doc;
  deserializeJson(doc, server.arg("plain"));
  setInputParams(doc["Settings"]["Global"]);
  setAnimation(doc["Animation"]);

  server.send(200);
}

void postAnimation() {
  String animationName = server.pathArg(0);

  if(animationName.equalsIgnoreCase("radiate"))
  {
    StaticJsonDocument<1024> doc;
    deserializeJson(doc, server.arg("plain"));
    setInputParams(doc["Settings"]["Global"]);
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
  server.on("/api/v1/status", HTTP_GET, getStatus);
  server.on("/api/v1/status", HTTP_POST, postStatus);
  server.on(UriRegex("^\\/api\\/v1\\/animations$"), HTTP_GET, getAnimations);
  server.on(UriRegex("^\\/api\\/v1\\/animations\\/([a-zA-Z]+)"), HTTP_GET, getAnimation);
  server.on(UriRegex("^\\/api\\/v1\\/animations\\/([a-zA-Z]+)"), HTTP_POST, postAnimation);
  server.onNotFound(handleNotFound);

  server.enableCORS(true);

  server.begin();
  DBG_OUTPUT_PORT.println("HTTP server started");
}