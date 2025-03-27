 #include "driver_sht4x.h"
 #include "drivers/i2c_driver.h"
 #include "sys/ioctl.h"

 
 #define CHIP_NAME                 "Sensirion SHT4X"       
 #define MANUFACTURER_NAME         "Sensirion"           
 #define SUPPLY_VOLTAGE_MIN        1.08f                  
 #define SUPPLY_VOLTAGE_MAX        3.6f                   
 #define MAX_CURRENT               100.0f                
 #define TEMPERATURE_MIN           -40.0f                  
 #define TEMPERATURE_MAX           125.0f                   
 #define DRIVER_VERSION            1000                     
 

 
 #define SHT4X_COMMAND_SOFT_RESET            0x94      
 #define SHT4X_COMMAND_READ_SERIAL_NUMBER    0x89       

//  static sht4x_handle_t sht4x_handle = {
//     .fd = -1,
//     .device = {
//         .port = I2C_PORT_1,
//         .address = SHT4X_ADDRESS_0
//     },
//     .inited = 0
//  };

static i2c_device_t sht4x_device = {
    .port = I2C_PORT_1,
    .address = SHT4X_ADDRESS_0
};

int fd = -1;
 

 static uint8_t a_sht4x_write_read(uint8_t cmd, uint16_t delay, uint8_t *data, uint16_t len)
 {
    long ret;


     i2c_transfer_t transfer;

     transfer.addr.len = 1;
     transfer.addr.data = &cmd;
     transfer.value.len = 1;
     transfer.value.data = &cmd;

     ret = ioctl(fd, I2C_WRITE_REG, &transfer);
     if(ret < 0){
        return -1;
     }

     if (delay > 0){
        usleep(delay * 1000);
     }

     if (len > 0) {
        transfer.value.len = len;
        transfer.value.data = data;

        ret = ioctl(fd, I2C_READ_REG, &transfer);
        if(ret < 0) {
            return -1;
        }
     }

     return 0;
 }
 

//  static uint8_t a_sht4x_crc(uint8_t *data, uint16_t len)
//  {
//      const uint8_t POLYNOMIAL = 0x31;
//      uint8_t crc = 0xFF;
//      uint16_t i;
//      uint16_t j;
   
//      for (j = len; j != 0; --j)                                              /* length-- */
//      {
//          crc ^= *data++;                                                     /* xor */
//          for (i = 8; i != 0; --i)                                            /* 8 times */
//          {
//              crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc<<1);        /* calculate crc */
//          }
//      }
   
//      return crc;                                                             /* return crc */
//  }
 
void sht45_init(void){
    if(sht4x_init() < 0) {
        printf("sht45 not initialized");
        return;
    }
}

 uint8_t sht4x_init(void)
 {

    driver_add("/dev/i2c0/sht45", &sht4x_device, &i2c_device_devops);

    fd = open("/dev/i2c0/sht45", 0, 0);
    if (fd < 0) {
        return -1;
    }

    if (a_sht4x_write_read(SHT4X_COMMAND_SOFT_RESET, 10, NULL, 0) < 0) {
        close(fd);
        fd = -1;
        return -1;
    }


    return 0;
 }
 

//  uint8_t sht4x_deinit(sht4x_handle_t *handle)
//  {
//      uint8_t res;
     
//      if (handle == NULL)                                               /* check handle */
//      {
//          return 2;                                                     /* return error */
//      }
//      if (handle->inited != 1)                                          /* check handle initialization */
//      {
//          return 3;                                                     /* return error */
//      }
     
//      res = a_sht4x_write_read(handle, SHT4X_COMMAND_SOFT_RESET, 
//                               10, NULL, 0);                            /* soft reset */
//      if (res != 0)                                                     /* check result */
//      {
//          handle->debug_print("sht4x: write command failed.\n");        /* write command failed */
         
