#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
static const int REQUEST_CNT = 9;
static const int RESPONSE_CNT = 9;
uint8_t getppm[REQUEST_CNT] = {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00,0x79};
#define WAIT_READ_TIMES 100
#define WAIT_READ_DELAY 10

float measurement;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
}
void loop() {
byte buf[RESPONSE_CNT-1];
byte cheksum;
writeCommand(getppm, buf);

  for (int i = 0; i < RESPONSE_CNT; i++)
  {
    buf[i] = 0x0;
  }

  writeCommand(getppm, buf);
  // parse
  cheksum = (buf[1]+buf[2]+buf[3]+buf[4]+buf[5]+buf[6]+buf[7]);
  cheksum = (~cheksum)+1;
  
  if (buf[0] == 0xff && buf[1] == 0x86 && buf[8] == cheksum)
  {
    measurement = (buf[2] * 256 + buf[3])*0.1;
  }
  else
  {
    measurement = -1;
  }

Serial.println(measurement);

delay(2000);

}

void writeCommand(uint8_t cmd[], uint8_t *response)
{
 mySerial.write(cmd, REQUEST_CNT);
 mySerial.flush();

  if (response != NULL)
  {
    int i = 0;
    while (mySerial.available() <= 0)
    {
      if (++i > WAIT_READ_TIMES)
      {
        Serial.println("can't get ZE15CO response.");
        return;
      }
      delay(WAIT_READ_DELAY);
    }
    mySerial.readBytes(response, RESPONSE_CNT);
  }
}
