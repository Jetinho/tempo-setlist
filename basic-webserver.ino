#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "feather32";
const char *password = "feather32";

AsyncWebServer server(80);
void setup()
{
  Serial.begin(115200);
  // Create access point
  Serial.println("\n[*] Creating AP");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("[+] AP Created with IP Gateway ");
  Serial.println(WiFi.softAPIP());

  // Define a route to serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
	   Serial.println("ESP32 Web Server: New request received:");  // for debugging
	   Serial.println("GET /");        // for debugging
	   request->send(200, "text/html", "<html><body><h1>Hello, ESP32!</h1></body></html>"); });
  // Start the server
  server.begin();
}
void loop() {}
