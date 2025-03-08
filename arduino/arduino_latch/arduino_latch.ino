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

void send_char(char val, unsigned int delay) {
  // Send data on parallel pins (PB4-PB7 and PF4-PF7)
    PORTB = (PORTB & 0x0F) | ((val & 0x0F) << 4);  // PB4-PB7 (D8-D11)
    PORTF = (PORTF & 0x0F) | ((val & 0xF0));       // PF4-PF7 (A3-A0)

    // Pulse the latch signal (D2) to indicate data is ready
    digitalWrite(2, HIGH);
    digitalWrite(2, LOW); // End latch pulse
    delayMicroseconds(delay);
}

void handle_serial() {
  // Forward data from Serial1 to Serial
  while (Serial1.available()) {
    char incomingByte = Serial1.read();
    Serial.write(incomingByte); // Send the byte to the computer
  }

  // Optionally, forward data from Serial to Serial1 (e.g., to send commands)
  while (Serial.available()) {
    char outgoingByte = Serial.read();
    Serial1.write(outgoingByte); // Send the byte to the ACIA or other device
  }
}

void loop() {
  static uint8_t val = ' ';
  const uint8_t chardelay = 150;
  const uint8_t longdelay = 2000;

  handle_serial();

  send_char(0xFF, longdelay); // send clear screen
  send_char(0xA0, longdelay); // send beep
  send_char(0x90, chardelay); // send background black
  for(uint8_t i=0x81; i<=0x8F; i++) {
    send_char(i, chardelay);  // send color control character
    for(char val=' '; val <= '~'; val++) {
      send_char(val, chardelay);
    }
  }

  handle_serial();

  send_char(0x80, chardelay); // send foreground black
  for(uint8_t i=0x91; i<=0x9F; i++) {
    send_char(i, chardelay);  // send color control character
    for(char val=' '; val <= '~'; val++) {
      send_char(val, chardelay);
    }
  }

  handle_serial();

  _delay_ms(3000);
}
