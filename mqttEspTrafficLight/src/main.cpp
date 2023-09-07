#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 

// put function declarations here:

#define INTERVAL_MESSAGE1 5000
#define INTERVAL_MESSAGE2 8000
#define INTERVAL_MESSAGE3 13000
#define INTERVAL_MESSAGE4 15000

unsigned long time_1 = 0;
unsigned long time_2 = 0;
unsigned long time_3 = 0;
unsigned long time_4 = 0;

const char* ssid = "iPhone";
const char* password = "ing870915";
const char* mqtt_server = "172.20.10.6"; 


WiFiClient espClient;
PubSubClient client(espClient);

 long now = 0;

long lastMsg = 0;

char msg[50];
int value = 0;

int trafficLightStatus = 0; // Semaforo inicia en Rojo (0)

int delaytrafficLightStop = 5000;
int delaytrafficLightWarning = 3000;
int delaytrafficLightGo = 4000;
boolean maintenanceMode = false; 

const char* statusStop="stop";
const char* statusWarning="warning";
const char* statusGo="go";





// const char* statusMaintenanceMode="on";



void setup_wifi() {
 
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void callback(char* topic, byte* payload, unsigned int length) {
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  // for (int i = 0; i < length; i++) {
  //   Serial.print((char)payload[i]);
  // }
  
  if((char)payload[0]== '0'){
    client.publish("maintenanceModeStatus", "off");
    maintenanceMode = false;
  }
  if((char)payload[0]== '1'){
    maintenanceMode = true;
    client.publish("maintenanceModeStatus", "on");
  }
 
  // Switch on the LED if an 1 was received as first character
  // if ((char)payload[0] == '1') {
  //   digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  //   // but actually the LED is on; this is because
  //   // it is acive low on the ESP-01)
  // } else {
  //   digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  // }
 
}
 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");  
      client.subscribe("maintenanceMode");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
 
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  
  // Traffic Light PinModes
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void print_time(unsigned long time_millis){
    Serial.print("Time: ");
    Serial.print(time_millis/1000);
    Serial.print("s - ");
}
 
void loop() {

  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
 

  if(maintenanceMode == true){
    // client.publish("maintenanceModeStatus", "on");
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D0, LOW);
    delay(500);
    digitalWrite(D0, HIGH);
    delay(500);
    // Poner en modo mantenimiento 
  }else{
    // INICIAR RUTINA DE EJECUCIÓN NORMAL. 

    // if(millis() <= time_1 + INTERVAL_MESSAGE1){
    //     time_1 +=INTERVAL_MESSAGE1;
    //     digitalWrite(D0, HIGH);
    //     digitalWrite(D1, LOW);
    //     digitalWrite(D2, LOW);
    //     print_time(time_1);
        
    // }
   
    // if(millis() <= time_2 + INTERVAL_MESSAGE2){
    //     time_2 +=INTERVAL_MESSAGE2;
    //     digitalWrite(D0, LOW);
    //     digitalWrite(D1, HIGH);
    //     digitalWrite(D2, LOW);
    //     print_time(time_2);
    //     Serial.println("Hello, I'm the second message.");
    // }
   
    // if(millis() <= time_3 + INTERVAL_MESSAGE3){
    //     time_3 +=INTERVAL_MESSAGE3;
    //     digitalWrite(D1, LOW);
    //     digitalWrite(D1, LOW);
    //     digitalWrite(D2, HIGH);
    //     print_time(time_3);
    //     Serial.println("My name is Message the third.");
    // }
   
    // if(millis() >= time_4 + INTERVAL_MESSAGE4){
    //     time_4 += INTERVAL_MESSAGE4;
    //     print_time(time_4);
    //     Serial.println("Message four is in the house!");
    // }
    
    
    digitalWrite(D0, HIGH);
    client.publish("trafficLightStatus",statusStop);
    delay(delaytrafficLightStop);    
    digitalWrite(D0, LOW);
    digitalWrite(D1, HIGH);
    client.publish("trafficLightStatus",statusWarning);
    delay(delaytrafficLightWarning);
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);
    client.publish("trafficLightStatus",statusGo);
    delay(delaytrafficLightGo);
    digitalWrite(D2, LOW);
  }
  
  // if (now - lastMsg > 2000) {
  //   lastMsg = now;
  //   ++value;
  //   snprintf (msg, 75, "hello world #%ld", value);
  //   Serial.print("Publish message: ");
  //   Serial.println(msg);
  //   client.publish("outTopic", msg);
  // }
}
