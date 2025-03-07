void setup() {
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
    delayMicroseconds(delay);
    digitalWrite(2, LOW); // End latch pulse
}

void loop() {
  static uint8_t val = ' ';

  for(char val=' '; val <= '~'; val++) {
    send_char(val, 120);
  }
  send_char('\n', 200);

  for(char val=' '; val <= '~'; val++) {
    send_char(val, 200);
  }
  send_char('\n', 200);

  delay(100);
}
