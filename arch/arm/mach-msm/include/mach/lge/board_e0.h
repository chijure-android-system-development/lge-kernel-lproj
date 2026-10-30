/* Board definitions for the LG Optimus L3 (E400, "e0").
 * Included from board_v3.h for CONFIG_MACH_MSM7X25A_E0; these take
 * precedence over the V3 defaults. Values from the 2.6.38 board-e0eu.h. */
#ifndef __ASM_ARCH_MSM_BOARD_E0_H
#define __ASM_ARCH_MSM_BOARD_E0_H

/* sensor bus on gpio 48/49 */
#define SENSOR_GPIO_I2C_SCL		49
#define SENSOR_GPIO_I2C_SDA		48

/* motion interrupt 2 */
#define ECOM_GPIO_INT			35

#endif
