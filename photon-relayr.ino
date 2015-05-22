#include "MQTT.h"

//define your mqtt credentials
#define DEVICE_ID "c36642b8-3327-4935-8f21-19a0196e7349" 
#define MQTT_USER "c36642b8-3327-4935-8f21-19a0196e7349" 
#define MQTT_PASSWORD "H_Ex02fQyBno"
#define MQTT_SERVER "mqtt.relayr.io"
#define MQTT_CLIENTID "photon-relayr" //can be anything else

const int led = D7;
unsigned long lastTime = 0;

void callback(char* topic, byte* payload, unsigned int length);

//create our mqtt client
MQTT client(MQTT_SERVER,1883,callback);

// define our callback method thats called on receiving data
void callback(char* topic, byte* payload, unsigned int length) {

    //blink for 1 second on receiving data
    blink(1000);
    //store the received payload
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    String message(p);
    Serial.println(message);

//change the photon's RGB LED according to payload received
    if (message.equals("{\"Color\":\"RED\"}"))
        RGB.color(255, 0, 0);
    else if (message.equals("{\"Color\":\"GREEN\"}"))
        RGB.color(0, 255, 0);
    else if (message.equals("{\"Color\":\"BLUE\"}"))
        RGB.color(0, 0, 255);
    else
    //if unexpected payload LED color to white
        RGB.color(255, 255, 255);
    delay(100);
}

void mqtt_connect(){
  Serial.println("Connecting to mqtt server");
  if(client.connect(MQTT_CLIENTID, MQTT_USER, MQTT_PASSWORD)) {
    Serial.println("Connection success");

    //subscribe to a topic, cmd in this case
    client.subscribe("/v1/"DEVICE_ID"/cmd");
  }
  else { 
    Serial.println("Connection failed, check your credentials or wifi");
  }
}

void setup() {
    RGB.control(true);
    Serial.begin(9600);
    Serial.println("Hello There, I'm your photon!");
    //setup our LED pin and connect to mqtt broker
    pinMode(led, OUTPUT);
    mqtt_connect();
}

void loop() {
    if (client.isConnected()) {
        client.loop();
        //publish every 2 seconds
        if (millis() - lastTime > 2000) {
        lastTime = millis();
        publish();
      }
    } else {
        //if connection lost, try to reconnect
      mqtt_connect();
    }
}

void publish() {
    //publish to the out topic
    client.publish("/v1/"DEVICE_ID"/data", "{\"meaning\":\"gas level\"}");
    Serial.println("Publishing");
    //100ms blink when publishing
    blink(100);
}

void blink(int time) {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(time);               // wait for a second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(time);
} 
