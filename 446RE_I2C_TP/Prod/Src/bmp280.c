#include "bmp280.h"
#include "main.h"
#include <stdio.h>

void bmp_reset(h_bmp280_t * bmp280){
	bmp280->registre = reset;
	bmp280->buf[0]= (bmp280->registre) & 0xFF;
	bmp280->buf[1]= BMP280_RESET;
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 2, HAL_MAX_DELAY)){
		printf("echec du reset\r\n");
	}
}

void bmp_id(h_bmp280_t * bmp280){
	bmp280->registre = id;
	bmp280->buf[0]= (bmp280->registre) & 0xFF;
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 1, HAL_MAX_DELAY)){
		printf("echec de l'id\r\n");
	}
	if(!HAL_I2C_Master_Receive(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 1, 1000)){
		printf("val id : %x\r\n",bmp280->buf[0]);
	}
}
