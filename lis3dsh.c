#include "lis3dsh.h"

LIS3DSH_Sett LIS3DSH_default_settings = {0x97, 0x00, 0x00, 0x00, 0x00, 0x10};
static const float LIS3DSH_sensivity[LIS3DSH_SCALE_SIZE] = {0.06f, 0.12f, 0.18f, 0.24f, 0.73f};
void LIS3DSH_write_data(LIS3DSH_Handler* handler, uint8_t addr, uint8_t* data, uint16_t len){
    addr &= 0x7F;
    HAL_GPIO_WritePin(handler->GPIO, handler->GPIO_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(handler->hspi, &addr, 1, LIS3DSH_SPI_TIMEOUT);
    HAL_SPI_Transmit(handler->hspi, data, len, LIS3DSH_SPI_TIMEOUT);
    HAL_GPIO_WritePin(handler->GPIO, handler->GPIO_Pin, GPIO_PIN_SET);
}
void LIS3DSH_read_data(LIS3DSH_Handler* handler, uint8_t addr, uint8_t* data, uint16_t len){
    addr = addr | 0x80;
    HAL_GPIO_WritePin(handler->GPIO, handler->GPIO_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(handler->hspi, &addr, 1, LIS3DSH_SPI_TIMEOUT);
    HAL_SPI_Receive(handler->hspi, data, len, LIS3DSH_SPI_TIMEOUT);
    HAL_GPIO_WritePin(handler->GPIO, handler->GPIO_Pin, GPIO_PIN_SET);
}


void LIS3DSH_init(LIS3DSH_Handler* handler){
    uint8_t temp_addr = LIS3DSH_CTRL_REG4_ADDR;
    handler->raw_acc.X = 0;
    handler->raw_acc.Y = 0;
    handler->raw_acc.Z = 0;
    LIS3DSH_write_data(handler, temp_addr, (uint8_t*)(&(handler->settings.CTRL_REG1)),  sizeof(LIS3DSH_Sett));
    LIS3DSH_write_data(handler, LIS3DSH_CTRL_REG1_ADDR, (uint8_t*)(&(handler->settings.CTRL_REG1)), 1);
    LIS3DSH_write_data(handler, LIS3DSH_CTRL_REG2_ADDR, (uint8_t*)(&(handler->settings.CTRL_REG2)), 1);
    LIS3DSH_write_data(handler, LIS3DSH_CTRL_REG3_ADDR, (uint8_t*)(&(handler->settings.CTRL_REG3)), 1);
    LIS3DSH_write_data(handler, LIS3DSH_CTRL_REG5_ADDR, (uint8_t*)(&(handler->settings.CTRL_REG5)), 1);
    LIS3DSH_write_data(handler, LIS3DSH_CTRL_REG6_ADDR, (uint8_t*)(&(handler->settings.CTRL_REG6)), 1);
    LIS3DSH_write_data(handler, LIS3DSH_CTRL_REG4_ADDR, (uint8_t*)(&(handler->settings.CTRL_REG4)), 1); // last
    LIS3DSH_read_data(handler, temp_addr, (uint8_t*)(&(handler->settings)), sizeof(LIS3DSH_Sett));
}



int LIS3DSH_check_for_data(LIS3DSH_Handler* handler){
    uint8_t data;
    LIS3DSH_read_data(handler, LIS3DSH_STATUS_ADDR, &data, 1);
    if(data & 0x08){
        return 1;
    }
    return 0;
}

void LIS3DSH_read_acc_raw(LIS3DSH_Handler* handler){
    LIS3DSH_read_data(handler, LIS3DSH_OUT_X_L_ADDR, (void*)(&handler->raw_acc), sizeof(LIS3DSH_ACC));
}

void LIS3DSH_read_acc_mg(LIS3DSH_Handler* handler){
    LIS3DSH_read_data(handler, LIS3DSH_OUT_X_L_ADDR, (void*)(&handler->raw_acc), sizeof(LIS3DSH_ACC));
    handler->acc_mg.X = (float)handler->raw_acc.X*LIS3DSH_sensivity[handler->scale];
    handler->acc_mg.Y = (float)handler->raw_acc.Y*LIS3DSH_sensivity[handler->scale];
    handler->acc_mg.Z = (float)handler->raw_acc.Z*LIS3DSH_sensivity[handler->scale];
}


void LIS3DSH_set_sampling_rate(LIS3DSH_Handler* handler, LIS3DSH_Sampling_Rate samp_rate){
    uint8_t ctrl_reg4;
    LIS3DSH_read_data(handler, LIS3DSH_CTRL_REG4_ADDR, &ctrl_reg4, 1);
    ctrl_reg4 = (ctrl_reg4 & 0x07) | (samp_rate << 4);
    LIS3DSH_write_data(handler, LIS3DSH_CTRL_REG4_ADDR, &(ctrl_reg4), 1);
    LIS3DSH_read_data(handler, LIS3DSH_CTRL_REG4_ADDR, &(handler->settings.CTRL_REG4), 1);
    if(ctrl_reg4 == handler->settings.CTRL_REG4){
        handler->sampling_rate = samp_rate;
    }
}
void LIS3DSH_set_scale(LIS3DSH_Handler* handler, LIS3DSH_Scale scale){
    uint8_t ctrl_reg5;
    LIS3DSH_read_data(handler, LIS3DSH_CTRL_REG5_ADDR, &ctrl_reg5, 1);
    ctrl_reg5 = (ctrl_reg5 & 0xC7) | (scale << 3);
    LIS3DSH_write_data(handler, LIS3DSH_CTRL_REG5_ADDR, &(ctrl_reg5), 1);
    LIS3DSH_read_data(handler, LIS3DSH_CTRL_REG5_ADDR, &(handler->settings.CTRL_REG5), 1);
    if(ctrl_reg5 == handler->settings.CTRL_REG5){
        handler->scale = scale;
    }
}


uint8_t LIS3DSH_get_ID(LIS3DSH_Handler* handler){
    uint8_t data;
    LIS3DSH_read_data(handler, LIS3DSH_WHO_AM_I, &data, sizeof(LIS3DSH_ACC));
    return data;
}



