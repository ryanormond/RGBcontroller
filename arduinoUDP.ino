//includes
//internet
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

// mac and ip
byte mac[] = { 0xAD, 0xBE, 0xBD, 0xEF, 0xDE, 0xEF}; //this can be any mac address
IPAddress ip(<local IP Here>);     // depends on local network
unsigned int localPort = 8888;      // local port to listen on
EthernetUDP Udp;  //ethernet instance
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,

//vars

byte ledPins[] = {5, 6, 9}; // red, green and blue pins

void setup() {
  // initialize the digital pin as an output.
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  //connection setup
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
  Serial.begin(9600);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if(packetSize == 4) // if there's data available, read a packet
  {
    Serial.println("packet received");
    // read the packet into packetBufffer
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    //set variables from the buffer array
    unsigned char cState = packetBuffer[0];
    unsigned char fState = packetBuffer[1];
    unsigned char vhState = packetBuffer[2];
    unsigned char vlState = packetBuffer[3];
    int changeDivisor = 1; //divisor for pwm function
    byte ledPin = 0; //default

    //set colour
    switch(cState) {
        case 'r': ledPin = ledPins[0]; break;
        case 'g': ledPin = ledPins[1]; break;
        case 'b': ledPin = ledPins[2]; break;
        default: return;
      }
    //set frequency
    switch(fState) {
        case '1': changeDivisor = 1; break;
        case '2': changeDivisor = 8; break;
        case '3': changeDivisor = 64; break;
        case '4': changeDivisor = 256; break;
        case '5': changeDivisor = 1024; break;
        default: return;
      }

    byte lightValue = (charToDec(vhState) << 4) | charToDec(vlState); //brightness
    setPwmFrequency(ledPin, changeDivisor); //set pulse width
    analogWrite(ledPin, lightValue); //write changes to the output
  }
}

/*
 * Converts the Hex value of the char from packet
 * into a decimal value  0 - 255.
 * This value is used to set the brightness of the led
 */
byte charToDec(unsigned char c)
{
  if(c >= '0' && c <= '9')
  {
    return c - '0';
  }
  else if (c >= 'A' && c <= 'F')
  {
    return c - 'A' + 10;
  }
  else if (c >= 'a' && c <= 'f')
  {
    return c - 'a' + 10;
  }
  return 0;
}

/**
 * Divides a given PWM pin frequency by a divisor.
 *
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 *
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
 *
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 *
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   http://forum.arduino.cc/index.php?topic=16612#msg121031
 */
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
