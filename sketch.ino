// Automated Irrigation Controller

const int sensorPin = A0;     // Soil moisture sensor analog output
const int ledPin = 13;        // Built-in LED

int moistureValue;
int threshold = 600;          // Adjust this based on your sensor readings

void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // LED OFF initially
}

void loop() {

  // Read the soil moisture value
  moistureValue = analogRead(sensorPin);

  // Display the value on the Serial Monitor
  Serial.print("Soil Moisture Value: ");
  Serial.println(moistureValue);

  // Check if the soil is dry
  if (moistureValue > threshold) {
    Serial.println("Soil is Dry -> Pump ON");
    digitalWrite(ledPin, HIGH);   // Turn LED ON (simulate pump)
  }
  else {
    Serial.println("Soil is Wet -> Pump OFF");
    digitalWrite(ledPin, LOW);    // Turn LED OFF
  }

  delay(1000); // Wait for 1 second before reading again
}