//          return 4;                                                     /* return error */
//      }
//      if (handle->iic_deinit() != 0)                                    /* iic deinit */
//      {
//          handle->debug_print("sht4x: iic deinit failed.\n");           /* iic deinit failed */
        
//          return 1;                                                     /* return error */
//      }
//      handle->inited = 0;                                               /* flag close */
     
//      return 0;                                                         /* success return 0 */
//  }
 

//  uint8_t sht4x_set_addr(sht4x_handle_t *handle, sht4x_address_t addr)
//  {
//      if (handle == NULL)                      /* check handle */
//      {
//          return 2;                            /* return error */
//      }
     
//      handle->iic_addr = (uint8_t)addr;        /* set address */
     
//      return 0;                                /* success return 0 */
//  }
 

//  uint8_t sht4x_get_addr(sht4x_handle_t *handle, sht4x_address_t *addr)
//  {
//      if (handle == NULL)                                 /* check handle */
//      {
//          return 2;                                       /* return error */
//      }
     
//      *addr = (sht4x_address_t)(handle->iic_addr);        /* get address */
     
//      return 0;                                           /* success return 0 */
//  }
 

//  uint8_t sht4x_read(sht4x_handle_t *handle, sht4x_mode_t mode, 
//                     uint16_t *temperature_raw, float *temperature_s, 
//                     uint16_t *humidity_raw, float *humidity_s)
//  {
//      uint8_t res;
//      uint8_t buf[6];
     
//      if (handle == NULL)                                                                       /* check handle */
//      {
//          return 2;                                                                             /* return error */
//      }
//      if (handle->inited != 1)                                                                  /* check handle initialization */
//      {
//          return 3;                                                                             /* return error */
//      }
     
//      switch (mode)                                                                             /* select mode */
//      {
//          case SHT4X_MODE_HIGH_PRECISION_WITH_NO_HEATER :                                       /* measure T & RH with high precision */
//          {
//              res = a_sht4x_write_read(handle,
//                                       SHT4X_MODE_HIGH_PRECISION_WITH_NO_HEATER, 
//                                       10, buf, 6);                                             /* read data */
//              if (res != 0)                                                                     /* check result */
//              {
//                  handle->debug_print("sht4x: write command failed.\n");                        /* write command failed */
                 
//                  return 1;                                                                     /* return error */
//              }
             
//              break;                                                                            /* break */
//          }
//          case SHT4X_MODE_MEDIUM_PRECISION_WITH_NO_HEATER :                                     /* measure T & RH with medium precision */
//          {
//              res = a_sht4x_write_read(handle,
//                                       SHT4X_MODE_MEDIUM_PRECISION_WITH_NO_HEATER, 
//                                       5, buf, 6);                                              /* read data */
//              if (res != 0)                                                                     /* check result */
//              {
//                  handle->debug_print("sht4x: write command failed.\n");                        /* write command failed */
                 
//                  return 1;                                                                     /* return error */
//              }
             
//              break;                                                                            /* break */
//          }
//          case SHT4X_MODE_LOWEST_PRECISION_WITH_NO_HEATER :                                     /* measure T & RH with the lowest precision */
//          {
//              res = a_sht4x_write_read(handle,
//                                       SHT4X_MODE_LOWEST_PRECISION_WITH_NO_HEATER, 
//                                       2, buf, 6);                                              /* read data */
//              if (res != 0)                                                                     /* check result */
//              {
//                  handle->debug_print("sht4x: write command failed.\n");                        /* write command failed */
                 
//                  return 1;                                                                     /* return error */
//              }
             
//              break;                                                                            /* break */
//          }
//          case SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_200MW_1S :                                 /* activate heater with 200mW for 1s, high precision */
//          {
//              res = a_sht4x_write_read(handle,
//                                       SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_200MW_1S, 
//                                       1100, buf, 6);                                           /* read data */
//              if (res != 0)                                                                     /* check result */
//              {
//                  handle->debug_print("sht4x: write command failed.\n");                        /* write command failed */
                 
//                  return 1;                                                                     /* return error */
//              }
             
