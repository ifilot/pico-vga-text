void setup() {
  // Initialize Serial (USB connection to computer)
  Serial.begin(115200);

  // Initialize Serial1 (Hardware serial for incoming data)
  Serial1.begin(115200); // Match the baud rate of your ACIA

  DDRB |= 0b11110000;  // PB4-PB7 (D8-D11) as outputs
  DDRF |= 0b11110000;  // PF4-PF7 (A3-A0) as outputs

  // Configure latch pin (PD1) as output
  pinMode(2, OUTPUT);  
  digitalWrite(2, LOW); // Ensure latch starts low
}

void send_char(char val) {
  // Send data on parallel pins (PB4-PB7 and PF4-PF7)
    PORTB = (PORTB & 0x0F) | ((val & 0x0F) << 4);  // PB4-PB7 (D8-D11)
    PORTF = (PORTF & 0x0F) | ((val & 0xF0));       // PF4-PF7 (A3-A0)

    // Pulse the latch signal (D2) to indicate data is ready
    digitalWrite(2, HIGH);
    delayMicroseconds(200);
    digitalWrite(2, LOW); // End latch pulse
}

void loop() {
  static uint8_t val = ' ';

  for(char val=' '; val <= '~'; val++) {
    send_char(val);
  }
  send_char('\n');

  delay(1000);

  // Forward data from Serial1 to Serial
  if (Serial1.available()) {
    char incomingByte = Serial1.read();
    Serial.write(incomingByte); // Send the byte to the computer
  }

  // Optionally, forward data from Serial to Serial1 (e.g., to send commands)
  if (Serial.available()) {
    char outgoingByte = Serial.read();
    Serial1.write(outgoingByte); // Send the byte to the ACIA or other device
  }
}
