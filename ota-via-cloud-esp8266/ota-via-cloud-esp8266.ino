#include <ESP8266WiFi.h>         

#include <PubSubClient.h>                         
#include <ESP8266httpUpdate.h>              
                        

const char* ssid = "yourssid";                  
const char* password =  "yourpassword";

const char* mqttServer = "broker.hivemq.com";   // MQTT Credentials
const int mqttPort = 1883;                      // MQTT Credentials
const char* mqttUser = "";                      // MQTT Credentials
const char* mqttPassword = "";                  // MQTT Credentials

const char* deviceID = "esp8266-1";               

WiFiClient espClient;                             
PubSubClient client(espClient);   

const int ledPin =  2;
int ledState = LOW;       

unsigned long previousMillis = 0;        
const long interval = 2000;                      // Change this blink speed to verify the OTA firmware update


void setup() {   

  pinMode(ledPin, OUTPUT);
   
  Serial.begin(9600);
  WiFi.begin(ssid, password);                     
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
    }  
  
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    
    if (client.connect(deviceID, mqttUser, mqttPassword )) {        
      Serial.print("connected");  
      } 
    
    else  {
      Serial.print("failed with state ");
      Serial.print(client.state());
      
      delay(2000);
      }
    }
  }

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    char receivedChar = (char)payload[i];
    Serial.print(receivedChar);    

    if (receivedChar == '1')                     // OTA UPDATE!
      ESPhttpUpdate.update("http://yourchosenname.000webhostapp.com/esp8266.bin"); // This is generated once you register with https://www.000webhost.com/        
      // Serial.println("Received message number 1.");
    }
  
  Serial.println();
  Serial.println("-----------------------");
  }   

void loop() {
  
  client.loop();
  client.subscribe("ota");                       // You can change this topic anytime. 

   unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    digitalWrite(ledPin, ledState);
    }
    
  }

  
