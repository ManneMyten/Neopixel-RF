/*
 * Fil: encoder_transmit
 * Namn: Manne Laurin
 * Datum: 2024-02-01
 * Beskrivning: Kod som läser av en rotary encoder och skickar ärdet via RF
 */

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

RH_ASK driver;

//----------------------------------

#define outputA 2 //rotary encodern är kopplad till pins 2 och 3
#define outputB 3

volatile int counter = 0; // Variabel för att räkna antal rotationer av en encoder
int aState; // Nuvarande tillstånd av outputA (en del av en roterande encoder)
int bState; // Nuvarande tillstånd av outputB (en del av en roterande encoder)
int aLastState; // Senaste tillståndet av outputA

//--------------------------------------

void setup() {
  pinMode(outputA, INPUT_PULLUP); // Sätter outputA som en input med inbyggd pull-up resistor
  pinMode(outputB, INPUT_PULLUP); // Sätter outputB som en input med inbyggd pull-up resistor

  attachInterrupt(digitalPinToInterrupt(outputA), encoderRead, RISING); // Kopplar interrupt till outputA för att läsa av encoder, därför behöver funktionen encoderRead inte kallas i void loop
  Serial.begin (9600);
  
  // Läser av det första tillståndet för outputA
  aLastState = digitalRead(outputA);

#ifdef RH_HAVE_SERIAL
  Serial.begin(9600);    // Endast för felsökning
#endif
  if (!driver.init()) // startar RF-driver
#ifdef RH_HAVE_SERIAL
  {
    Serial.println("init failed");
  }
#else
    ;
#endif
  else
#ifdef RH_HAVE_SERIAL
    Serial.println("init success");
#else
    ;
#endif
}

void loop() {
  transmitCounter(); //Skickar counter-värdet
}

/*  
 *  Skickar det registrerade counter-värdet från rotary encoder 
 */
void transmitCounter(){
    int counterTwo = counter; // Kopierar värdet av counter till en ny variabel
    Serial.println(counterTwo); // Skriver ut counterTwo-värdet på seriell monitor
    const char *msg = reinterpret_cast<const char*>(&counterTwo); // Konverterar counterTwo-värdet till en char-array
    Serial.println(msg); // Skriver ut arrayen på seriell monitor
    

    driver.send((uint8_t *)msg, strlen(msg)); // Skickar meddelandet över RF
    driver.waitPacketSent(); // Väntar tills meddelandet är sänt klart
    Serial.println("Message sent"); // Skriver ut att meddelandet har sänts på seriell monitor
    delay(200); // Väntar 200 millisekunder innan nästa sändning
}

/* 
 *  Läser av rotary encoder och uppdaterar en 'counter' variabel för användning till transmitCounter
 */
void encoderRead() {
  aState = digitalRead(outputA); // Läser av outputA
  bState = digitalRead(outputB); // Läser av outputB
  //Serial.println(aState);

  if (aState != aLastState) { // Kollar om outputA har förändrats sedan senaste läsningen
    if ((aState == bState)&&(counter >= 0)) { // Om outputA och outputB är i samma tillstånd så har encoder roterats åt vänster, men minska inte värdet till negativt
      counter--; // Minskar räknaren med ett
      //Serial.println("left");
    } else if ((aState != bState)&&(counter <= 24)) { // Annars har encoder roterats åt höger, men counter får inte vara mer är antal pixlar på ringen, i detta fall tjugofyra
      counter++; // Ökar räknaren med ett
      //Serial.println("right");
    }
  }

  Serial.println(counter); // Skriver ut räknarens värde på seriell monitor
  aLastState = aState; // Uppdaterar senaste tillståndet av outputA för nästa iteration
}
