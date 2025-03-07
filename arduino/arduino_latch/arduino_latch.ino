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
  const uint8_t delay = 120;

  send_char(0xFF, 120); // send clear screen
  send_char(0x90, 120); // send background black
  for(uint8_t i=0x81; i<=0x8F; i++) {
    send_char(i, delay);  // send color control character
    for(char val=' '; val <= '~'; val++) {
      send_char(val, delay);
    }
  }

  send_char(0x80, 120); // send foreground black
  for(uint8_t i=0x91; i<=0x9F; i++) {
    send_char(i, delay);  // send color control character
    for(char val=' '; val <= '~'; val++) {
      send_char(val, delay);
    }
  }

  _delay_ms(5000);
}
