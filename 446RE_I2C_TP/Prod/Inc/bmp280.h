#include <stdint.h>

#define BMP280_ADDR 0x77
#define BMP280_RESET 0xB6

typedef enum bmp280_reg_enum
{
	temp_xlsb   = 0xFC,
	temp_lsb   	= 0xFB,
	temp_msb  	= 0xFA,
	press_xmsb	= 0xF9,
	press_lsb	= 0xF8,
	press_msb	= 0xF7,
	config		= 0xF5,
	ctrl_mes	= 0xF4,
	status		= 0xF3,
	reset		= 0xE0,
	id			= 0xD0,
	calib25		= 0xA1,
	calib00		= 0x88
} bmp280_reg_t;

typedef struct bmp280_struct
{
	bmp280_reg_t 	registre;
	uint8_t			buf[26];
	uint8_t			id;
	uint8_t			config;
	uint8_t			calib[25];
	int32_t			temp;
} h_bmp280_t;

void bmp_reset(h_bmp280_t * bmp280);
void bmp_id(h_bmp280_t * bmp280);
void bmp_config(h_bmp280_t * bmp280);
void bmp_calib_read(h_bmp280_t * bmp280);
void bmp_temp_read(h_bmp280_t * bmp280);
int32_t bmp_compensate_T(h_bmp280_t * bmp280);

