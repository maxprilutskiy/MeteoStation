#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
int lightSensorPin = 0;

#define rxPin 2
#define txPin 3

SoftwareSerial btSerial(rxPin, txPin);

struct SensorData
{
  float Pressure;
  float Temperature;
  float Altitude;
  float Lightness;
  void DisplaySensorData()
{
    Serial.print("Light:       ");
    Serial.print(this->Lightness, 2);
    Serial.println("%");
    Serial.print("Altitude:    "); 
    Serial.print(this->Altitude); 
    Serial.println(" m");
    Serial.print("Temperature: ");
    Serial.print(this->Temperature);
    Serial.println(" C");
    /* Display atmospheric pressue in hPa */
    Serial.print("Pressure:    ");
    Serial.print(this->Pressure);
    Serial.println(" hPa");
    Serial.println("");
}

void DisplaySensorDataInTwoRows()
{
    Serial.print("Temp: "); Serial.print(this->Temperature); Serial.println(" C       ");
    Serial.print("Pr: "); Serial.print(this->Pressure); Serial.println(" Pa       ");
    delay(1000);    
    Serial.print("Alt: "); Serial.print(this->Altitude); Serial.println(" m       ");
    Serial.print("Light: "); Serial.print(this->Lightness); Serial.println(" %      ");
    delay(1000);
}
void SendDataToRemote()
{
    btSerial.print("Temp: "); btSerial.print(this->Temperature); btSerial.print(" C\n");
    btSerial.print("Pr:   "); btSerial.print(this->Pressure); btSerial.print(" hPa\n");
    delay(5000);    
    btSerial.print("Alt:   "); btSerial.print(this->Altitude); btSerial.print(" m\n");
    btSerial.print("Light: "); btSerial.print(this->Lightness); btSerial.print(" %  \n");
    delay(5000);
}
};



void displaySensorDetails(void)
{
  sensor_t sensor;
  bmp.getSensor(&sensor);
  
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" hPa");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" hPa");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" hPa");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void setup(void) 
{
   // define pin modes for tx, rx pins:
   pinMode(rxPin, INPUT);
   pinMode(txPin, OUTPUT);
   btSerial.begin(38400);
   
  Serial.begin(9600);
  Serial.println("Pressure Sensor Test"); Serial.println("");
  
   
  /* Initialise the sensor */
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
    
  /* Display some basic information on this sensor */
  displaySensorDetails();
}
int counter = 0;
void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event;
  bmp.getEvent(&event);
  SensorData data;
  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure)
  {
    data.Pressure = event.pressure;
    
    float temperature;
    bmp.getTemperature(&temperature);
    data.Temperature = temperature;

    /* Then convert the atmospheric pressure, and SLP to altitude         */
    /* Update this next line with the current SLP for better results      */
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    float altitude = bmp.pressureToAltitude(seaLevelPressure, event.pressure);
    data.Altitude = altitude;  
    
    int lightValue = analogRead(lightSensorPin);
    float lightValueInPercent = 1.0 * lightValue / 1024 * 100;
    data.Lightness = lightValueInPercent;
    
    //data.DisplaySensorDataInTwoRows();    
  }
  else
  {
    Serial.println("Sensor error");
  }
  Serial.println("");
  data.SendDataToRemote();
}
