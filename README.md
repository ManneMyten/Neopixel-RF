# RF-kontroll av NeoPixel-ring med två Arduinos

## Beskrivning
Det här projektet visar hur man styr en NeoPixel-ring med hjälp av en Arduino och en RF-sändare/mottagare. Systemet består av två Arduinos - en som styr NeoPixel-ringen och tar emot RF-signalerna (`pixel_and_receive.ino`), och en annan som kodar en roterande encoder och skickar dess värde via RF (`encoder_transmit.ino`). Med detta system kan du använda en roterande encoder för att ändra hur många pixlar som lyser på din ring, trådlöst

## Krav
- Två Arduino-kompatibla mikrokontroller (t.ex. Arduino Uno)
- RF-sändare och mottagare (t.ex. 433MHz)
- NeoPixel-ring
- Adafruit NeoPixel-bibliotek
- RadioHead-biblioteket för RF-kommunikation

## Installation
### För Arduino med koden "pixel_and_receive.ino":
1. Anslut NeoPixel-ringen till din Arduino enligt tillverkarens anvisningar.
2. Anslut RF-mottagaren till din Arduino enligt tillverkarens anvisningar.
3. Ladda ner och installera Adafruit NeoPixel-biblioteket och RadioHead-biblioteket i Arduino IDE.
4. Ladda upp programmet `pixel_and_receive.ino` till din Arduino.

### För Arduino med koden "encoder_transmit.ino":
1. Anslut roterande encoder till din Arduino enligt tillverkarens anvisningar.
2. Anslut RF-sändaren till din Arduino enligt tillverkarens anvisningar.
3. Ladda ner och installera RadioHead-biblioteket i Arduino IDE.
4. Ladda upp programmet `encoder_transmit.ino` till din Arduino.

## Användning
1. Anslut strömförsörjningen till båda dina Arduinos.
2. Använd roterande encodern på den första Arduino för att ändra pixlarna på NeoPixel-ringen.
3. Se NeoPixel-ringen reagera på dina kommandon i realtid.

## Viktigt
- Se till att RF-sändaren och mottagaren är korrekt anslutna och konfigurerade för att kommunicera med varandra.
- Testa och kalibrera systemet noggrant innan du använder det i ett verkligt scenario för att undvika oönskade problem.

## Licens
Det här projektet är licensierat under [MIT-licensen](LICENSE).
