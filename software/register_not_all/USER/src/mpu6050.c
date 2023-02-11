//
// Created by 27900 on 2023/2/1.
//
#include "../inc/mpu6050.h"
uint16_t Accel_X_RAW = 0;
uint16_t Accel_Y_RAW = 0;
uint16_t Accel_Z_RAW = 0;

uint16_t Gyro_X_RAW = 0;
uint16_t Gyro_Y_RAW = 0;
uint16_t Gyro_Z_RAW = 0;

float Ax, Ay, Az, Gx, Gy, Gz;

uint8_t check;
void MPU_Write (uint8_t Address, uint8_t Reg, uint8_t Data)
{
    I2C_Start (MPU_I2C);
    I2C_Address (MPU_I2C,Address);
    I2C_Write (MPU_I2C,Reg);
    I2C_Write (MPU_I2C,Data);
    I2C_Stop (MPU_I2C);
}

void MPU_Read (uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size)
{
    I2C_Start (MPU_I2C);
//    printf("start su\n");
    I2C_Address (MPU_I2C,Address);
//    printf("Address su\n");
    I2C_Write (MPU_I2C,Reg);
//    printf("write su\n");
    I2C_Stop(MPU_I2C);
//    printf("stop su\n");
    I2C_Start (MPU_I2C);  // repeated start
    I2C_Read (MPU_I2C,Address+0x01, buffer, size);
    I2C_Stop (MPU_I2C);
}
void MPU6050_Init(){
    uint8_t check;
    uint8_t Data;
    MPU_Read (MPU6050_ADDR,WHO_AM_I_REG, &check, 1);
//    printf("check: %d\n",check);
    if (check == 104)  // 0x68 will be returned by the sensor if everything goes well
    {
        // power management register 0X6B we should write all 0's to wake the sensor up
        Data = 0;
        MPU_Write (MPU6050_ADDR, PWR_MGMT_1_REG, Data);

        // Set DATA RATE of 1KHz by writing SMPLRT_DIV register
        Data = 0x07;
        MPU_Write(MPU6050_ADDR, SMPLRT_DIV_REG, Data);

        // Set accelerometer configuration in ACCEL_CONFIG Register
        // XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> ? 2g
        Data = 0x00;
        MPU_Write(MPU6050_ADDR, ACCEL_CONFIG_REG, Data);

        // Set Gyroscopic configuration in GYRO_CONFIG Register
        // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ? 250 ?/s
        Data = 0x00;
        MPU_Write(MPU6050_ADDR, GYRO_CONFIG_REG, Data);
    }
}
void MPU6050_Read_Accel (void)
{

    uint8_t Rx_data[6];

    // Read 6 BYTES of data starting from ACCEL_XOUT_H register

    MPU_Read (MPU6050_ADDR, ACCEL_XOUT_H_REG, Rx_data, 6);

    Accel_X_RAW = (uint16_t)(Rx_data[0] << 8 | Rx_data [1]);
    Accel_Y_RAW = (uint16_t)(Rx_data[2] << 8 | Rx_data [3]);
    Accel_Z_RAW = (uint16_t)(Rx_data[4] << 8 | Rx_data [5]);

    /*** convert the RAW values into acceleration in 'g'
         we have to divide according to the Full scale value set in FS_SEL
         I have configured FS_SEL = 0. So I am dividing by 16384.0
         for more details check ACCEL_CONFIG Register              ****/

    Ax = Accel_X_RAW/16384.0;
    Ay = Accel_Y_RAW/16384.0;
    Az = Accel_Z_RAW/16384.0;
}
void MPU6050_Read_Gyro (void)
{

    uint8_t Rx_data[6];

    // Read 6 BYTES of data starting from ACCEL_XOUT_H register

    MPU_Read (MPU6050_ADDR, GYRO_XOUT_H_REG, Rx_data, 6);

    Accel_X_RAW = (uint16_t)(Rx_data[0] << 8 | Rx_data [1]);
    Accel_Y_RAW = (uint16_t)(Rx_data[2] << 8 | Rx_data [3]);
    Accel_Z_RAW = (uint16_t)(Rx_data[4] << 8 | Rx_data [5]);

    Ax = Accel_X_RAW;
    Ay = Accel_Y_RAW;
    Az = Accel_Z_RAW;
}
void MPU6050_Read_temp(void)
{
    uint8_t Rx_data[2];
    MPU_Read (MPU6050_ADDR, TEMP_OUT_H_REG, Rx_data, 2);
    Accel_X_RAW = (uint16_t)(Rx_data[0]<<8 | Rx_data[1]);
    Ax = Accel_X_RAW;

}