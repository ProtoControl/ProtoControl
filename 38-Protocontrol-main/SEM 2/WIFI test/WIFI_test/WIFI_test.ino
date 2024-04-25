#include <ArduinoHttpClient.h>
#include <WiFi.h>

const char* ssid = "ncsu";
const char* password = "";
const char* serverIP = "YourServerIPAddress";
const int serverPort = 80; // HTTP server port
const String endpoint = "/data"; // Endpoint to receive information from the server

WiFiClient wifiClient;
HttpClient httpClient = HttpClient(wifiClient, serverIP, serverPort);

void setup() {
    Serial.begin(115200);
    delay(100);

    Serial.println("Connecting to WiFi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Send HTTP GET request to server
    sendGetRequest();
}

void loop() {
    // Do other tasks if needed
}

void sendGetRequest() {
    httpClient.get(endpoint);
    int statusCode = httpClient.responseStatusCode();
    String response = httpClient.responseBody();

    Serial.println("Response status code:");
    Serial.println(statusCode);
    Serial.println("Response body:");
    Serial.println(response);
}