//              break;                                                                            /* break */
//          }
//          case SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_200MW_0P1S :                               /* activate heater with 200mW for 0.1s, high precision */
//          {
//              res = a_sht4x_write_read(handle,
//                                       SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_200MW_0P1S, 
//                                       110, buf, 6);                                            /* read data */
//              if (res != 0)                                                                     /* check result */
//              {
//                  handle->debug_print("sht4x: write command failed.\n");                        /* write command failed */
                 
//                  return 1;                                                                     /* return error */
//              }
             
//              break;                                                                            /* break */
//          }
//          case SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_110MW_1S :                                 /* activate heater with 110mW for 1s, high precision */
//          {
//              res = a_sht4x_write_read(handle,
//                                       SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_110MW_1S, 
//                                       1100, buf, 6);                                           /* read data */
//              if (res != 0)                                                                     /* check result */
//              {
//                  handle->debug_print("sht4x: write command failed.\n");                        /* write command failed */
                 
//                  return 1;                                                                     /* return error */
//              }
             
//              break;                                                                            /* break */
//          }
//          case SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_110MW_0P1S :                               /* activate heater with 110mW for 0.1s, high precision */
//          {
//              res = a_sht4x_write_read(handle,
//                                       SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_110MW_0P1S, 
//                                       110, buf, 6);                                            /* read data */
//              if (res != 0)                                                                     /* check result */
//              {
//                  handle->debug_print("sht4x: write command failed.\n");                        /* write command failed */
                 
//                  return 1;                                                                     /* return error */
//              }
             
//              break;                                                                            /* break */
//          }
//          case SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_20MW_1S :                                  /* activate heater with 20mW for 1s, high precision */
//          {
//              res = a_sht4x_write_read(handle,
//                                       SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_20MW_1S, 
//                                       1100, buf, 6);                                           /* read data */
//              if (res != 0)                                                                     /* check result */
//              {
//                  handle->debug_print("sht4x: write command failed.\n");                        /* write command failed */
                 
//                  return 1;                                                                     /* return error */
//              }
             
//              break;                                                                            /* break */
//          }
//          case SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_20MW_0P1S :                                /* activate heater with 20mW for 0.1s, high precision */
//          {
//              res = a_sht4x_write_read(handle,
//                                       SHT4X_MODE_HIGH_PRECISION_WITH_HEATER_20MW_0P1S, 
//                                       110, buf, 6);                                            /* read data */
//              if (res != 0)                                                                     /* check result */
//              {
//                  handle->debug_print("sht4x: write command failed.\n");                        /* write command failed */
                 
//                  return 1;                                                                     /* return error */
//              }
             
//              break;                                                                            /* break */
//          }
//          default :                                                                             /* invalid */
//          {
//              memset(buf, 0, sizeof(uint8_t) * 6);                                              /* clear buffer */
             
//              break;                                                                            /* break */
//          }
//      }
//      if (a_sht4x_crc(buf + 0, 2) != buf[2])                                                    /* check crc */
//      {
//          handle->debug_print("sht4x: crc is error.\n");                                        /* crc is error */
         
//          return 4;                                                                             /* return error */
//      }
//      if (a_sht4x_crc(buf + 3, 2) != buf[5])                                                    /* check crc */
//      {
//          handle->debug_print("sht4x: crc is error.\n");                                        /* crc is error */
         
//          return 4;                                                                             /* return error */
//      }
//      *temperature_raw = (uint16_t)((((uint16_t)buf[0]) << 8) | buf[1]);                        /* get raw temperature */
//      *humidity_raw = (uint16_t)((((uint16_t)buf[3]) << 8) | buf[4]);                           /* get raw humidity */
//      *temperature_s = (float)(*temperature_raw) / 65535.0f * 175.0f - 45.0f;                   /* convert raw temperature */
//      *humidity_s = (((float)(*humidity_raw) / 65535.0f) * 125.0f - 6.0f);                      /* convert raw humidity */
//      if ((*humidity_s) > 100.0f)                                                               /* check humidity range */
//      {
//          *humidity_s = 100.0f;                                                                 /* max humidity is 100% */
//      }
//      if ((*humidity_s) < 0.0f)                                                                 /* check humidity range */
//      {
//          *humidity_s = 0.0f;                                                                   /* min humidity is 0% */
//      }
     
