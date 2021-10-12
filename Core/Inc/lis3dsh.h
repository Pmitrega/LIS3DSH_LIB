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

#define LIS_CS_GPIO GPIOE
#define LIS_CS_GPIO_PIN GPIO_PIN_3

#define LIS_SPI_TIMEOUT 100

#define LIS_INFO1_ADDR          0x0D
#define LIS_INFO2_ADDR          0x0E
#define LIS_WHO_AM_I            0x0F
#define LIS_CTRL_REG3_ADDR      0x23
#define LIS_CTRL_REG4_ADDR      0x20
#define LIS_CTRL_REG5_ADDR      0x24
#define LIS_CTRL_REG6_ADDR      0x25

#define LIS_STATUS_ADDR         0x27
#define LIS_OUT_T_ADDR          0x0C
#define LIS_OFF_X_ADDR          0x10
#define LIS_OFF_Y_ADDR          0x11
#define LIS_OFF_Z_ADDR          0x12
#define LIS_OUT_X_L_ADDR        0x28
#define LIS_OUT_X_H_ADDR        0x29
#define LIS_OUT_Y_L_ADDR        0x2A
#define LIS_OUT_Y_H_ADDR        0x2B
#define LIS_OUT_Z_L_ADDR        0x2C
#define LIS_OUT_Z_H_ADDR        0x2D

#define LIS_MEASUREMENTS_MULTIPLIER 0.06f


struct LIS3DSH_ACC{
    float X;
    float Y;
    float Z;
};

struct LIS3DSH_Init_Sett{
    uint8_t CTRL_REG3;
    uint8_t CTRL_REG4;
    uint8_t CTRL_REG5;
    uint8_t CTRL_REG6;
};
//you have to use
void LIS3DSH_write_data(SPI_HandleTypeDef* hspi, uint8_t addr, uint8_t data){
    HAL_GPIO_WritePin(LIS_CS_GPIO, LIS_CS_GPIO_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(hspi, &addr,1, LIS_SPI_TIMEOUT);
    HAL_SPI_Transmit(hspi, &data,1, LIS_SPI_TIMEOUT);
    HAL_GPIO_WritePin(LIS_CS_GPIO, LIS_CS_GPIO_PIN, GPIO_PIN_SET);
}

void LIS3DSH_set_default_sett(struct LIS3DSH_Init_Sett* lis_sett){
    lis_sett->CTRL_REG3 = 0b00000000;
    lis_sett->CTRL_REG4 = 0b10010111;
    lis_sett->CTRL_REG5 = 0b00000000;
    lis_sett->CTRL_REG6 = 0b00000000;
};

void LIS3DSH_Init(SPI_HandleTypeDef* hspi ,struct LIS3DSH_Init_Sett* lis_sett){
    HAL_GPIO_WritePin(LIS_CS_GPIO, LIS_CS_GPIO_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
    uint8_t temp_addr = LIS_CTRL_REG3_ADDR;
    LIS3DSH_write_data(hspi, temp_addr, lis_sett->CTRL_REG3);
    temp_addr = LIS_CTRL_REG4_ADDR;
    LIS3DSH_write_data(hspi, temp_addr, lis_sett->CTRL_REG4);
    temp_addr = LIS_CTRL_REG5_ADDR;
    LIS3DSH_write_data(hspi, temp_addr, lis_sett->CTRL_REG5);
    temp_addr = LIS_CTRL_REG3_ADDR;
    LIS3DSH_write_data(hspi, temp_addr, (lis_sett->CTRL_REG6));

}

uint8_t LIS3DSH_read_data(SPI_HandleTypeDef* hspi, uint8_t addr){
    uint8_t data;
    addr |= 0x80;
    HAL_GPIO_WritePin(LIS_CS_GPIO, LIS_CS_GPIO_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(hspi, &addr, 1, LIS_SPI_TIMEOUT);
    HAL_SPI_Receive(hspi, &data, 1, LIS_SPI_TIMEOUT);
    HAL_GPIO_WritePin(LIS_CS_GPIO, LIS_CS_GPIO_PIN, GPIO_PIN_SET);
    return data;
}

uint8_t LIS3DSH_read_acc(SPI_HandleTypeDef* hspi,struct LIS3DSH_ACC* measurements){
    uint8_t l_byte;
    uint8_t h_byte;
    l_byte = LIS3DSH_read_data(hspi, LIS_OUT_X_L_ADDR);
    h_byte = LIS3DSH_read_data(hspi, LIS_OUT_X_H_ADDR);
    measurements->X = (float)((int16_t)(h_byte<<8 | l_byte)) *LIS_MEASUREMENTS_MULTIPLIER/1000.0f;
    l_byte = LIS3DSH_read_data(hspi, LIS_OUT_Y_L_ADDR);
    h_byte = LIS3DSH_read_data(hspi, LIS_OUT_Y_H_ADDR);
    measurements->Y = (float)((int16_t)(h_byte<<8 | l_byte)) * LIS_MEASUREMENTS_MULTIPLIER/1000.0f;
    l_byte = LIS3DSH_read_data(hspi, LIS_OUT_Z_L_ADDR);
    h_byte = LIS3DSH_read_data(hspi, LIS_OUT_Z_H_ADDR);
    measurements->Z = (float)((int16_t)(h_byte<<8 | l_byte)) * LIS_MEASUREMENTS_MULTIPLIER/1000.0f;

}



#endif //LIS3DSH_LIB_LIS3DSH_H
