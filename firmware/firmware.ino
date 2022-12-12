#include <WiFi.h>
#include <PubSubClient.h>
#include <TinyGPSPlus.h>
#include "DHT.h"
#include "time.h"         //Time for NTP
#define DHTPIN 13         //define pin number
#define DHTTYPE DHT22     //our sensor is DHT22 type
int runs = 0;
const unsigned long eventInterval = 310000;
const unsigned long eventInterval1 = 5000;
unsigned long previousTime = 10000;
unsigned long previousTime1 = 100;
const unsigned long eventInterval2 = 10000;
unsigned long previousTime2 = 1000;
//Light sensor
#define LIGHT_SENSOR_PIN 32 // ESP32 pin GIOP32 (ADC0)
#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP 1200    //seconds the sensor would sleep
float analogValue = analogRead(LIGHT_SENSOR_PIN);

//ESP32 MQTT user config
const char* ssid = "ITEK 2nd"; // Wifi SSID
const char* password = "Four_Sprints_F21v"; // Wifi Password
const char* pubTopic = "g2"; // publish/username/apiKeyIn

//MQTT config
const char* mqtt_server = "test.mosquitto.org";
unsigned int mqtt_port = 1883;

//NTP
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600; //GMT+1 for Denmark (3600sec)
const int   daylightOffset_sec = 3600; //add 1 hr.

// objects
WiFiClient askClient;
PubSubClient client(askClient);
TinyGPSPlus gps; // The TinyGPS++ object
DHT dht(DHTPIN, DHTTYPE); //create an instance of DHT sensor

const char *gpsStream =
  "$GPGGA,105427.701,5607.164,N,01009.524,E,1,12,1.0,0.0,M,0.0,M,,*61\r\n"
  "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n"
  "$GPRMC,105427.701,A,5607.164,N,01009.524,E,006.1,259.4,231122,000.0,W*79\r\n"
  "$GPGGA,105428.701,5607.164,N,01009.521,E,1,12,1.0,0.0,M,0.0,M,,*6B\r\n"
  "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n"
  "$GPRMC,105428.701,A,5607.164,N,01009.521,E,010.7,024.0,231122,000.0,W*7E\r\n"
  "$GPGGA,105429.701,5607.166,N,01009.522,E,1,12,1.0,0.0,M,0.0,M,,*6B\r\n"
  "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n"
  "$GPRMC,105429.701,A,5607.166,N,01009.522,E,010.8,099.9,231122,000.0,W*7E\r\n"
  "$GPGGA,105430.701,5607.165,N,01009.527,E,1,12,1.0,0.0,M,0.0,M,,*65\r\n"
  "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n"
  "$GPRMC,105430.701,A,5607.165,N,01009.527,E,017.6,208.8,231122,000.0,W*72\r\n"
  "$GPGGA,105431.701,5607.161,N,01009.525,E,1,12,1.0,0.0,M,0.0,M,,*62\r\n"
  "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n"
  "$GPRMC,105431.701,A,5607.161,N,01009.525,E,010.4,347.4,231122,000.0,W*76\r\n"
  "$GPGGA,105432.701,5607.164,N,01009.524,E,1,12,1.0,0.0,M,0.0,M,,*65\r\n"
  "$GPGSA,A,3,01,02,03,04,05,06,07,08,09,10,11,12,1.0,1.0,1.0*30\r\n"
  "$GPRMC,105432.701,A,5607.164,N,01009.524,E,010.4,347.4,231122,000.0,W*71\r\n";

