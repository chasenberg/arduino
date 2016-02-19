#define RELAY_ON 0
#define RELAY_OFF 1
#define RELAY_1  2

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  //Teil der das Relais ansteuert
  // Set pin as output.
  pinMode(RELAY_1, OUTPUT);
 
  // Initialize relay one as off so that on reset it would be off by default
  digitalWrite(RELAY_1, RELAY_OFF);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
  Serial.println(voltage);
  //Teil der das Relais ansteuert
  // Turn on and wait 3 seconds.
  digitalWrite(RELAY_1, RELAY_ON);
  delay(100);
 
  // Turn off and wait 3 seconds.
  digitalWrite(RELAY_1, RELAY_OFF);
  delay(3000);
}

