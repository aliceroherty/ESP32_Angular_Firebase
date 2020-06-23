#include <FirebaseESP32.h>
#include <FirebaseJson.h>
#include <WiFi.h>
#include <DHT.h>

#define DHTPIN 15     
#define DHTTYPE DHT11
#define LED 2
#define FIREBASE_HOST "esp32-sensor-test-46e8c.firebaseio.com" 
#define FIREBASE_AUTH "YPuhcHGipPXti02Ybtae8o14zSJkP97cmPUTd4yY" 
#define WIFI_SSID "Alice and Ksandy" 
#define WIFI_PASSWORD "Yettiqueen" 

DHT dht(DHTPIN, DHTTYPE);
FirebaseData firebaseData;
FirebaseJson reading;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to network");

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setMaxRetry(firebaseData, 3);
  Firebase.reconnectWiFi(true);
  
  dht.begin();
}

void loop() {
  WiFi.reconnect();
  
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED, LOW);
  
    delay(3000);
    
    digitalWrite(LED, HIGH);
    
    int humidity = (int)dht.readHumidity();
    int temperature = (int)dht.readTemperature();
  
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%\t\t\t\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C\n");

    reading.set("temperature", temperature);
    reading.set("humidity", humidity);
  
    if (Firebase.updateNode(firebaseData, "/sensors/dht11", reading)) {
      Serial.println("Firebase Updated");
      /*Serial.println(firebaseData.dataPath());
      Serial.println(firebaseData.jsonString());*/
    } else {
      Serial.println(firebaseData.errorReason());
    }
  }
  
  delay(3000);
}
