/*
 * Potentiometer-Based Transistor Load Control
 * -------------------------------------------
 * Reads a potentiometer on A0 and sets the PWM duty cycle on D9, which
 * drives a low-side transistor switching a DC motor.
 *
 * Controller used in the reconstructed implementation: Arduino Uno.
 *
 * Wiring:
 *   Pot wiper -> A0 (outer pins to 5V / GND)
 *   D9 -> 1k -> TIP120 base, 10k base pull-down to GND
 *   Motor between motor V+ and TIP120 collector, emitter to GND
 *   1N4007 across the motor (cathode to V+)
 *   D13 -> 220R -> red LED -> GND
 *   Motor supply GND tied to Arduino GND
 *
 * NEVER connect the motor directly to an Arduino pin.
 */

const int POT_PIN    = A0;
const int MOTOR_PIN  = 9;    // Timer1 PWM, ~490 Hz
const int STATUS_LED = 13;

void setup() {
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  analogWrite(MOTOR_PIN, 0);       // motor off at boot

  Serial.begin(9600);
  Serial.println(F("adc,pwm,duty_percent"));
}

void loop() {
  int sensorValue = analogRead(POT_PIN);                 // 0..1023
  int outputValue = map(sensorValue, 0, 1023, 0, 255);   // 0..255

  analogWrite(MOTOR_PIN, outputValue);
  digitalWrite(STATUS_LED, outputValue > 0);

  // CSV so the input/output relationship can be plotted
  Serial.print(sensorValue);
  Serial.print(',');
  Serial.print(outputValue);
  Serial.print(',');
  Serial.println(outputValue * 100 / 255);

  delay(50);
}
