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
	if(!HAL_I2C_Master_Receive(&hi2c1, (uint16_t) BMP280_ADDR<<1, &bmp280->id, 1, 1000)){
		printf("val id : %x\r\n",bmp280->id);
	}
}

/*
void bmp_id(h_bmp280_t * bmp280){
	bmp280->registre = id;
	bmp280->buf[0]= (bmp280->registre) & 0xFF;
	if(HAL_I2C_Mem_Read(&hi2c1, (uint16_t) BMP280_ADDR, bmp280->registre & 0xFF, 1, &bmp280->id, 1, HAL_MAX_DELAY)){
		printf("echec de l'id\r\n");
	}
	printf("val id : %x\r\n",bmp280->id);
}*/

void bmp_config(h_bmp280_t * bmp280){
	bmp280->registre = ctrl_mes;
	bmp280->buf[0]= (bmp280->registre) & 0xFF;
	bmp280->buf[1]= 0b01010111;
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 2, HAL_MAX_DELAY)){
		printf("echec de config\r\n");
	}
	if(!HAL_I2C_Master_Receive(&hi2c1, (uint16_t) BMP280_ADDR<<1, &bmp280->config, 1, 1000)){
		printf("config value : %x\r\n",bmp280->config);
	}
	//HAL_I2C_Mem_Read(hi2c, DevAddress, MemAddress, MemAddSize, pData, Size, Timeout)
}

void bmp_calib_read(h_bmp280_t * bmp280){
	bmp280->registre = calib00;
	bmp280->buf[0]= (bmp280->registre) & 0xFF;
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 1, HAL_MAX_DELAY)){
		printf("echec de calib\r\n");
	}
	if(!HAL_I2C_Master_Receive(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->calib, 25, HAL_MAX_DELAY)){
		printf("calib values : %s\r\n",bmp280->calib);
	}
	//HAL_I2C_Mem_Read(hi2c, DevAddress, MemAddress, MemAddSize, pData, Size, Timeout)
}

void bmp_temp_read(h_bmp280_t * bmp280){
	bmp280->registre = temp_xlsb;
	bmp280->buf[0]= (bmp280->registre) & 0xFF;
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 1, HAL_MAX_DELAY)){
		printf("echec de temp read\r\n");
	}
	if(!HAL_I2C_Master_Receive(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 1, HAL_MAX_DELAY)){
		bmp280->temp = (int32_t)((bmp280->buf[0]) >>4);
		/*bmp280->temp = bmp280->buf[0]>>4;
		int32_t temp_shifting = bmp280->buf[1];
		temp_shifting = temp_shifting<<4;
		bmp280->temp += temp_shifting;
		temp_shifting = bmp280->buf[2];
		temp_shifting = temp_shifting<<12;
		bmp280->temp += temp_shifting;
		bmp280->temp = ((int32_t)bmp280->buf[0]>>4) + ((int32_t)bmp280->buf[1]<<4) + ((int32_t)bmp280->buf[2]<<12);
		printf("temp brute : %ld\r\n",bmp280->temp);
		bmp280->temp = bmp_compensate_T(bmp280);
		printf("temp : %ld\r\n",bmp280->temp);*/
	}
	bmp280->registre = temp_lsb;
	bmp280->buf[0]= (bmp280->registre) & 0xFF;
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 1, HAL_MAX_DELAY)){
		printf("echec de temp read\r\n");
	}
	if(!HAL_I2C_Master_Receive(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 1, HAL_MAX_DELAY)){
		bmp280->temp += ((int32_t)(bmp280->buf[0]))<<4;
	}
	bmp280->registre = temp_msb;
	bmp280->buf[0]= (bmp280->registre) & 0xFF;
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 1, HAL_MAX_DELAY)){
		printf("echec de temp read\r\n");
	}
	if(!HAL_I2C_Master_Receive(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 1, HAL_MAX_DELAY)){
		bmp280->temp += ((int32_t)(bmp280->buf[0]))<<12;
	}
	printf("temp : %ld\r\n",bmp280->temp);
}

int32_t bmp_compensate_T(h_bmp280_t * bmp280){
	int32_t adc_T = bmp280->temp;
	uint16_t dig_T1;
	int16_t	 dig_T2, dig_T3;
	dig_T1 =((uint16_t) bmp280->calib[0])+ (((uint16_t) bmp280->calib[1])<<8);
	dig_T2 =((int16_t) bmp280->calib[2]) + (((int16_t) bmp280->calib[3])<<8);
	dig_T3 =((int16_t) bmp280->calib[4]) + (((int16_t) bmp280->calib[5])<<8);

	int32_t var1, var2, T;
	var1 = ((((adc_T>>3)-((int32_t)dig_T1<<1)))*((int32_t)dig_T2))>>11;
	var2 = (((((adc_T>>4)-((int32_t)dig_T1))*((adc_T>>4)-((int32_t)dig_T1)))>>12)*((int32_t)dig_T3))>>14;
	int32_t t_fine = var1+ var2;
	T = (t_fine*5 + 128)>>8;
	return T;
}
