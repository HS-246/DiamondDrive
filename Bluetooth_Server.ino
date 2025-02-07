#include <BluetoothSerial.h>
#include <esp_bt.h>
#include <esp_bt_device.h>
#include <esp_gap_bt_api.h>
#include <esp_spp_api.h>
  // Correctly include the library
BluetoothSerial YantraBT;
char cmd;

void setup() {
  // Initialize BluetoothSerial with device name "Yantra"
  YantraBT.begin("Yantra");
  
  // Initialize pin 2 as output
  pinMode(2, OUTPUT);
}

void loop() {
  // If Bluetooth data is available, read it
  if (YantraBT.available()) {
    cmd = YantraBT.read();
  }
  
  // Check the received command and control the output pin accordingly
  if (cmd == '1') {  // Compare with character '1'
    digitalWrite(2, HIGH);  // Turn on LED or whatever is connected to pin 2
  }
  else if (cmd == '0') {  // Compare with character '0'
    digitalWrite(2, LOW);   // Turn off LED or device
  }
  
  delay(20);  // Small delay to prevent spamming
}
