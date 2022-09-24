#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// DEFINES
//#define STATION1
#define STATION2
//#define STATION3

#define DHTPIN 4
#define DHTTYPE DHT22

// SSID/Password combination for WiFi connection
const char* ssid = "ENTER SSID";
const char* password = "ENTER PASSWORD";

// MQTT Borker IP address and credentials
const char* mqtt_server = "ENTER ADDRESS";
const char* mqtt_user = "ENTER USERNAME";
const char* mqtt_pwd = "@ENTER PASSWORD";

#ifdef STATION1
const char* mqtt_topic_temp = "station1/temperature";
const char* mqtt_topic_humid = "station1/humidity";
const char* mqtt_topic_enable = "station1/enable";
#endif
#ifdef STATION2
const char* mqtt_topic_temp = "station2/temperature";
const char* mqtt_topic_humid = "station2/humidity";
const char* mqtt_topic_enable = "station2/enable";
#endif
#ifdef STATION3
const char* mqtt_topic_temp = "station3/temperature";
const char* mqtt_topic_humid = "station3/humidity";
const char* mqtt_topic_enable = "station3/enable";
#endif

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

float temperature = 0;
float humidity = 0;
float heatIndex = 0;
bool enable = false;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("=====================================");
  #ifdef STATION1
  Serial.println("========== ESP32 Station 1 ==========");
  #endif
  #ifdef STATION2
  Serial.println("========== ESP32 Station 2 ==========");
  #endif
  #ifdef STATION3
  Serial.println("========== ESP32 Station 3 ==========");
  #endif
  Serial.println("=====================================");

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqtt_callback);

  dht.begin();
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void mqtt_callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Process read MQTT topic ?
  if (String(topic) == String(mqtt_topic_enable)) {
    Serial.print("Changing output to ");
    if(messageTemp == "true"){
      Serial.println("true");
      enable = true;
    }
    else if(messageTemp == "false"){
      Serial.println("false");
      enable = false;
    }
    else {
      Serial.println("(nothing, not recognised value)");
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    //Attemp to connect
    #ifdef STATION1
    if (client.connect("ESP32Client-station1", mqtt_user, mqtt_pwd)) {
    #endif
    #ifdef STATION2
    if (client.connect("ESP32Client-station2", mqtt_user, mqtt_pwd)) {
    #endif
    #ifdef STATION3
    if (client.connect("ESP32Client-station3", mqtt_user, mqtt_pwd)) {
    #endif
      Serial.println("Connected");
      // Subscribe
      client.subscribe(mqtt_topic_enable);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (enable) {
    long now = millis();
    if ((lastMsg == 0) || (abs(now - lastMsg) > 120000)) {
      lastMsg = now;
  
      // Temperature in Celsius
      temperature = dht.readTemperature();
      humidity = dht.readHumidity();
  
      char tempString[8];
      char humidString[8];
      dtostrf(temperature, 1, 2, tempString);
      dtostrf(humidity, 1, 2, humidString);
      Serial.print("Temperature: ");
      Serial.println(tempString);
      Serial.print("Humidity: ");
      Serial.println(humidString);
      client.publish(mqtt_topic_temp, tempString, true);
      client.publish(mqtt_topic_humid, humidString, true);
    }
  } else {
    lastMsg = 0;
  }
}
