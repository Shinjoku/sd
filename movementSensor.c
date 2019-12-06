#include <WiFi.h>
#include <DHT.h>
#include <PubSubClient.h>

const char* ssid = "Andrade";
const char* password =  "30192228a";
const char* mqttServer = "192.168.43.118";
const int mqttPort = 1883;
const char* mqttUser = "teste";
const char* mqttPassword = "teste";

// Set GPIOs for LED and PIR Motion Sensor
const int led = 26;
const int motionSensor = 27;
const int temperatureSensor = 25;

DHT dht(temperatureSensor, DHT22);
WiFiClient espClient;
PubSubClient MQTT(espClient);
char mensagem[30];


// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR detectsMovement() {
  Serial.println("MOTION DETECTED!!!");
  sendLampMessage("Liga");
  digitalWrite(led, HIGH);
}

void IRAM_ATTR stopMovement() {
  Serial.println("Motion stopped.");
  sendLampMessage("Desliga");
  digitalWrite(led, LOW);
}

void sendLampMessage(String msg) {
  //Envia a mensagem ao broker
  char message[25];
  sprintf(message, "%s", msg);
  MQTT.publish("lampada", message);
  Serial.println("Mensagem enviada para 'lampada' com sucesso...");
}

void sendTemperatureMessage(float msg) {
  //Envia a mensagem ao broker
  char message[25];
  sprintf(message, "%.2f", msg);  
  MQTT.publish("temperatura", message);
  Serial.println("Mensagem enviada para 'temperatura' com sucesso...");
}

float sendTemperature() {
  float t = dht.readTemperature();
  sendTemperatureMessage(t);
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(motionSensor, INPUT_PULLUP);
  
  // Set LED to LOW
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Iniciando conexao com a rede WiFi..");  
  } 
  
  Serial.println("Conectado na rede WiFi!");
}

void connectBroker() {
  MQTT.setServer(mqttServer, mqttPort);
  while (!MQTT.connected())
  {
    Serial.println("Conectando ao broker MQTT...");
    if (MQTT.connect("clientid", mqttUser, mqttPassword ))
    {
      Serial.println("Conectado ao broker!");
    }
    else
    {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(MQTT.state());
      delay(2000);
    }
  }
}


void loop() {
  connectBroker();
  sendTemperature();
  bool lastIsPresent = false;
  
  if(digitalRead(motionSensor) == HIGH){
    if(!lastIsPresent){
      lastIsPresent = true;
      detectsMovement();  
    }
    
  }
  else {
    if(lastIsPresent){
      lastIsPresent = false;
      stopMovement();
    }
  }
}