// setup
void setup() {

  Serial.begin(115200);
  Serial.println("*****************************************************");
  Serial.print("********** connecting to WIFI : ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

  }
  Serial.println("");
  Serial.println("->WiFi connected");
  Serial.println("->IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);


  //DHT22 sensor
  Serial.println("DHT22 sensor initiated");
  //call begin to start sensor
  dht.begin();

  //Initiate time (config)
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

//MQTT callback
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

int lightSensor() {
  int analogValue = analogRead(LIGHT_SENSOR_PIN);
  return analogValue;
}

void displayInfoLight() {
  char mqtt_payload[120] = "Low light detected. Waiting...";
  float Light = lightSensor();
  snprintf (mqtt_payload, 20, "%lf", Light);
  Serial.println(mqtt_payload);
  client.publish("g2/light", "49");
  
}


void light_sleep() {
  //Light Sensor
  float Light = lightSensor();
  // Light sensor reads the input on analog pin (value between 0 and 4095)
  // We'll have a few threshholds, qualitatively determined
  if (Light < 50) {
    Serial.println(" => Dark");
    displayInfoLight();
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    Serial.println("ESP32 set to sleep for " + String(TIME_TO_SLEEP) +
                   " Seconds");

    Serial.flush();
    esp_deep_sleep_start();

  }
}


// GPS displayInfo
void displayInfo() {

  //Light Sensor
  float Light = lightSensor();

  if (gps.location.isValid()) {
    double latitude = (gps.location.lat());
    double longitude = (gps.location.lng());


    //DHT22 sensor loop
    //use the functions which are supplied by library.
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Check if any reads failed and exit early (to try again).

    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    //time object
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }

    //time conv. to string
    char timeStringBuff[50]; //50 chars should be enough
    strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M:%S", &timeinfo);

    Serial.println("****** Publishing MQTT data to WebServer ******");
    char mqtt_payload[120] = "Low light detected. Waiting...";

    // snprintf (mqtt_payload, 90, "Hum:%lf; Temp:%lf; Lat:%lf; Long:%lf; Light:%lf;Time:%lf", h, t, latitude, longitude, Light, timeStringBuff);
    Serial.print("Publish message: ");
    // Serial.println(mqtt_payload);
    client.publish(pubTopic, mqtt_payload);

    snprintf (mqtt_payload, 20, "%lf", t);
    Serial.println(mqtt_payload);
    client.publish("g2/temp", mqtt_payload);

    snprintf (mqtt_payload, 20, "%lf", h);
    Serial.println(mqtt_payload);
    client.publish("g2/humid", mqtt_payload);

    snprintf (mqtt_payload, 20, "%lf", Light);
    Serial.println(mqtt_payload);
    client.publish("g2/light", mqtt_payload);

    snprintf (mqtt_payload, 50, "%lf, %lf", latitude, longitude);
    Serial.println(mqtt_payload);
    client.publish("g2/gps", mqtt_payload);

    snprintf (mqtt_payload, 50, timeStringBuff);
    Serial.println(mqtt_payload);
    client.publish("g2/time", mqtt_payload);

    Serial.println("> MQTT data published");
    Serial.println("********** End **********");


  }
}

//MQTT reconnect
void reconnect() {
  unsigned long currentTime1 = millis();
  // Loop until we're reconnected
  if (currentTime1 - previousTime1 >= eventInterval1) {
    while (!client.connected()) {
      Serial.print("********** Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect("ESP32Client")) {
        Serial.println("-> MQTT client connected");
      } else {

        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println("-> try again in 5 seconds");
        previousTime1 = currentTime1; // Wait 5 seconds before retrying
      }
    }
  }
}


// loop
void loop() {


  if (!client.connected()) {
    reconnect();
    client.loop();
  }
  light_sleep();
  unsigned long currentTime2 = millis();
  if (currentTime2 - previousTime2 >= eventInterval2) {
    if (runs < 2) {
      Serial.print("Printing test: ");
      while (*gpsStream)
        (gps.encode(*gpsStream++));
      Serial.println(runs);
      displayInfo();
      runs++;
    }

    previousTime2 = currentTime2;
  }
  unsigned long currentTime = millis();
  if (currentTime - previousTime >= eventInterval) {
    while (*gpsStream)
      (gps.encode(*gpsStream++));
    displayInfo();
    previousTime = currentTime;

  }
}
