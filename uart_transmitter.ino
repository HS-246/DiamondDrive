void setup() {
  Serial.begin(9600); // Default UART on TX (GPIO1) and RX (GPIO3)
}

void loop() {
  Serial.println("HELLO WORLD"); // Send data over UART
  delay(1000); // Send every second
}
