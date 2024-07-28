
/*
  Florin Simedru
  Complete project details at https://blog.automatic-house.ro

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
/*
; PlatformIO Project Configuration File
;
;   Build options: build flags, source filter
;   Upload options: custom upload port, speed and extra flags
;   Library options: dependencies, extra library storages
;   Advanced options: extra scripting
;
; Please visit documentation for the other options and examples
; https://docs.platformio.org/page/projectconf.html

[env:esp32doit-devkit-v1]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino
upload_port = COM9
monitor_port = COM9
monitor_speed = 115200
lib_deps = hideakitai/ESP32SPISlave@^0.2.0
*/
#include <Arduino.h>
#include <ESP32SPISlave.h>

ESP32SPISlave slave;

static constexpr uint32_t BUFFER_SIZE{32};
uint8_t spi_slave_tx_buf[BUFFER_SIZE];
uint8_t spi_slave_rx_buf[BUFFER_SIZE];

#define LED 2
void setup()
{
  Serial.begin(115200);
  delay(2000);
  pinMode(LED, OUTPUT);
  // begin() after setting
  // HSPI = CS: 15, CLK: 14, MOSI: 13, MISO: 12 -> default
  // VSPI = CS:  5, CLK: 18, MOSI: 23, MISO: 19
  slave.setDataMode(SPI_MODE0);
  // slave.begin();
  slave.begin(VSPI); // you can use VSPI like this

  // clear buffers
  memset(spi_slave_tx_buf, 0, BUFFER_SIZE);
  memset(spi_slave_rx_buf, 0, BUFFER_SIZE);
  Serial.println("Init ok! ");
}

void loop()
{
  // block until the transaction comes from master
  slave.wait(spi_slave_rx_buf, spi_slave_tx_buf, BUFFER_SIZE);

  // if transaction has completed from master,
  // available() returns size of results of transaction,
  // and buffer is automatically updated
  char data=-1;
  while (slave.available())
  {
    // show received data
    Serial.print("Command Received: ");
    Serial.println(spi_slave_rx_buf[0]);
    data = spi_slave_rx_buf[0];
    slave.pop();
  }
  if (data == 255)
  {
    Serial.println("Setting LED active HIGH ");
    digitalWrite(LED, HIGH);
  }
  else if (data == 0)
  {
    Serial.println("Setting LED active LOW ");
    digitalWrite(LED, LOW);
  }
  Serial.println("");
}
