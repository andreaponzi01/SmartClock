#include <Adafruit_NeoPixel.h>
#include <DS3231_Simple.h>

DS3231_Simple Clock;
DateTime MyDateAndTime;

#define LEDCLOCK_PIN    9
#define LEDDOWNLIGHT_PIN    0
#define LEDCLOCK_COUNT 207
#define LEDDOWNLIGHT_COUNT 0

uint32_t clockMinuteColour = 0xDAA520; // Colore ocra per i minuti
uint32_t clockHourColour = 0xDAA520;   // Colore ocra per le ore
int clockFaceBrightness = 0;

Adafruit_NeoPixel stripClock(LEDCLOCK_COUNT, LEDCLOCK_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripDownlighter(LEDDOWNLIGHT_COUNT, LEDDOWNLIGHT_PIN, NEO_GRB + NEO_KHZ800);

const int trigPin = 7; // Pin del Trigger dell'HC-SR04
const int echoPin = 6; // Pin dell'Echo dell'HC-SR04
unsigned long lastMotionDetected = 0; // Tempo dell'ultimo movimento
const unsigned long motionTimeout = 300000; // 5 minuti in millisecondi

void setup() {
  Serial.begin(9600);
  Clock.begin();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  stripClock.begin();
  stripClock.show();
  stripClock.setBrightness(100);
  
  stripDownlighter.begin();
  stripDownlighter.show();
  stripDownlighter.setBrightness(50);
}

void loop() {
  // Controllo movimento
  if (detectMotion()) {
    lastMotionDetected = millis(); // aggiorna il tempo dell'ultimo movimento
    displayClock(true);            // accendi il display dell'orologio
  } else if (millis() - lastMotionDetected > motionTimeout) {
    displayClock(false);           // spegni il display dopo 5 minuti di inattività
  }
  delay(500); // Rallenta il loop per evitare rilevamenti eccessivi
}

// Funzione per rilevare movimento tramite l'HC-SR04
bool detectMotion() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1; // Calcola la distanza in cm

    // Stampa la distanza per debugging
  Serial.print("Distanza rilevata: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Rileva un movimento se la distanza è entro un certo range
  return distance > 0 && distance < 100; // Regola il range secondo la tua esigenza
}

// Funzione per accendere/spegnere il display
void displayClock(bool state) {
  if (state) {
    readTheTime();
    displayTheTime();
    stripClock.show();
  } else {
    stripClock.clear();
    stripClock.show();
  }
}

// Funzione per leggere l'ora
void readTheTime() {
  MyDateAndTime = Clock.read();
  Serial.print("Time is: "); Serial.print(MyDateAndTime.Hour);
  Serial.print(":"); Serial.print(MyDateAndTime.Minute);
  Serial.print(":"); Serial.println(MyDateAndTime.Second);
}

// Funzione per visualizzare l'ora
void displayTheTime() {
  stripClock.clear();
  int firstMinuteDigit = MyDateAndTime.Minute % 10;
  displayNumber(firstMinuteDigit, 0, clockMinuteColour);
  int secondMinuteDigit = floor(MyDateAndTime.Minute / 10);
  displayNumber(secondMinuteDigit, 63, clockMinuteColour);
  
  int firstHourDigit = MyDateAndTime.Hour;
  if (firstHourDigit > 12) firstHourDigit = firstHourDigit - 12;
  firstHourDigit = firstHourDigit % 10;
  displayNumber(firstHourDigit, 126, clockHourColour);
  
  int secondHourDigit = MyDateAndTime.Hour;
  if (secondHourDigit > 12) secondHourDigit = secondHourDigit - 12;
  if (secondHourDigit > 9) stripClock.fill(clockHourColour,189, 18);
}

// Funzione per visualizzare un numero
void displayNumber(int digitToDisplay, int offsetBy, uint32_t colourToUse) {
  switch (digitToDisplay) {
    case 0:
      digitZero(offsetBy, colourToUse);
      break;
    case 1:
      digitOne(offsetBy, colourToUse);
      break;
    case 2:
      digitTwo(offsetBy, colourToUse);
      break;
    case 3:
      digitThree(offsetBy, colourToUse);
      break;
    case 4:
      digitFour(offsetBy, colourToUse);
      break;
    case 5:
      digitFive(offsetBy, colourToUse);
      break;
    case 6:
      digitSix(offsetBy, colourToUse);
      break;
    case 7:
      digitSeven(offsetBy, colourToUse);
      break;
    case 8:
      digitEight(offsetBy, colourToUse);
      break;
    case 9:
      digitNine(offsetBy, colourToUse);
      break;
    default:
      break;
  }
}


