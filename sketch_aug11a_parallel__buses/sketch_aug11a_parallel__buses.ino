// Copyright © 2019 Richard Gemmell
// Released under the MIT License. See license.txt. (https://opensource.org/licenses/MIT)
//
// Demonstrates use of the raw I2C driver.
// Creates an I2C master and probes the bus to find any listening slave devices.

#include <Arduino.h>
#include <i2c_driver.h>
#include "imx_rt1060/imx_rt1060_i2c_driver.h"

I2CMaster& master = Master;
float gyro1_x, gyro1_y, gyro1_z, acc1_x, acc1_y, acc1_z;
float gyro2_x, gyro2_y, gyro2_z, acc2_x, acc2_y, acc2_z;
int16_t accel_x, accel_y, accel_z;
int16_t gyro_x, gyro_y, gyro_z;
byte mpu1 = 104;
byte mpu2 = 105;

void finish(bool report_timeout);

void setup() {
  // Initialise the master
  master.begin(100 * 1000U);

  Serial.begin(921600);
  while (!Serial);
}

void loop() {
  Serial.println("Getting data");

  uint8_t buffer[] = {0};

  master.read_async(mpu1, buffer, 1, true);
  finish(false);
  master.write_async(mpu1, buffer, 0, true);
  finish(true);

  I2CError status = master.error();
  if (status == I2CError::ok) {
    Serial.print("Slave device found at address ");
    Serial.println(mpu1);
  }
  else {
    Serial.print("Unexpected error at address ");
    Serial.println(mpu1);
  }

  master = Master1;
  pinMode(23, OUTPUT);
  digitalWrite(23, HIGH);

  master.read_async(mpu2, buffer, 1, true);
  finish(false);
  master.write_async(mpu2, buffer, 0, true);
  finish(true);

  status = master.error();
  if (status == I2CError::ok) {
    Serial.print("Slave device found at address ");
    Serial.println(mpu2);
  }
  else {
    Serial.print("Unexpected error at address ");
    Serial.println(mpu2);
  }

  Serial.println();

  delay(3'000);   // wait a while before scanning again
      }

        void finish(bool report_timeout) {
        elapsedMillis timeout;
        while (timeout < 200) {
        if (master.finished()){
        return;
      }
      }
        if (report_timeout) {
        Serial.println("Master: ERROR timed out waiting for transfer to finish.");
      }
      }
