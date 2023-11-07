#include "bmp280.h"
#include "main.h"
#include <stdio.h>

static int32_t t_fine; //Used by both temp and press functions (needs to be called by the temp funct at first)

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
	//printf("temp : %ld\r\n",bmp280->temp);

	bmp280->temp = bmp_compensate_T(bmp280);

	//printf("celcius temp : %ld\r\n",bmp280->temp);

}

void bmp_press_read(h_bmp280_t * bmp280){
	/*bmp280->registre = press_xlsb;
	bmp280->buf[0]= (bmp280->registre) & 0xFF;
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 1, HAL_MAX_DELAY)){
		printf("echec de press_xlsb read\r\n");
	}
	if(!HAL_I2C_Master_Receive(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 1, HAL_MAX_DELAY)){
		bmp280->press = (int32_t)((bmp280->buf[0]) >>4);
	}

	bmp280->registre = press_lsb;
	bmp280->buf[0]= (bmp280->registre) & 0xFF;
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 1, HAL_MAX_DELAY)){
		printf("echec de press_lsb read\r\n");
	}
	if(!HAL_I2C_Master_Receive(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 1, HAL_MAX_DELAY)){
		bmp280->press += ((int32_t)(bmp280->buf[0]))<<4;
	}

	bmp280->registre = press_msb;
	bmp280->buf[0]= (bmp280->registre) & 0xFF;
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 1, HAL_MAX_DELAY)){
		printf("echec de press_msb read\r\n");
	}
	if(!HAL_I2C_Master_Receive(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 1, HAL_MAX_DELAY)){
		bmp280->press += ((int32_t)(bmp280->buf[0]))<<12;
	}
	printf("press : %ld\r\n",bmp280->press);*/

	bmp280->registre = press_msb;
	bmp280->buf[0]= (bmp280->registre) & 0xFF;
	if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 1, HAL_MAX_DELAY)){
		printf("echec de press_xlsb read\r\n");
	}
	if(!HAL_I2C_Master_Receive(&hi2c1, (uint16_t) BMP280_ADDR<<1, bmp280->buf, 3, HAL_MAX_DELAY)){
		bmp280->press = (int32_t)((bmp280->buf[2]) >>4) + ((int32_t)(bmp280->buf[1]) <<4) + ((int32_t)(bmp280->buf[0]) <<12);
	}

	//printf("press : %ld\r\n",bmp280->press);

	bmp280->press = bmp_compensate_P(bmp280);

	//printf("corrected press : %ld\r\n",bmp280->press);

}

int32_t bmp_compensate_T(h_bmp280_t * bmp280){
	int32_t adc_T = bmp280->temp;

	//Extracting calibration values
	uint16_t dig_T1;
	int16_t	 dig_T2, dig_T3;
	dig_T1 =((uint16_t) bmp280->calib[0]) + (((uint16_t) bmp280->calib[1])<<8);
	dig_T2 =((int16_t) bmp280->calib[2]) + (((int16_t) bmp280->calib[3])<<8);
	dig_T3 =((int16_t) bmp280->calib[4]) + (((int16_t) bmp280->calib[5])<<8);

	//Temperature calculus (in 1/100th deg C)
	int32_t var1, var2, T;
	var1 = ((((adc_T>>3)-((int32_t)dig_T1<<1)))*((int32_t)dig_T2))>>11;
	var2 = (((((adc_T>>4)-((int32_t)dig_T1))*((adc_T>>4)-((int32_t)dig_T1)))>>12)*((int32_t)dig_T3))>>14;
	t_fine = var1+ var2;
	T = (t_fine*5 + 128)>>8;
	return T;
}

int32_t bmp_compensate_P(h_bmp280_t * bmp280){
	//Extracting calibration values
	uint16_t dig_P1;
	int16_t	 dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
	dig_P1 =((uint16_t) bmp280->calib[6])+ (((uint16_t) bmp280->calib[7])<<8);
	dig_P2 =((int16_t) bmp280->calib[8]) + (((int16_t) bmp280->calib[9])<<8);
	dig_P3 =((int16_t) bmp280->calib[10]) + (((int16_t) bmp280->calib[11])<<8);
	dig_P4 =((int16_t) bmp280->calib[12]) + (((int16_t) bmp280->calib[13])<<8);
	dig_P5 =((int16_t) bmp280->calib[14]) + (((int16_t) bmp280->calib[15])<<8);
	dig_P6 =((int16_t) bmp280->calib[16]) + (((int16_t) bmp280->calib[17])<<8);
	dig_P7 =((int16_t) bmp280->calib[18]) + (((int16_t) bmp280->calib[19])<<8);
	dig_P8 =((int16_t) bmp280->calib[20]) + (((int16_t) bmp280->calib[21])<<8);
	dig_P9 =((int16_t) bmp280->calib[22]) + (((int16_t) bmp280->calib[23])<<8);

	//Pressure calculus
	int64_t var1, var2, P;
	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)dig_P6;
	var2 = var2 + ((var1*(int64_t)dig_P5)<<17);
	var2 = var2 + (((int64_t)dig_P4)<<35);
	var1 = ((var1 * var1 * (int64_t)dig_P3)>>8) + ((var1 * (int64_t)dig_P2)<<12);
	var1 = (((((int64_t)1)<<47) + var1))*((int64_t)dig_P1)>>33;
	if(var1 == 0){
		return 0;  //avoid exception caused by division by zero
	}
	P = 1048576 - bmp280->press;
	P = (((P<<31)-var2)*3125)/var1;
	var1 = (((int64_t)dig_P9) * (P>>13) * (P>>13))>>25;
	var2 = (((int64_t)dig_P8) * P)>>19;
	P = ((P + var1 + var2)>>8) + (((int64_t)dig_P7)<<4);
	return (int32_t) P;
}
