#include <DHT.h> 
#include <Servo.h>
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <SoftwareSerial.h>
#include <PubSubClient.h>

// Constants
#define trigPin 4
#define echoPin 6
#define redLED 7
#define dir1 8
#define dir2 9
#define speedPin 10
#define servoPin 11
#define sensePin 12
#define greenLED 5
#define sprayCooldown 30000 
#define Type DHT11
#define MQTT_SERVER  "io.adafruit.com"
#define IO_USERNAME  "up895059"
#define IO_KEY       "SECRETsecretKEYkey"
#define MQTT_PORT 1883

// Variables
char ssid[]  = "Z-0322";
char pass[] = "TestTest";
int status = WL_IDLE_STATUS;
unsigned long startMillis;
unsigned long sprayMillis = 0;
float humidity; float tempC; 
int tempLimit=26; int humidityLowerLimit = 60;
String temp_str;
String hum_str;
char temp[50];
char hum[50];

// Setup for libraries
Servo myServo;
DHT HT(sensePin, Type);
WiFiEspClient WiFiClient;//WiFi
PubSubClient client(MQTT_SERVER, MQTT_PORT, WiFiClient); //MQTT Client
SoftwareSerial esp8266(2,3);

// Method to move the servo, causing a spray
void spray()
{
    myServo.write(0);
    delay(1000);
    myServo.write(180);
    delay(500);
    myServo.write(0);  
    delay(250);
}

// Method to get distance from ultrasonic sensor
int getDistance()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    return duration * 0.034 / 2;
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i=0;i<length;i++) {
        Serial.print((char)payload[i]);
    }
}

void printWifiStatus()
{
  Serial.print("WiFi Status: ");
  Serial.println(WiFi.status());
}

void reconnect()
{
    while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
        if (client.connect("arduinoClient",IO_USERNAME,IO_KEY)) {
            Serial.println("connected");
            // Publish statistics of device
            client.publish("up895059/feeds/iotcw.humidity",hum);
            client.publish("up895059/feeds/iotcw.temp",temp);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void setup()
{
    // Sets OUTPUTS and INPUTS
    pinMode(speedPin,OUTPUT);
    pinMode(dir1,OUTPUT);
    pinMode(dir2,OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(redLED, OUTPUT);
    pinMode(greenLED, OUTPUT);
    pinMode(echoPin, INPUT);
    //Direction of spin on the DC motor
    digitalWrite(dir1, HIGH);
    digitalWrite(dir2, LOW);

    // Setup for libraries
    Serial.begin(9600);
    esp8266.begin(9600);
    HT.begin();
    myServo.attach(servoPin);
    WiFi.init(&esp8266);

    startMillis = millis();

    // Ensures WiFi shield is present
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        while (true);
    }

    while ( status != WL_CONNECTED) {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);
    }

    Serial.println("You're connected to the network");
    printWifiStatus();
    client.setCallback(callback);
}

void loop()
{
    // Checks to see if there is a plant infront of device
    boolean plantPlaced = getDistance() > 20;
    if (plantPlaced)
    {
        // If plant isn't placed, then turn on red LED
        digitalWrite(redLED, HIGH);
        digitalWrite(greenLED, LOW);
        // Disables DC motor if running
        analogWrite(speedPin,0);
        delay(5000);
    }else
    {
        // If plant is placed then carry on with program, enable green light
        digitalWrite(greenLED, HIGH);
        digitalWrite(redLED, LOW);

        
        humidity = HT.readHumidity();
        tempC = HT.readTemperature();
        unsigned long currentMillis = millis();

        // If temp is over limit then run DC motor
        if (tempC > tempLimit)
        {
            analogWrite(speedPin,255);
        }else
        {
            analogWrite(speedPin,0);
        }

        // If Humidity is too low and spay cooldown has ended then spray
        if(humidity<humidityLowerLimit && currentMillis>=sprayMillis)
        {
            spray();
            sprayMillis = currentMillis + sprayCooldown;
            Serial.println(sprayMillis);
        }
        // Float to Char[]
        temp_str = String(tempC);
        temp_str.toCharArray(temp, temp_str.length() + 1);
        hum_str = String(humidity);
        hum_str.toCharArray(hum, hum_str.length() + 1);

        // MQTT connect to Adafruit
        if (!client.connected()){reconnect();}
        client.loop();
    }
    delay(500);
}


