//include all required libraries
#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "xxxxxxxx";
const char* password = "xxxxxxxx";

const char* TEXT_INPUT1 = "HTML_STR_INPUT1";// String type input
const char* TEXT_INPUT2 = "HTML_INT_INPUT2";// Integer type input
const char* TEXT_INPUT3 = "HTML_FLOAT_INPUT3"; //Float type input
// HTML web page to handle 3 input fields (HTML_STR_INPUT1, HTML_INT_INPUT2, HTML_FLOAT_INPUT3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP32 HTML Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    Enter string Input Here: <input type="text" name="HTML_STR_INPUT1">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    Enter Integer Input Here: <input type="text" name="HTML_INT_INPUT2">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    Enter Float Input Here: <input type="text" name="HTML_FLOAT_INPUT3">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(TEXT_INPUT1)) {
      inputMessage = request->getParam(TEXT_INPUT1)->value();
      inputParam = TEXT_INPUT1;
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(TEXT_INPUT2)) {
      inputMessage = request->getParam(TEXT_INPUT2)->value();
      inputParam = TEXT_INPUT2;
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    else if (request->hasParam(TEXT_INPUT3)) {
      inputMessage = request->getParam(TEXT_INPUT3)->value();
      inputParam = TEXT_INPUT3;
    }
    else {
      inputMessage = "No Input Text sent on ESP32";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // Code to define logic based on received message from HTML form
}
