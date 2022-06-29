#include <DHT.h>
#include<string.h>
#include <AntaresESP32HTTP.h>

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define MQ2PIN 34

int mq2thres = 850; //adjust mq2 ppm here
int thermalThres = 30; //adjust thermal threshold here
char statusNow[100];
#define ACCESSKEY "yourACCESSKEY"
#define WIFISSID "yourWIFISSID"
#define PASSWORD "YOURWIFIPASS"

#define projectName "appName"
#define deviceName "DeviceName"

AntaresESP32HTTP antares(ACCESSKEY);

void setup() {
  pinMode(MQ2PIN, INPUT);
  dht.begin();
  Serial.begin(115200);
  antares.setDebug(true);
  antares.wifiConnection(WIFISSID, PASSWORD);
}

void loop() {
  float h = float(dht.readHumidity());//Humidity
  float c = float(dht.readTemperature());//Celcius
  int mq2read = analogRead(MQ2PIN);
  if (isnan(h) || isnan(c)) {
    Serial.println("your sensor maybe error haha");
    return;
  }
  // Serial.print("MQ2 Value: ");
  // Serial.println(mq2read);
  
  if (mq2read > mq2thres && c>thermalThres)
  {
    strcpy(statusNow,"DANGER");
  }
  else if(mq2read > mq2thres || c>thermalThres)
  {
    strcpy(statusNow,"WARNING");
  }else 
  {
    strcpy(statusNow,"SAFE");
  }

    antares.add("mq2", mq2read);
    antares.add("thermal", c);
    antares.add("humidity", h);
    antares.add("status", statusNow);
    
    // Send from buffer to Antares
    antares.send(projectName, deviceName);
    delay(2000);
}