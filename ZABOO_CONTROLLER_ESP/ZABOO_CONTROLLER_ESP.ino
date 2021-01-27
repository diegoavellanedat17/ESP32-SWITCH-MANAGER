#include <FS.h>                   //Este sistema maneja los archivos en la flash 
#include "SPIFFS.h"
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <PubSubClient.h>
#include "Secret_config.h"

#include <WiFi.h>
#define L1 14
#define L2 27 
#define GL 18
#define BL 5

unsigned long check_wifi = 10000;

//MQTT 
const char* mqttServer = MQTT_SERVER ;
const int mqttPort = MQTT_PORT;
const char* mqttUser = MQTT_USER;
const char* mqttPassword =MQTT_PASSWORD;
const char* topico =TOPICO;
int mqttCon=0;
int trys=0;

// Cliente ESP en el Broker mqtt

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
 
 
//  for (int i = 0; i< length; i++) {
//    Serial.print((char)payload[i]);
//  }
//  Serial.println(" ");

    if(strncmp((char*)payload,"L1ON",length)==0){
     Serial.println("L1ON");
    }

   else  if(strncmp((char*)payload,"L1OFF",4)==0){
    Serial.println("L1OFF");
  }
  
  
  else if(strncmp((char*)payload,"STATE",length)==0){
      client.publish(topico,"ZabooAC0/alive");
  }

  else if(strncmp((char*)payload,"HOW",length)==0){

    if(digitalRead(L1)==HIGH){
      client.publish(topico,"encendido");
    }
    else if(digitalRead(L1)==LOW){
      client.publish(topico,"apagado");
    }
    
  }




}
// Reconexión al Broker MQTT
void reconnect(int trys) {
  // Loop until we're reconnected
  while (!client.connected()  && trys < 3 ) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ZabooAC01", mqttUser, mqttPassword )) {
      Serial.println("connected");
      // Subscribe
      client.subscribe(topico);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(3000);

    }
    trys ++;
    }
}


void setup() {
  Serial.begin(9600);
  Serial.println("IM ESP");
  pinMode(L1,INPUT);
  pinMode(L2,INPUT);
  pinMode(BL,OUTPUT);
  pinMode(GL,OUTPUT);

  digitalWrite(BL,LOW);
  digitalWrite(GL,LOW);

  //Wifi handle
  WiFi.mode(WIFI_STA); // Set this Mode
    digitalWrite(BL,HIGH);
  digitalWrite(GL,HIGH);

  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;
  bool res;
  wm.setDebugOutput(false);
  res = wm.autoConnect("Zaboo AC CONTROLLER","password");
 
  if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();


  } 
  else {
           
        client.setServer(mqttServer, mqttPort);
        client.setCallback(callback);
       
        while (!client.connected()&& mqttCon==0 ) {
          Serial.println("Connecting to MQTT");
       
          if (client.connect("ZabooAC01", mqttUser, mqttPassword )) {

       
          } else {
       
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
            mqttCon=1;
       
          }
        }
         
          client.subscribe(topico);
    
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status() == WL_CONNECTED && client.connected()){
          delay(100);
          digitalWrite(GL,HIGH);
          digitalWrite(BL,LOW);
           delay(500);
          digitalWrite(BL,LOW);
          digitalWrite(GL,LOW);
    }
      // put your main code here, to run repeatedly:
  if (WiFi.status() != WL_CONNECTED  && (millis() > check_wifi)){

    digitalWrite(GL,LOW);
    digitalWrite(BL,HIGH);

    if(WiFi.status() == WL_CONNECTED){
      Serial.println("We have internet again");
    }

    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.begin();
    
    check_wifi = millis() + 10000;
  }

  if (!client.connected()) {
    digitalWrite(GL,LOW);
    digitalWrite(BL,HIGH);
    reconnect(trys);
  }

  client.loop();

}
