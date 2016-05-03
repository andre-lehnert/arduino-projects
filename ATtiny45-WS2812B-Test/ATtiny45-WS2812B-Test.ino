// NeoPixel Bibliothek hinzufügen für die Ansteuerung der WS2812B LED Streifen
#include <Adafruit_NeoPixel.h>

// Instanz initialisieren
// Parameter Angaben:
// 1. Anzahl für verwendeter RGB LEDs
// 2. Auf welchen Pin soll verwendet werden.
// 3. Auswahl zwischen RGB, GRB oder BRG und verwendeter Takt 400KHz oder 800KHz
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(4, 2, NEO_GRB + NEO_KHZ800);

// Helligkeit einstellen. Kann von 0 bis Maximal 255 festgelegt werden.
// Bei Maximaler Ausleuchtung ist zu prüfen, ob die Maximale Stromabnahme erreicht wird.
int brightness = 100;

// Helligkeit bei verwendung der WS2812b, wenn sie weis auf blitzen sollen.
// Bei 210 wird die Maximale Stromabnahme überschritten. 
// Darf nur kurz verwendet werden, ansonsten brennt der Spannungsregler durch.
int brightnessWhite = 210;

// Wird zum festlegen des Licht Einstellung verwendet.
// Damit die Einstellung funktioniert, 
// muss an der Fernsteuerung einmal hin und her Geschaltet oder gedreht werden,
// um die längste Pulslänge zu ermitteln. 
unsigned long maxPwmIn = 2;

boolean sensorValue = true;

void setup() {
  // Signal von der Fernsteuerung aufnehmen.
  //pinMode(3, INPUT);
  // Positionslichter schalten
  pinMode(0, OUTPUT);
  
  // RGB LEDs Stribes starten
  pixels.begin();
  pixels.show();
}

void loop() {
  

    // RGB LED Streifen ansteuern.
    // Rot und Grün
    allRGB_LED_Off();
    pixels.setPixelColor(0, pixels.Color(brightness, 0, 0)); 
    pixels.setPixelColor(0, pixels.Color(0, brightness, 0)); 
    pixels.show();
    //delay(10);
    allRGB_LED_Off();
    pixels.setPixelColor(0, pixels.Color(brightness, 0, 0)); 
    pixels.setPixelColor(0, pixels.Color(0, brightness, 0)); 
    pixels.show();
    //delay(10);

  
}

// Stellt alle Werte zurück.
void allRGB_LED_Off()
{
  for(int i = 0; i < 4; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
  }
}
