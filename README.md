# OPPGAVE 1, SPILLKONSOLL (Eksamen - PG5501)

> Prosjektmappe ligger vedlagt


## Komponenter
| Komponent | Antall |
| ----------- | ----------- |
| Arduino Uno | 1 |
| Breadboard | 1 |
| Adafruit ST7789 | 1 |
| Tactile Push Button | 4 |
| Analog Joystick | 1 |
| Buzzer | 1 |
| 220Ω Resistor | 1 |
| 10KΩ Resistor | 4 |
| Breadboard Jumper Wires | 22 |

### Adafruit 1.14" 240x135 Color TFT - ST7789
TFT-skjermen brukes som display for spillet, og har en størrelse på 1.14". Skjermen har høy ppi (pixel density), noe som gjør at skrift vises tydelig, selv om den kan være i det minste laget. Siden jeg ikke benyttet meg av SD-kort-slot'en til skjermen, brukte jeg kun 4 av SPI-koblingene; SCK, MOSI, TFTCS og DC. 

### Tactile Push Buttons
Konsollen består av 4 knapper, men ikke alle brukes i dette prosjektet. Jeg valgte å bruke fire knapper for å få spillkonsoll-"looken", og det er tenkt å benytte alle fire knapper ved utvikling av nye og mer komplekse spill til konsollen min. 

### Analog Joystick
Joysticken sender analoge signaler som brukes både i spilloppsett og i selve spillet. Komponenten sender i utgangspunktet både verdier for en X-akse og en Y-akse, men i dette prosjektet har jeg kun brukt verdier fra X-aksen.

## Funksjonalitet
### Lagring av High Score i EEPROM
High score blir lagret i Arduino UNO sin EEPROM, som er minne som blir lagret på boardet selv når det slås av. Dette forsikrer at High scoren ikke endres før den faktisk blir slått, for deretter å bli lagret i EEPROM igjen. 

### Splash Screen / Setup Screen
Under setUp kjøres en animasjon som presenterer spillnavnet, og gir spilleren en liten innsikt i hva spillet handler om. 

### Hjemskjerm
Hjemskjermen viser spilleren at man skal trykke på knappen markert i grønt for å starte spillet. Nederst på skjermen er det fire biler i alle mulige fargevalg, og high scoren vises i nedre venstre hjørne. 

### Velg farge på bil
På skjermen for å velge farge på bilen, kan spilleren bruke joysticken til å velge mellonm fire bilfarger, og trykke på knappen som er markert på skjermen for å bekrefte farge.

### Instruksjoner
Etter at spilleren har valgt ønsket bilfarge, er det en skjerm med instruksjoner på som vises i 6 sekunder med en teller som vises nederst på skjermen. 

### Spill
Når spillet starter, er det en for-loop som teller ned, og spiller av en lyd på buzzeren. Spillet går ut på å ikke kræsje i tømmerstokkene som ruller nedover veien. For å styre bilen benyttes joysticken for å bevege bilen på en X-akse. I tillegg kan bilen skyte laser for å fjerne en tømmerstokk fra veibanen dersom x-koordinatene til laseren samsvarer med et x-koordinat på tømmerstokken. Spilleren starter med 5 lasere, og får 1 laser for hvert 10. poeng som blir sanket. Når laseren blir aktivert, tegnes det en laser i tilfeldig farge fra lyktene på bilen og til toppen av skjermen mens det spilles av en "laserlyd" på buzzeren. Dersom et punkt på tømmerstokken ligger mellom laserens x-koordinat og laserens x-koordinat + laserens bredde, vil tømmerstokken forsvinne, og en ny dukker opp og beveger seg nedover skjermen. For hver passerende trestokk, får spilleren ett poeng. Poengscoren og antall lasere vises i øvre venstre hjørne. X-koordinaten til tømmerstokkene er generert tilfeldig, men slik at de alltid vil dukke opp innenfor den tegnede veien. Tømmerstokkene beveger seg nedover banen på grunn av y-koordinatet som blir mindre for hver runde i loopen, og av den grunn beveger seg nedover på skjermen.

For at en kræsj skal trigges, er det flere faktorer som spiller inn. Koordinatene til venstre hjørne av bilen må matche et x-koordinat til en tømmerstokk når tømmerstokken kommer ned til bilens øverste y-koordinat. Dette gjelder også bilens høyre hjørne, som er regnet ut fra bilens x-koordinat plusset med bilens bredde. 

### Game over
Når spilleren taper, vises det en Game Over skjerm som viser poengscoren oppnådd denne runden av spillet. Dersom scoren er høyere enn den som er lagret i EEPROM, byttes verdien i EEPROM ut med den nye high scoren, og det står at spilleren har fått en ny high score. Etter 5 sekunder er spilleren tilbake på hjemskjermen igjen.
Detalj: Bilen som vises på sluttskjermen er i samme farge som spilleren valgte å spille med. 

## Koblingsskjema
<img width="468" alt="image" src="https://user-images.githubusercontent.com/56083423/142681695-bf595826-e11e-42be-a03e-ffe65253cecb.png">



