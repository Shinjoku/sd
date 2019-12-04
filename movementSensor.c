// Set GPIOs for LED and PIR Motion Sensor
const int led = 26;
const int motionSensor = 27;

// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR detectsMovement() {
  Serial.println("MOTION DETECTED!!!");
  digitalWrite(led, HIGH);
}

void IRAM_ATTR stopMovement() {
  Serial.println("Motion stopped.");
  digitalWrite(led, LOW);
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  //attachInterrupt(digitalPinToInterrupt(motionSensor), stopMovement, FALLING);
  //attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);
  // Set LED to LOW
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}

void loop() {
  if(digitalRead(motionSensor) == HIGH){
    detectsMovement();
  }
  else{
    stopMovement();
  }
}
