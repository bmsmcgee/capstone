 #ifndef DRIVER_SHT4X_INTERFACE_H
 #define DRIVER_SHT4X_INTERFACE_H
 
 #include "driver_sht4x.h"
 
 #ifdef __cplusplus
 extern "C"{
 #endif
 
 /**
  * @defgroup sht4x_interface_driver sht4x interface driver function
  * @brief    sht4x interface driver modules
  * @ingroup  sht4x_driver
  * @{
  */
 
 /**
  * @brief  interface iic bus init
  * @return status code
  *         - 0 success
  *         - 1 iic init failed
  * @note   none
  */
 uint8_t sht4x_interface_iic_init(void);
 
 /**
  * @brief  interface iic bus deinit
  * @return status code
  *         - 0 success
  *         - 1 iic deinit failed
  * @note   none
  */
 uint8_t sht4x_interface_iic_deinit(void);
 
 /**
  * @brief     interface iic bus write command
  * @param[in] addr iic device write address
  * @param[in] *buf pointer to a data buffer
  * @param[in] len length of the data buffer
  * @return    status code
  *            - 0 success
  *            - 1 write failed
  * @note      none
  */
 uint8_t sht4x_interface_iic_write_cmd(uint8_t addr, uint8_t *buf, uint16_t len);
 
 /**
  * @brief      interface iic bus read command
  * @param[in]  addr iic device write address
  * @param[out] *buf pointer to a data buffer
  * @param[in]  len length of the data buffer
  * @return     status code
  *             - 0 success
  *             - 1 read failed
  * @note       none
  */
 uint8_t sht4x_interface_iic_read_cmd(uint8_t addr, uint8_t *buf, uint16_t len);
 
 /**
  * @brief     interface delay ms
  * @param[in] ms time
  * @note      none
  */
 void sht4x_interface_delay_ms(uint32_t ms);
 
 /**
  * @brief     interface print format data
  * @param[in] fmt format data
  * @note      none
  */
 void sht4x_interface_debug_print(const char *const fmt, ...);
 
 /**
  * @}
  */
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif