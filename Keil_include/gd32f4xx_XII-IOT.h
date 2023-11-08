#ifndef GD32F4XX_XII_IOT_H
#define GD32F4XX_XII_IOT_H

#ifdef cplusplus
 extern "C" {
#endif

#include "gd32f4xx.h"
#include <stdio.h>     
/* exported types */
typedef enum 
{
	LED1 = 0,
	LED2 = 1,
	LED3 = 2,
	LED4 = 3,
	LED5 = 4,
	LED6 = 5,
	LED7 = 6,
	LED8 = 7
} led_typedef_enum;

typedef enum 
{
	USER1_KEY = 0,
	USER2_KEY = 1,
	USER3_KEY = 2,
	USER4_KEY = 3,
	USER5_KEY = 4,
	USER6_KEY = 5,
	USER7_KEY = 6,
	USER8_KEY = 7
} key_typedef_enum;


typedef enum 
{
	DIP1 = 0,
	DIP2 = 1,
	DIP3 = 2,
	DIP4 = 3,
	DIP5 = 4,
	DIP6 = 5,
	DIP7 = 6,
	DIP8 = 7,
} dip_typedef_enum;


//#ifndef ON
//#define ON 1
//#endif
//#ifndef OFF
//#define OFF 0
//#endif

#define relay(n)   	n?gpio_bit_set(GPIOD,GPIO_PIN_3):gpio_bit_reset(GPIOD,GPIO_PIN_3);
#define buzzer(n)		n?gpio_bit_set(GPIOD,GPIO_PIN_12):gpio_bit_reset(GPIOD,GPIO_PIN_12);

typedef enum 
{
	KEY_MODE_GPIO = 0,
	KEY_MODE_EXTI = 1
} keymode_typedef_enum;

/* eval board low layer led */
#define LEDn                             8U

#define LED1_PIN                         GPIO_PIN_6
#define LED1_GPIO_PORT                   GPIOF
#define LED1_GPIO_CLK                    RCU_GPIOF
  
#define LED2_PIN                         GPIO_PIN_7
#define LED2_GPIO_PORT                   GPIOF
#define LED2_GPIO_CLK                    RCU_GPIOF
  
#define LED3_PIN                         GPIO_PIN_8
#define LED3_GPIO_PORT                   GPIOF
#define LED3_GPIO_CLK                    RCU_GPIOF
  
#define LED4_PIN                         GPIO_PIN_9
#define LED4_GPIO_PORT                   GPIOF
#define LED4_GPIO_CLK                    RCU_GPIOF

#define LED5_PIN                         GPIO_PIN_10
#define LED5_GPIO_PORT                   GPIOF
#define LED5_GPIO_CLK                    RCU_GPIOF

#define LED6_PIN                         GPIO_PIN_12
#define LED6_GPIO_PORT                   GPIOG
#define LED6_GPIO_CLK                    RCU_GPIOG

#define LED7_PIN                         GPIO_PIN_13
#define LED7_GPIO_PORT                   GPIOG
#define LED7_GPIO_CLK                    RCU_GPIOG

#define LED8_PIN                         GPIO_PIN_14
#define LED8_GPIO_PORT                   GPIOG
#define LED8_GPIO_CLK                    RCU_GPIOG


#define COMn                             6U
#define EVAL_COM0                        USART0
#define EVAL_COM0_CLK                    RCU_USART0
#define EVAL_COM0_TX_PIN                 GPIO_PIN_9
#define EVAL_COM0_RX_PIN                 GPIO_PIN_10
#define EVAL_COM0_GPIO_PORT              GPIOA
#define EVAL_COM0_GPIO_CLK               RCU_GPIOA
#define EVAL_COM0_AF                     GPIO_AF_7

#define EVAL_COM1                        USART1
#define EVAL_COM1_CLK                    RCU_USART1
#define EVAL_COM1_TX_PIN                 GPIO_PIN_2
#define EVAL_COM1_RX_PIN                 GPIO_PIN_3
#define EVAL_COM1_GPIO_PORT              GPIOA
#define EVAL_COM1_GPIO_CLK               RCU_GPIOA
#define EVAL_COM1_AF                     GPIO_AF_7

#define EVAL_COM2                        USART2
#define EVAL_COM2_CLK                    RCU_USART2
#define EVAL_COM2_TX_PIN                 GPIO_PIN_8
#define EVAL_COM2_RX_PIN                 GPIO_PIN_9
#define EVAL_COM2_GPIO_PORT              GPIOD
#define EVAL_COM2_GPIO_CLK               RCU_GPIOD
#define EVAL_COM2_AF                     GPIO_AF_7

#define EVAL_COM3                        UART3
#define EVAL_COM3_CLK                    RCU_UART3
#define EVAL_COM3_TX_PIN                 GPIO_PIN_10
#define EVAL_COM3_RX_PIN                 GPIO_PIN_11
#define EVAL_COM3_GPIO_PORT              GPIOC
#define EVAL_COM3_GPIO_CLK               RCU_GPIOC

#define EVAL_COM4                        UART4
#define EVAL_COM4_CLK                    RCU_UART4
#define EVAL_COM4_TX_PIN                 GPIO_PIN_12
#define EVAL_COM4_RX_PIN                 GPIO_PIN_11
#define EVAL_COM4_GPIO_PORT              GPIOC
#define EVAL_COM4_GPIO_CLK               RCU_GPIOC

#define EVAL_COM5                        USART5
#define EVAL_COM5_CLK                    RCU_USART5
#define EVAL_COM5_TX_PIN                 GPIO_PIN_11
#define EVAL_COM5_RX_PIN                 GPIO_PIN_12
#define EVAL_COM5_GPIO_PORT              GPIOA
#define EVAL_COM5_GPIO_CLK               RCU_GPIOA
#define EVAL_COM5_AF                     GPIO_AF_8

#define KEYn                             4U
/* K1 */
#define USER1_KEY_PIN                   	GPIO_PIN_2
#define USER1_KEY_GPIO_PORT             	GPIOE
#define USER1_KEY_GPIO_CLK              	RCU_GPIOE
#define USER1_KEY_EXTI_LINE             	EXTI_2
#define USER1_KEY_EXTI_PORT_SOURCE      	EXTI_SOURCE_GPIOE
#define USER1_KEY_EXTI_PIN_SOURCE       	EXTI_SOURCE_PIN2
#define USER1_KEY_EXTI_IRQn             	EXTI2_IRQn

/* K2 */
#define USER2_KEY_PIN                   	GPIO_PIN_3
#define USER2_KEY_GPIO_PORT             	GPIOE
#define USER2_KEY_GPIO_CLK              	RCU_GPIOE
#define USER2_KEY_EXTI_LINE             	EXTI_3
#define USER2_KEY_EXTI_PORT_SOURCE      	EXTI_SOURCE_GPIOE
#define USER2_KEY_EXTI_PIN_SOURCE       	EXTI_SOURCE_PIN3
#define USER2_KEY_EXTI_IRQn             	EXTI3_IRQn  

/* K3 */
#define USER3_KEY_PIN                     GPIO_PIN_4
#define USER3_KEY_GPIO_PORT               GPIOE
#define USER3_KEY_GPIO_CLK                RCU_GPIOE
#define USER3_KEY_EXTI_LINE               EXTI_4
#define USER3_KEY_EXTI_PORT_SOURCE        EXTI_SOURCE_GPIOE
#define USER3_KEY_EXTI_PIN_SOURCE         EXTI_SOURCE_PIN4
#define USER3_KEY_EXTI_IRQn               EXTI4_IRQn

/* K4 */
#define USER4_KEY_PIN                     GPIO_PIN_5
#define USER4_KEY_GPIO_PORT               GPIOE
#define USER4_KEY_GPIO_CLK                RCU_GPIOE
#define USER4_KEY_EXTI_LINE               EXTI_5
#define USER4_KEY_EXTI_PORT_SOURCE        EXTI_SOURCE_GPIOE
#define USER4_KEY_EXTI_PIN_SOURCE         EXTI_SOURCE_PIN5
#define USER4_KEY_EXTI_IRQn               EXTI5_9_IRQn

#define DIPn                            8U
/* user DIP-switch */
#define DIP1_PIN                     		GPIO_PIN_8
#define DIP1_GPIO_PORT               		GPIOE
#define DIP1_GPIO_CLK                		RCU_GPIOE

#define DIP2_PIN                     		GPIO_PIN_9
#define DIP2_GPIO_PORT               		GPIOE
#define DIP2_GPIO_CLK                		RCU_GPIOE

#define DIP3_PIN                     		GPIO_PIN_10
#define DIP3_GPIO_PORT               		GPIOE
#define DIP3_GPIO_CLK                		RCU_GPIOE

#define DIP4_PIN                     		GPIO_PIN_11
#define DIP4_GPIO_PORT               		GPIOE
#define DIP4_GPIO_CLK                		RCU_GPIOE

#define DIP5_PIN                     		GPIO_PIN_12
#define DIP5_GPIO_PORT               		GPIOE
#define DIP5_GPIO_CLK                		RCU_GPIOE

#define DIP6_PIN                     		GPIO_PIN_13
#define DIP6_GPIO_PORT               		GPIOE
#define DIP6_GPIO_CLK                		RCU_GPIOE

#define DIP7_PIN                     		GPIO_PIN_14
#define DIP7_GPIO_PORT               		GPIOE
#define DIP7_GPIO_CLK                		RCU_GPIOE

#define DIP8_PIN                     		GPIO_PIN_15
#define DIP8_GPIO_PORT               		GPIOE
#define DIP8_GPIO_CLK                		RCU_GPIOE

/* function declarations */
/* configures led GPIO */
void gd_eval_led_init(led_typedef_enum lednum);
/* turn on selected led */
void gd_eval_led_on(led_typedef_enum lednum);
/* turn off selected led */
void gd_eval_led_off(led_typedef_enum lednum);
/* toggle the selected led */
void gd_eval_led_toggle(led_typedef_enum lednum);
/* configure key */
void gd_eval_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode);
void gd_eval_dip_init(dip_typedef_enum dip_num);
/* return the selected button state */
uint8_t gd_eval_key_state_get(key_typedef_enum button);
uint8_t gd_eval_dip_state_get(dip_typedef_enum button);
/* configure COM port */
void gd_eval_com_init(uint32_t com,uint32_t baud);
void usart0Init(uint32_t com,uint32_t baud);
void usart1Init(uint32_t com,uint32_t baud);
void uart3Init(uint32_t com,uint32_t baud);
void usart5Init(uint32_t com,uint32_t baud);
/* configure LED port */
void gd_XII_systeminit(void);
void dipinit(void);
void gd_gpio_init(void);
int fputc(int ch, FILE *f);
void Uart0WriteByte(char c);
void Uart0Printf(char* fmt, ...);
void Uart1WriteByte(char c);
void Uart1Printf(char* fmt, ...);

#ifdef cplusplus
}
#endif

#endif /* GD32F4XX_XII_IOT */

