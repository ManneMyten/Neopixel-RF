#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Krävs för 16 MHz Adafruit Trinket
#endif


#define PIN 9 // Vilken pinne på Arduino är ansluten till NeoPixel ringen?


#define NUMPIXELS 24 // Hur många NeoPixels är anslutna till Arduino?

// När NeoPixel-biblioteket ställs in, talar vi om för det hur många pixlar,
// och vilken pin att använda för att skicka signaler. 
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Tid (i millisekunder) att pausa mellan pixlar


// Här kommer kod för mottagaren:
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Inte faktiskt använd men nödvändig för att kompilera
#endif

RH_ASK driver;

int counter = 0; // Variabel för att styra antalet tända pixlar i ringen.

void setup() {
  // Dessa rader är specifika för att stödja Adafruit Trinket 5V 16 MHz.
  // För alla andra kort kan du ta bort den här delen (men ingen skada att lämna den):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // Slut på Trinket-specifik kod.

  pixels.begin(); // INITIERA NeoPixel-stripobjektet 
  pixels.clear();

  // För mottagaren
#ifdef RH_HAVE_SERIAL
  Serial.begin(9600);    // Endast för felsökning
#endif
  if (!driver.init())
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
  readReceiver(); //läser av mottagaren för inkommande signaler
}

/*
 * Kollar om det finns några inkommande meddelanden, samt kallar på updateCounter med den  data som tas emot.
 */
void readReceiver() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen)) // Om den kan motta signaler som är krypterade rätt så kallas updateCounter
  {
    updateCounter("Got:", buf, buflen);
  }
}

/* 
 *  Uppdaterar counter med den data som tas emot, samt skriver ut det i terminalen. Denna funktion tillåter också för mottagning av längre meddelanden som t.ex strängar.
 */
void updateCounter(const char* prompt, const uint8_t* buf, uint8_t len) {
  Serial.println(prompt);
  for (uint8_t i = 0; i < len; i++)
  {
    if (i % 16 == 15)
      Serial.println(buf[i]);
    else
    {
      Serial.print(buf[i]);
      Serial.print(' ');
    }
  }
  Serial.println("");
  counter = (buf[0]); //uppdaterar counter
  
  updatePixels(); // Uppdatera pixlarna
}

/* 
 *  Denna funktion använder counter-värdet för att tända det antalet pixlar på ringen.
 */
void updatePixels() {
  for (int i = 0; i < NUMPIXELS; i++) { // Varje pixels uppdateras en i taget.

    // pixels.Color() tar RGB-värden, från 0,0,0 upp till 255,255,255
    if (i < counter) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 75)); // Blå färg för pixlar som är mindre än räknaren
    } else {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Ingen färg för övriga pixlar
    }

    pixels.show();   // Skicka de uppdaterade färgerna till hårdvaran.
  }
}
