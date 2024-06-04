#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "config.h"

// DHT sensor settings
#define DHTPIN D4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// WiFi and MQTT clients
WiFiClientSecure net;
PubSubClient client(net);

void connectWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void connectAWS() {
  net.setCertificate(deviceCert);
  net.setPrivateKey(privateKey);
  net.setCACert(rootCA);

  client.setServer(awsEndpoint, 8883);
  
  while (!client.connected()) {
    Serial.print("Connecting to AWS IoT...");
    if (client.connect("NodeMCUClient")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void publishData(float temperature, float humidity) {
  StaticJsonDocument<200> doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  if (client.publish("your/topic", jsonBuffer)) {
    Serial.println("Publish message: ");
    Serial.println(jsonBuffer);
  } else {
    Serial.println("Publish failed");
  }
}

void setup() {
  Serial.begin(9600);  // Change baud rate to 9600
  dht.begin();

  connectWiFi();
  connectAWS();
}

void loop() {
  if (!client.connected()) {
    connectAWS();
  }
  client.loop();

  delay(2000);  // Wait a few seconds between measurements

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  publishData(temperature, humidity);
}
