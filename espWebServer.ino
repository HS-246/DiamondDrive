#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

// WiFi Access Point credentials
const char* ssid = "Arya";
const char* password = "12345678";

// UART Pins (Modify as per your connection)
#define RXD2 16  // ESP32 RX
#define TXD2 17  // ESP32 TX
HardwareSerial mySerial(2);

WebServer server(80);
String latestData = "No data received";

// Function to read UART data
void readUART() {
    if (mySerial.available()) {
        latestData = mySerial.readStringUntil('\n');  // Read UART data
        Serial.println("Received: " + latestData);
    }
}

// Serve index.html from LittleFS
void handleRoot() {
    File file = LittleFS.open("/index.html", "r");
    if (file) {
        server.streamFile(file, "text/html");
        file.close();
    } else {
        server.send(404, "text/plain", "File Not Found");
    }
}

// Serve latest UART data
void handleData() {
    readUART();
    server.send(200, "text/plain", latestData);
}

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600, SERIAL_8N1, RXD2, TXD2);  // UART2 on ESP32

    WiFi.softAP(ssid, password);
    Serial.println("WiFi AP Started. Connect to 'ESP32_AP' and open 192.168.4.1");

    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    server.on("/", handleRoot);
    server.on("/data", handleData);

    server.begin();
}

void loop() {
    server.handleClient();
    readUART();
}
