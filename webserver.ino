// Load Wi-Fi library
#include <WiFi.h>
#include "LittleFS.h"
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h> // Include ArduinoJson library

// Replace with your network credentials
const char *ssid = "feather32";
const char *password = "feather32";

// Set web server port number to 80
AsyncWebServer server(80);

// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup()
{
  Serial.begin(115200);

  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  File file = LittleFS.open("/data/test_example.txt", "r");
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.println("File Content:");
  while (file.available())
  {
    Serial.write(file.read());
  }
  file.close();

  Serial.println("\n[*] Creating AP");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("[+] AP Created with IP Gateway ");
  Serial.println(WiFi.softAPIP());

  // Define a route to serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("ESP32 Web Server: New request received:"); // for debugging
    Serial.println("GET /");                                   // for debugging
    request->send(LittleFS, "/index.html", "text/html"); });

  // Define a route to serve the CSS file
  server.on("/css/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("ESP32 Web Server: New request received:"); // for debugging
    Serial.println("GET /style.css");                          // for debugging
    request->send(LittleFS, "/css/style.css", "text/css"); });

  // Define a route to serve the pico.amber.min.css file
  server.on("/css/pico.amber.min.css", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("ESP32 Web Server: New request received:"); // for debugging
    Serial.println("GET /css/pico.amber.min.css");             // for debugging
    request->send(LittleFS, "/css/pico.amber.min.css", "text/css"); });

  // Define a route to serve the setlist.json file using ArduinoJson
  server.on("/data/setlist.json", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("ESP32 Web Server: New request received:"); // for debugging
    Serial.println("GET data/setlist.json");                       // for debugging

    File file = LittleFS.open("/data/setlist.json", "r");
    if (!file) {
      request->send(404, "application/json", "{\"error\":\"File not found\"}");
      return;
    }

    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
      request->send(500, "application/json", "{\"error\":\"Failed to parse JSON\"}");
      return;
    }

    String json;
    serializeJson(doc, json);
    request->send(200, "application/json", json); });

  // Define a route to serve the JavaScript file
  server.on("/js/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("ESP32 Web Server: New request received:"); // for debugging
    Serial.println("GET /js/script.js");                       // for debugging
    request->send(LittleFS, "/js/script.js", "application/javascript"); });

  // Define a route to serve the test_example.txt file
  server.on("/data/test_example.txt", HTTP_GET, [](AsyncWebServerRequest *request)
            {
      Serial.println("ESP32 Web Server: New request received:"); // for debugging
      Serial.println("GET /data/test_example.txt");                   // for debugging
      request->send(LittleFS, "/data/test_example.txt", "text/plain"); });

  // Define a route to handle POST requests to /setlist
  server.on("/setlist", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
            {
    Serial.println("ESP32 Web Server: New POST request received:");
    Serial.println("POST /setlist");

    Serial.println("Received data:");
    for (size_t i = 0; i < len; i++) {
      Serial.print((char)data[i]);
    }
    Serial.println();

    String json;
    for (size_t i = 0; i < len; i++) {
      json += (char)data[i];
    }

    Serial.println("Received JSON:");
    Serial.println(json);

    // Send response
    request->send(200, "application/json", "{\"status\":\"success\"}"); });

  // Start the server
  server.begin();
}

void loop() {}
