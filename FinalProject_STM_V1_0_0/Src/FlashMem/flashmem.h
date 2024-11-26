/*
 * customBitmap.h
 *
 *  Created on: Oct 19, 2024
 *      Author: Admin
 */
/*
 * flashmem.c
 *
 *  Created on: Nov 25, 2024
 *      Author: Admin
 */


#ifndef INC_FLASH_STORE_DATA_H_
#define INC_FLASH_STORE_DATA_H_

#include "stdio.h"
#include "stm32f4xx_flash.h"
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_4    /* Start address of user Flash area */

#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_11  /* End address of user Flash area */

/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base address of Sector 0, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base address of Sector 1, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base address of Sector 2, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base address of Sector 3, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base address of Sector 4, 64 Kbytes   */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base address of Sector 5, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base address of Sector 6, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base address of Sector 7, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base address of Sector 8, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base address of Sector 9, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base address of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base address of Sector 11, 128 Kbytes */
#define DATA_32                 ((uint32_t)0x12345678)

#endif /* INC_FLASH_STORE_DATA_H_ */



FLASH_Status flashWrite(uint32_t ID, uint32_t data);
uint32_t flashReadByte(uint32_t ID);
void flashConfig();

