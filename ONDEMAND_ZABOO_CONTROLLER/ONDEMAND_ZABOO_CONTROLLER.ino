// This Script launch the configuration portal whe a button is trigger
//In a diferent scenary it trys to connect to a previous SSID and PASWORD, if exists
//Diego Avellaneda Torres

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

// select which pin will trigger the configuration portal when set to LOW
#define TRIGGER_PIN 0

int timeout = 120; // seconds to run for
WiFiManager wm;
WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "", 40);

void setup() {
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
     Serial.begin(9600);
    Serial.println("\n Starting");
    pinMode(TRIGGER_PIN, INPUT_PULLUP);

}

void loop() {
    // is configuration portal requested?
  if ( digitalRead(TRIGGER_PIN) == LOW) {
  

    //reset settings - for testing
    //wifiManager.resetSettings();
    wm.addParameter(&custom_mqtt_server);
    //wm.setConfigPortalBlocking(false);
    wm.setSaveParamsCallback(saveParamsCallback);
    // set configportal timeout
    
    wm.setConfigPortalTimeout(timeout);

    if (!wm.startConfigPortal("OnDemandAP")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.restart();
      delay(5000);
    }

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

  }

}

void saveParamsCallback () {
  Serial.println("Get Params:");
  Serial.print(custom_mqtt_server.getID());
  Serial.print(" : ");
  Serial.println(custom_mqtt_server.getValue());
}