//      return 0;                                                                                 /* success return 0 */
//  }
 

//  uint8_t sht4x_get_serial_number(sht4x_handle_t *handle, uint8_t num[4])
//  {
//      uint8_t res;
//      uint8_t buf[6];
     
//      if (handle == NULL)                                                       /* check handle */
//      {
//          return 2;                                                             /* return error */
//      }
//      if (handle->inited != 1)                                                  /* check handle initialization */
//      {
//          return 3;                                                             /* return error */
//      }
     
//      res = a_sht4x_write_read(handle, SHT4X_COMMAND_READ_SERIAL_NUMBER, 
//                               10, buf, 6);                                     /* read serial number */
//      if (res != 0)                                                             /* check result */
//      {
//          handle->debug_print("sht4x: write command failed.\n");                /* write command failed */
         
//          return 1;                                                             /* return error */
//      }
     
//      if (a_sht4x_crc(buf + 0, 2) != buf[2])                                    /* check crc */
//      {
//          handle->debug_print("sht4x: crc is error.\n");                        /* crc is error */
         
//          return 4;                                                             /* return error */
//      }
//      if (a_sht4x_crc(buf + 3, 2) != buf[5])                                    /* check crc */
//      {
//          handle->debug_print("sht4x: crc is error.\n");                        /* crc is error */
         
//          return 4;                                                             /* return error */
//      }
//      num[0] = buf[0];                                                          /* set number 0 */
//      num[1] = buf[1];                                                          /* set number 1 */
//      num[2] = buf[3];                                                          /* set number 2 */
//      num[3] = buf[4];                                                          /* set number 3 */
     
//      return 0;                                                                 /* success return 0 */
//  }
 

//  uint8_t sht4x_soft_reset(sht4x_handle_t *handle)
//  {
//      uint8_t res;
     
//      if (handle == NULL)                                               /* check handle */
//      {
//          return 2;                                                     /* return error */
//      }
//      if (handle->inited != 1)                                          /* check handle initialization */
//      {
//          return 3;                                                     /* return error */
//      }
     
//      res = a_sht4x_write_read(handle, SHT4X_COMMAND_SOFT_RESET, 
//                               10, NULL, 0);                            /* soft reset */
//      if (res != 0)                                                     /* check result */
//      {
//          handle->debug_print("sht4x: write command failed.\n");        /* write command failed */
         
//          return 1;                                                     /* return error */
//      }
     
//      return 0;                                                         /* success return 0 */
//  }
 

//  uint8_t sht4x_write_read(sht4x_handle_t *handle, uint8_t cmd, uint16_t delay_ms, uint8_t *data, uint16_t len)
//  {
//      if (handle == NULL)                                                /* check handle */
//      {
//          return 2;                                                      /* return error */
//      }
//      if (handle->inited != 1)                                           /* check handle initialization */
//      {
//          return 3;                                                      /* return error */
//      }
     
//      return a_sht4x_write_read(handle, cmd, delay_ms, data, len);       /* write and read */
//  }
 

 uint8_t sht4x_info(sht4x_info_t *info)
 {
     if (info == NULL)                                               /* check handle */
     {
         return 2;                                                   /* return error */
     }
     
     memset(info, 0, sizeof(sht4x_info_t));                          /* initialize sht4x info structure */
     strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
     strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
     strncpy(info->interface, "IIC", 8);                             /* copy interface name */
     info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
     info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
     info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
     info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
     info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
     info->driver_version = DRIVER_VERSION;                          /* set driver version */
     
     return 0;                                                       /* success return 0 */
 }