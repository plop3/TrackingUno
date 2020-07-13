/*
   Nombre de dents: 138
   Poulie 1:        12
   Poulie 2:        72 (40)
   Pas:             200
   µPas:            16
*/

// DELAI_MS        32
// DELAI_OFFSET    10

#define PIN_RA_EN          8 // Pin enable

#define PIN_RA_STEP 5
#define PIN_RA_DIRECTION 2

#define PIN_8X  A0
#define PIN_RA_PLUS   A1
#define PIN_RA_MINUS  A2
#define PIN_RA_ENABLE    A3
const unsigned int STEP_SIGNAL_DURATION = 1000;         // microseconds
const unsigned long STEP_INTERVAL_SIDEREAL = 32609;     // 58696 microseconds
const unsigned long STEP_INTERVAL_2X_SIDEREAL = 16304;  // 29348 microseconds
const unsigned long STEP_INTERVAL_8X_SIDEREAL = 4076;   // 7337 microseconds
unsigned long lastRightAscensionStepTime;
void setup()
{
  pinMode(PIN_RA_STEP, OUTPUT);
  pinMode(PIN_RA_DIRECTION, OUTPUT);
  pinMode(PIN_8X, INPUT_PULLUP);
  pinMode(PIN_RA_PLUS, INPUT_PULLUP);
  pinMode(PIN_RA_MINUS, INPUT_PULLUP);
  pinMode(PIN_RA_ENABLE, INPUT_PULLUP);
    digitalWrite(PIN_RA_STEP, LOW);
  digitalWrite(PIN_RA_DIRECTION, LOW);                  // clockwise as a default
  lastRightAscensionStepTime = 0;
  //Serial.begin(9600);
  pinMode(PIN_RA_EN, OUTPUT);
}

void loop()

{
  digitalWrite(PIN_RA_EN, !digitalRead(PIN_RA_ENABLE));
  setRightAscensionDirection();
  unsigned long now = micros();
  unsigned long delta = 0;
  if (now < lastRightAscensionStepTime)                  // overflow of micros() counter
    delta = (now + (4294967295 - lastRightAscensionStepTime));
  else
    delta = (now - lastRightAscensionStepTime);
  if (delta >= getRightAscensionStepInterval())
  {
    digitalWrite(PIN_RA_STEP, HIGH);
    lastRightAscensionStepTime = now;
  }
  now = micros();
  delta = 0;
  if (now < lastRightAscensionStepTime)                  // overflow of micros() counter
    delta = (now + (4294967295 - lastRightAscensionStepTime));
  else
    delta = (now - lastRightAscensionStepTime);
  if ((now - lastRightAscensionStepTime) >= STEP_SIGNAL_DURATION)
    digitalWrite(PIN_RA_STEP, LOW);
}
void setRightAscensionDirection()
{
  if (digitalRead(PIN_RA_MINUS) == HIGH)
  {
    digitalWrite(PIN_RA_DIRECTION, HIGH);                 // counterclockwise
  }
  else
  {
    digitalWrite(PIN_RA_DIRECTION, LOW);                // clockwise
  }
}
unsigned long getRightAscensionStepInterval()
{
  if ((digitalRead(PIN_8X) == LOW) && isRightAscensionButtonPressed())
    return STEP_INTERVAL_8X_SIDEREAL;
  if (isRightAscensionButtonPressed())
    return STEP_INTERVAL_2X_SIDEREAL;
  return STEP_INTERVAL_SIDEREAL;
}
boolean isRightAscensionButtonPressed()
{
  return (digitalRead(PIN_RA_MINUS) == LOW) || (digitalRead(PIN_RA_PLUS) == LOW);
}
