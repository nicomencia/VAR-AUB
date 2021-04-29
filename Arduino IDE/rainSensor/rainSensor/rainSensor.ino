#define DIGITAL_PIN 6
#define ANALOG_PIN A2

uint16_t rainVal;
boolean isRaining = false;
String raining;

void setup() {
  Serial.begin(9600);
  pinMode(DIGITAL_PIN, INPUT);
}
void loop() {
  rainVal = analogRead(ANALOG_PIN);
  isRaining = digitalRead(DIGITAL_PIN);
  if (isRaining) {
    raining = "No";
  }
  else {
    raining = "Yes";
  }
  rainVal = map(rainVal, 0, 1023, 100, 0);
  Serial.print("Raining: ");
  Serial.println(raining);
  Serial.print("Moisture: ");
  Serial.print(rainVal);
  Serial.println("%\n");
  delay(1000);
}
