/*
 * 
 * ESP32 PUBLISHER to topic test_topic
 * 
 */

#include <WiFi.h> // include WiFi library

#include <PubSubClient.h> // Include library to create a client MQTT

const char* ssid     = "<put-your-SSID-here>";            // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "<put-your-password-here>";     // The password of the Wi-Fi network

const char* mqttServer = "<put-your-mqttserverip-here>";           // The IP address of your MQTT Broker
const int mqttPort = 1883;                         // The TCP port of your MQTT Broker
const char* mqttUser = "<put-your-mqttuser-here>";               // MQTT client user
const char* mqttPassword = "<put-your-mqttpassword-here>";  // MQTT client password

WiFiClient espClient_1;                             // Define the client that allows us to transmit by WiFi. Must be differet for each MQQT client that you want to connect to a MQTT Broker
PubSubClient client(espClient_1);                   // Define the MQTT client that will access the MQTT Broker.

char payload[512];                                  // It will be the message to RECEIVE

int t=0;                                              //Incremental value to test the topic

const int BUTTON_PIN = 0; // constant for button
// Define led constant
#define LED 2

void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  // Set pin mode
  pinMode(LED,OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  digitalWrite(LED,LOW);
  
  /*Connecting ESP32 to WiFi */
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid);
 
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(500);
    Serial.print('.');
  }
 
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP32 to the computer
  
  /*Connecting ESP32 to MQTT Broker, subscribing and receivin messages */
  client.setServer(mqttServer, mqttPort); //configure the PubSubClient with the adress and port

  while (!client.connected()) { // While mqtt client it's not connected stay at loop
    Serial.println("Trying to connect to MQTT");

    if (client.connect("espClient_1", mqttUser, mqttPassword )) { // connect to broker (return true if connect, false if not)
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop() {
  // Send the message only when press the button
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.print("The message of the Test Topic: ");
    Serial.print("message ");
    t++;
    Serial.println(t);
    
    sprintf(payload, "Message, %d", t);
    if (client.publish("<put-your-topic-here>", payload, true)) {
      Serial.println("Sent to broker");
      // Blink LED when send the message
      digitalWrite(LED,HIGH);
    }
    delay(1000);
    // after 1 sec stop the LED
    digitalWrite(LED,LOW);
  }
}
