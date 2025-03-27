 #ifndef DRIVER_SHT4X_H
 #define DRIVER_SHT4X_H

 #include "drivers/i2c_driver.h"
 #include <stdio.h>
 #include <stdint.h>
 #include <string.h>
 #include <unistd.h>
 #include <errno.h>
 #include <fcntl.h>
 
 #ifdef __cplusplus
 extern "C"{
 #endif
 

 typedef enum
 {
     SHT4X_ADDRESS_0 = 0x44,        /**< address0 */
     SHT4X_ADDRESS_1 = 0x45,        /**< address1 */
     SHT4X_ADDRESS_2 = 0x46,        /**< address2 */
 } sht4x_address_t;
 

 typedef enum {
    SHT4X_MODE_HIGH_PRECISION_WITH_NO_HEATER   = 0xFD,
    SHT4X_MODE_MEDIUM_PRECISION_WITH_NO_HEATER = 0xF6,
    SHT4X_MODE_LOWEST_PRECISION_WITH_NO_HEATER = 0xE0,
} sht4x_mode_t;
 
 
//  typedef struct sht4x_handle_s {
//     int fd;                       
//     i2c_device_t device;              
//     uint8_t inited;                   
// } sht4x_handle_t;
 

 typedef struct sht4x_info_s {
     char chip_name[32];                
     char manufacturer_name[32];        
     char interface[8];                 
     float supply_voltage_min_v;        
     float supply_voltage_max_v;      
     float max_current_ma;             
     float temperature_min;
     float temperature_max;             
     uint32_t driver_version;           
 } sht4x_info_t;
 
 uint8_t sht4x_info(sht4x_info_t *info);
 

//  uint8_t sht4x_set_addr(sht4x_handle_t *handle, sht4x_address_t addr);
 

//  uint8_t sht4x_get_addr(sht4x_handle_t *handle, sht4x_address_t *addr);

void sht45_init(void);

uint8_t sht4x_init(void);

//  uint8_t sht4x_deinit(sht4x_handle_t *handle);
 

//  uint8_t sht4x_read(sht4x_handle_t *handle, sht4x_mode_t mode, 
//                     uint16_t *temperature_raw, float *temperature_s, 
//                     uint16_t *humidity_raw, float *humidity_s);

//  uint8_t sht4x_soft_reset(sht4x_handle_t *handle);
 

//  uint8_t sht4x_get_serial_number(sht4x_handle_t *handle, uint8_t num[4]);
 


//  uint8_t sht4x_write_read(sht4x_handle_t *handle, uint8_t cmd, uint16_t delay_ms, uint8_t *data, uint16_t len);
 

 
 #ifdef __cplusplus
 }
 #endif
 
 #endif