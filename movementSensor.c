#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "NOME-DA-REDE-WIFI";
const char* password =  "SENHA-DA-REDE-WIFI";
const char* mqttServer = "iot.eclipse.org";
const int mqttPort = 1883;
const char* mqttUser = "abcdefg";
const char* mqttPassword = "123456";

WiFiClient espClient;
PubSubClient client(espClient);
char mensagem[30];

// Set GPIOs for LED and PIR Motion Sensor
const int led = 26;
const int motionSensor = 27;

// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR detectsMovement() {
  Serial.println("MOTION DETECTED!!!");
  sendMessage("Motion Detected.");
  digitalWrite(led, HIGH);
}

void IRAM_ATTR stopMovement() {
  Serial.println("Motion stopped.");
  sendMessage("Motion Stopped.");
  digitalWrite(led, LOW);
}

float sendTemperature() {
  float t = dht.readTemperature();
  sendMessage(String(t)));
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
  client.setServer(mqttServer, mqttPort);
  while (!client.connected())
  {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword ))
    {
      Serial.println("Conectado ao broker!");
    }
    else
    {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void sendMessage(String msg) {
  //Envia a mensagem ao broker
  client.publish("ArduinoeCia", msg);
  Serial.println("Mensagem enviada com sucesso...");
}

void loop() {
  connectBroker();
  sendTemperature();
  
  if(digitalRead(motionSensor) == HIGH){
    detectsMovement();
  }
  else{
    stopMovement();
  }
}
