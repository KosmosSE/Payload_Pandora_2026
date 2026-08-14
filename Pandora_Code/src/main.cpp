#include <Arduino.h>
#include <wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL375.h>
#include <Adafruit_BME280.h>
#include <Adafruit_MPU6050.h>
#include <SD.h>
#include <SPI.h>
#include <stdio.h>
#include <string.h>
#include <TinyGPSPlus.h>

#define chipSelect 5

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C BME endereçõ 0x76
Adafruit_MPU6050 mpu; // I2C MPU 
Adafruit_ADXL375 adxl = Adafruit_ADXL375(12345);
TinyGPSPlus gps; // GPS
HardwareSerial gpsSerial(2); // Serial para o GPS

void dadosBME();
void readGPS(TinyGPSPlus &gps, HardwareSerial &SerialGPS);

void setup()
{
  Serial.begin(115200);
  Serial.println("Satélite Pandora 2026");

  gpsSerial.begin(9600, SERIAL_8N1, 17, 16);

    while (!gpsSerial){
        Serial.println("Erro: Falha ao iniciar GPS");
        delay(10);
    }
  
  bool status;
  status = bme.begin(0x76);  
  //Debug bme
  if (!status) {
    Serial.println("Erro no BME280, mal contato");
    while (1);
  }

  //adxl
  /* Initialise the sensor */
  if(!adxl.begin(0x53))
  {
    /* There was a problem detecting the ADXL343 ... check your connections */
    Serial.println("Ooops, no ADXL375 detected ... Check your wiring!");
    while(1);
  }
  
  // Try to initialize!
  if (!mpu.begin(0x68)) {
    Serial.println("Failed to find MPU6050 chip");
  }
  //Debug cartão SD
  if (!SD.begin(chipSelect)) {
    Serial.println("Card Mount Failed");
    return;
    }
  File file = SD.open("/dados_pandora.csv", FILE_WRITE); //Testar se isso funciona
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }
    file.println("Satélite Pandora 2026");
    file.println("ADXL375, , ,MPU6050 aceleração linear, , ,aceleração angular, , ,temperatura, pressão, umidade, altitude, latitude, longitude");
    file.println("eixo x, eixo y, eixo z, eixo x, eixo y, eixo z, angular x, angular y, angular z, temperatura, pressão, umidade, altitude, latitude, longitude");
    file.close();
    Serial.println("File written");

  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
}

void loop()
{
  dadosBME();
  readGPS(gps, gpsSerial);
  /* Get a new sensor event */
  sensors_event_t event;
  adxl.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");
  Serial.println("m/s^2 ");
  
  Serial.print("Raw X: "); Serial.print(adxl.getX()); Serial.print("  ");
  Serial.print("Y: "); Serial.print(adxl.getY()); Serial.print("  ");
  Serial.print("Z: "); Serial.print(adxl.getZ()); Serial.print("  ");
  Serial.println(" counts");
  Serial.println();
  delay(500);

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");
}

void readGPS(TinyGPSPlus &gps, HardwareSerial &SerialGPS){
      // Lê os dados do GPS
    
    printf("%d",SerialGPS.available());
    while (SerialGPS.available() > 0) {
        gps.encode(SerialGPS.read());
      }
  
    if (gps.location.isUpdated()) {
      Serial.print("Lat: ");
      Serial.println(gps.location.lat());
      Serial.print("Lng: ");
      Serial.println(gps.location.lng());
      Serial.print("Satélites: ");
      Serial.println(gps.satellites.value());
      Serial.println("-----");
    }

}


void dadosBME() {
 
  char Temperatura [10];
  snprintf(Temperatura ,sizeof(Temperatura), "%.2f", bme.readTemperature());
  strcat(Temperatura, " °C, ");
  
  char pressao[15];
  snprintf(pressao, sizeof(pressao), "%.2f", bme.readPressure());
  strcat(pressao, " Pa, ");
  
  char altitude[10];
  snprintf(altitude, sizeof(altitude), "%.2f", bme.readAltitude(SEALEVELPRESSURE_HPA));
  strcat(altitude, " m, ");
  
  char umidade[10];
  snprintf(umidade, sizeof(umidade),"%.2f", bme.readHumidity());
  strcat(umidade, " %, ");

  char mensagem[100] = "";
  strcat(mensagem, Temperatura);
  strcat(mensagem, pressao);
  strcat(mensagem, altitude);
  //strcat(mensagem, umidade);
  
  Serial.println(mensagem);
}