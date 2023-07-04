
//Wait for the modules to be available then read and store the values in arr
bool read_8hx(uint32_t *arr, int channels[], int n, int sclk) {
  //wait for the 8 modules to be ready (dout=0 for each module)
  int rdy = 0;
  for (int i = 0; i < n; i++)
    rdy += digitalRead(channels[i]);
  if (rdy != 0)
    return false;

  int32_t val[8];

  for (uint8_t i = 0; i < n; i++) {

    val[i] = 0;
    arr[i] = 0;
  }
  uint32_t v = 0;

  //read 24 bits for each modules
  long dt = micros();
  for (uint8_t i = 0; i < 24; i++) {
    digitalWrite(sclk, HIGH);
    digitalWrite(sclk, LOW);
    for (int j = 0; j < n; j++) {
        arr[j] |= (int32_t)(digitalRead(channels[j])) << (23 - i);
    }
  }
  // Send clock cycles to select the gain of following reading 1=128
  for (uint8_t i = 0; i < 1; i++) {
    digitalWrite(sclk, HIGH);
    delay(1);
    digitalWrite(sclk, LOW);
  }
  //v=val[n-1];
  //arr[2]=val[2];
  //val[2]=8388607;
  //Serial.print(micros() - dt);
  // for (int j = 0; j < n; j++) {

  //   Serial.println(arr[j], HEX);
  // }

  // for (uint8_t i = 0; i < 24; i++) {
  //   Serial.println(i);
  //   Serial.println(" ");
  //   for (int j = 0; j < n; j++) {
  //     Serial.print(j);
  //     Serial.print(" ");
  //     Serial.println(values[j],HEX);
  //   }
  // }




  for (uint8_t i = 0; i < n; i++) {
    uint8_t d[3] = { (arr[i] & 0xff), ((arr[i] >> 8) & 0xff), ((arr[i] >> 16) & 0xff) };

    uint8_t filler = 0x00;
    if (d[2] & 0x80)
      filler = 0xff;

    arr[i] = (static_cast<unsigned long>(filler) << 24
              | static_cast<unsigned long>(d[2]) << 16
              | static_cast<unsigned long>(d[1]) << 8
              | static_cast<unsigned long>(d[0]));
  }
  return true;
}