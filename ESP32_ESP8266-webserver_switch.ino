//inclue all required libraries for ESP32 and ESP8266 board
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "Your_SSID";
const char* password = "Your_Password";

const int LED_OUTPUT = 2;

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
  <head>
    <title>ESP32-ESP8266 Momentary Webserver</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Times New Roman; text-align: center; margin:0px auto; padding-top: 30px;}
      /**************************
  DEFAULT BOOTSTRAP STYLES
**************************/
.btn {
  display: inline-block;
  padding: 6px 12px;
  margin-bottom: 0;
  font-size: 14px;
  font-weight: normal;
  line-height: 1.42857143;
  text-align: center;
  white-space: nowrap;
  vertical-align: middle;
  cursor: pointer;
  -webkit-user-select: none;
     -moz-user-select: none;
      -ms-user-select: none;
          user-select: none;
  background-image: none;
  border: 1px solid transparent;
  border-radius: 4px;
  padding: 10px 16px;
}
  
.btn-lg {
  font-size: 18px;
  line-height: 1.33;
  border-radius: 6px;
}

.btn-primary {
  color: #fff;
  background-color: #428bca;
  border-color: #357ebd;
}

.btn-primary:hover,
.btn-primary:focus,
.btn-primary:active,
.btn-primary.active,
.open .dropdown-toggle.btn-primary {
  color: #fff;
  background-color: #3276b1;
  border-color: #285e8e;
}

/***********************
  ROUND BUTTONS
************************/
.round {
  border-radius: 24px;
}

/***********************
  CUSTON BTN VALUES
************************/

.btn {
  padding: 14px 24px;
  border: 0 none;
  font-weight: 700;
  letter-spacing: 1px;
  text-transform: uppercase;
}
.btn:focus, .btn:active:focus, .btn.active:focus {
  outline: 0 none;
}

.btn-primary {
  background: #0099cc;
  color: #ffffff;
}
.btn-primary:hover, .btn-primary:focus, .btn-primary:active, .btn-primary.active, .open > .dropdown-toggle.btn-primary {
  background: #FF0000;
}
.btn-primary:active, .btn-primary.active {
  background: #008000;
  box-shadow: none;
}
    </style>
  </head>
  <body>
    <center><h1>ESP32-ESP8266 Momentary Webserver</h1></center>
    <button class="btn btn-primary btn-lg round" onmousedown="toggleCheckbox('ON');" ontouchstart="toggleCheckbox('ON');" onmouseup="toggleCheckbox('OFF');" ontouchend="toggleCheckbox('OFF');">Momentary Switch: Press to Turn ON</button>
   <script>
   function toggleCheckbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
   }
  </script>
  </body>
</html>)rawliteral";


AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(LED_OUTPUT, OUTPUT);
  digitalWrite(LED_OUTPUT, LOW);

  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });

  // Receive an HTTP GET request
  server.on("/ON", HTTP_GET, [] (AsyncWebServerRequest * request) {
    digitalWrite(LED_OUTPUT, HIGH);
    request->send(200, "text/plain", "OK");
  });

  // Receive an HTTP GET request
  server.on("/OFF", HTTP_GET, [] (AsyncWebServerRequest * request) {
    digitalWrite(LED_OUTPUT, LOW);
    request->send(200, "text/plain", "OK");
  });


  server.begin();
}

void loop() {

}
