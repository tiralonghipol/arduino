//#include <Wire.h>
#include <i2c_driver.h>
#include <i2c_driver_wire.h>

float gyro1_x, gyro1_y, gyro1_z, acc1_x, acc1_y, acc1_z;
float gyro2_x, gyro2_y, gyro2_z, acc2_x, acc2_y, acc2_z;
int16_t accel_x, accel_y, accel_z;
int16_t gyro_x, gyro_y, gyro_z;
byte mpu1 = 104;
byte mpu2 = 105;


void getDataMPU1(int address, float* gX, float* gY, float* gZ, float* aX, float* aY, float* aZ)
{
  // request accelerometer data
  Wire.beginTransmission(address);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(address, 6);
  accel_x = Wire.read() << 8 | Wire.read();
  accel_y = Wire.read() << 8 | Wire.read();
  accel_z = Wire.read() << 8 | Wire.read();
  *aX = accel_x / 2048.0;  // value divided by configured LSB
  *aY = accel_y / 2048.0;
  *aZ = accel_z / 2048.0;

  // request gyro data
  Wire.beginTransmission(address);
  Wire.write(0x43); // access storing measuments gyro registers
  Wire.endTransmission();
  Wire.requestFrom(address, 6);
  gyro_x = Wire.read() << 8 | Wire.read();
  gyro_y = Wire.read() << 8 | Wire.read();
  gyro_z = Wire.read() << 8 | Wire.read();
  *gX = (float)gyro_x / 65.5; // value divided by configured LSB
  *gY = (float)gyro_y / 65.5;
  *gZ = (float)gyro_z / 65.5;
}

void getDataMPU2(int address, float* gX, float* gY, float* gZ, float* aX, float* aY, float* aZ)
{
  //  pinMode(22, OUTPUT);
  //  digitalWrite(22, HIGH);
  // request accelerometer data
  Wire1.beginTransmission(address);
  Wire1.write(0x3B);
  Wire1.endTransmission();
  Wire1.requestFrom(address, 6);
  accel_x = Wire1.read() << 8 | Wire1.read();
  accel_y = Wire1.read() << 8 | Wire1.read();
  accel_z = Wire1.read() << 8 | Wire1.read();
  *aX = accel_x / 2048.0;  // value divided by configured LSB
  *aY = accel_y / 2048.0;
  *aZ = accel_z / 2048.0;

  // request gyro data
  Wire1.beginTransmission(address);
  Wire1.write(0x43); // access storing measuments gyro registers
  Wire1.endTransmission();
  Wire1.requestFrom(address, 6);
  gyro_x = Wire1.read() << 8 | Wire1.read();
  gyro_y = Wire1.read() << 8 | Wire1.read();
  gyro_z = Wire1.read() << 8 | Wire1.read();
  *gX = (float)gyro_x / 65.5; // value divided by configured LSB
  *gY = (float)gyro_y / 65.5;
  *gZ = (float)gyro_z / 65.5;
}


void setup()
{
  pinMode(22, OUTPUT);
  digitalWrite(22, HIGH);
  pinMode(23, OUTPUT);
  digitalWrite(23, LOW);

//  Wire.setSCL(19);  // SCL on first i2c bus on T4.1
//  Wire.setSDA(18);  // SDA on first i2c bus on T4.1
//  Wire1.setSCL(16); // SCL1 on second i2c bus on T4.1
//  Wire1.setSDA(17); // SDA1 on second i2c bus on T4.1

  Wire.setClock(4e5);
  Wire1.setClock(4e5);

  Wire.begin();
  Wire1.begin();

  delay(250);  // need some time to start

  // configure DLPF_CFG
  // mpu1
  Wire.beginTransmission(mpu1); // contact register 117 Who Am I
  Wire.write(0x1A); // contact register 26
  Wire.write(0x05); // configure DLPF_CFG to 10Hz
  Wire.endTransmission();
  // mpu2
  Wire1.beginTransmission(mpu2); // contact register 117 Who Am I
  Wire1.write(0x1A); // contact register 26
  Wire1.write(0x05); // configure DLPF_CFG to 10Hz
  Wire1.endTransmission();

  Wire.beginTransmission(mpu2);
  Wire.write(0x1B); // contact register 27 - gyro config
  Wire.write(0x08); // configure GYRO_CONFIG to 65.5 LSB*/s
  Wire.endTransmission();

  // configure ACCEL_CONFIG
  Wire.beginTransmission(mpu1);
  Wire.write(0x1C);
  Wire.write(0x03);
  Wire.endTransmission();
  Wire1.beginTransmission(mpu2);
  Wire1.write(0x1C);
  Wire1.write(0x03);
  Wire1.endTransmission();

  //  SMPLRT_DIV
  Wire.beginTransmission(mpu1);
  Wire.write(0x19);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire1.beginTransmission(mpu2);
  Wire1.write(0x19);
  Wire1.write(0x00);
  Wire1.endTransmission();

  // set powermode register 107 - power management
  Wire.beginTransmission(mpu1);
  Wire.write(0x6B);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire1.beginTransmission(mpu2); // set powermode
  Wire1.write(0x6B);
  Wire1.write(0b00000000);
  Wire1.endTransmission();

  Serial.begin(921600);

}

void loop()
{

    getDataMPU1(mpu1, &gyro1_x, &gyro1_y, &gyro1_z, &acc1_x, &acc1_y, &acc1_z );
    getDataMPU2(mpu2, &gyro2_x, &gyro2_y, &gyro2_z, &acc2_x, &acc2_y, &acc2_z );
  
    Serial.print(String(gyro1_x) + " " + gyro1_y + " " + gyro1_z + " " + acc1_x + " " + acc1_y + " " + acc1_z + " ");
    Serial.print(String(gyro2_x) + " " + gyro2_y + " " + gyro2_z + " " + acc2_x + " " + acc2_y + " " + acc2_z);
    Serial.println();

  //  delay(800);
}
