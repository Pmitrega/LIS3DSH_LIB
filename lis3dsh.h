//
// Created by Paweł Mitręga on 12.10.2021.
//
//This is library to read LIS3DSH accelerometer
//using HAL library
//
//
#ifndef LIS3DSH_LIB_LIS3DSH_H
#define LIS3DSH_LIB_LIS3DSH_H

#include <stm32f4xx_hal.h>


/*raw output of sensor*/
typedef struct LIS3DSH_ACC{
    int16_t X;
    int16_t Y;
    int16_t Z;
}LIS3DSH_ACC;

/*acceleration in m/s^2*/
typedef struct LIS3DSH_ACC_SI{
    float X;
    float Y;
    float Z;
}LIS3DSH_ACC_MG;

typedef struct LIS3DSH_Sett{
    uint8_t CTRL_REG4;
    uint8_t CTRL_REG1;
    uint8_t CTRL_REG2;
    uint8_t CTRL_REG3;
    uint8_t CTRL_REG5;
    uint8_t CTRL_REG6;
}LIS3DSH_Sett;

typedef enum LIS3DSH_Sampling_Rate{
    LIS3DSH_SMP_PWR_DWN = 0x00,
    LIS3DSH_SMP_3_125HZ,
    LIS3DSH_SMP_6_25HZ,
    LIS3DSH_SMP_12_5HZ,
    LIS3DSH_SMP_25HZ,
    LIS3DSH_SMP_50HZ,
    LIS3DSH_SMP_100HZ,
    LIS3DSH_SMP_400HZ,
    LIS3DSH_SMP_1600HZ,
}LIS3DSH_Sampling_Rate;


typedef enum LIS3DSH_Scale{
    LIS3DSH_SCALE_2G,
    LIS3DSH_SCALE_4G,
    LIS3DSH_SCALE_6G,
    LIS3DSH_SCALE_8G,
    LIS3DSH_SCALE_16G,
    LIS3DSH_SCALE_SIZE
}LIS3DSH_Scale;


typedef struct LIS3DSH_Handler{
    GPIO_TypeDef* GPIO;
    uint16_t GPIO_Pin;
    SPI_HandleTypeDef* hspi;
    LIS3DSH_Sett settings;
    LIS3DSH_ACC raw_acc;
    LIS3DSH_ACC_MG acc_mg;
    LIS3DSH_Sampling_Rate sampling_rate;
    LIS3DSH_Scale scale;
}LIS3DSH_Handler;



void LIS3DSH_init(LIS3DSH_Handler* handler);
void LIS3DSH_read_acc_raw(LIS3DSH_Handler* handler);
void LIS3DSH_read_acc_mg(LIS3DSH_Handler* handler);
int LIS3DSH_check_for_data(LIS3DSH_Handler* handler);
void LIS3DSH_set_sampling_rate(LIS3DSH_Handler* handler, LIS3DSH_Sampling_Rate samp_rate);
void LIS3DSH_set_scale(LIS3DSH_Handler* handler, LIS3DSH_Scale scale);
extern LIS3DSH_Sett LIS3DSH_default_settings;



#define LIS3DSH_SPI_TIMEOUT 100

#define LIS3DSH_INFO1_ADDR          0x0D
#define LIS3DSH_INFO2_ADDR          0x0E
#define LIS3DSH_WHO_AM_I            0x0F

#define LIS3DSH_CTRL_REG4_ADDR      0x20
#define LIS3DSH_CTRL_REG1_ADDR      0x21
#define LIS3DSH_CTRL_REG2_ADDR      0x22
#define LIS3DSH_CTRL_REG3_ADDR      0x23
#define LIS3DSH_CTRL_REG5_ADDR      0x24
#define LIS3DSH_CTRL_REG6_ADDR      0x25

#define LIS3DSH_STATUS_ADDR         0x27
#define LIS3DSH_OUT_T_ADDR          0x0C
#define LIS3DSH_OFF_X_ADDR          0x10
#define LIS3DSH_OFF_Y_ADDR          0x11
#define LIS3DSH_OFF_Z_ADDR          0x12
#define LIS3DSH_OUT_X_L_ADDR        0x28
#define LIS3DSH_OUT_X_H_ADDR        0x29
#define LIS3DSH_OUT_Y_L_ADDR        0x2A
#define LIS3DSH_OUT_Y_H_ADDR        0x2B
#define LIS3DSH_OUT_Z_L_ADDR        0x2C
#define LIS3DSH_OUT_Z_H_ADDR        0x2D

#define LIS3DSH_MEASUREMENTS_MULTIPLIER 0.06f


#endif //LIS3DSH_LIB_LIS3DSH
