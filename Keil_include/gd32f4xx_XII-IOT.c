#include "gd32f4xx_XII-IOT.h"
#include "gd32f4xx_usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "stdarg.h"
#include <string.h>

static char buffer1[128];
static char buffer2[128];
/* LED��ʼ�� */
static uint32_t GPIO_PORT[LEDn] 	= { LED1_GPIO_PORT, 
	                                    LED2_GPIO_PORT, 
	                                    LED3_GPIO_PORT, 
	                                    LED4_GPIO_PORT,
																			LED5_GPIO_PORT, 
                                      LED6_GPIO_PORT, 
                                      LED7_GPIO_PORT, 
                                      LED8_GPIO_PORT };

static uint32_t GPIO_PIN[LEDn] 		= { LED1_PIN, LED2_PIN, 
																			LED3_PIN, LED4_PIN,
																			LED5_PIN,LED6_PIN,
                                      LED7_PIN,LED8_PIN };

/* ���ڳ�ʼ�� */																	
static rcu_periph_enum COM_CLK[COMn] 			= {EVAL_COM0_CLK,       EVAL_COM1_CLK,       EVAL_COM2_CLK,       EVAL_COM3_CLK,       EVAL_COM4_CLK,       EVAL_COM5_CLK};
static uint32_t COM_TX_PIN[COMn] 					= {EVAL_COM0_TX_PIN,    EVAL_COM1_TX_PIN,    EVAL_COM2_TX_PIN,    EVAL_COM3_TX_PIN,    EVAL_COM4_TX_PIN,    EVAL_COM5_TX_PIN};
static uint32_t COM_RX_PIN[COMn] 					= {EVAL_COM0_RX_PIN,    EVAL_COM1_RX_PIN,    EVAL_COM2_RX_PIN,    EVAL_COM3_RX_PIN,    EVAL_COM4_RX_PIN,    EVAL_COM5_RX_PIN};
static uint32_t COM_GPIO_PORT[COMn] 			= {EVAL_COM0_GPIO_PORT, EVAL_COM1_GPIO_PORT, EVAL_COM2_GPIO_PORT, EVAL_COM3_GPIO_PORT, EVAL_COM4_GPIO_PORT, EVAL_COM5_GPIO_PORT};
static rcu_periph_enum COM_GPIO_CLK[COMn] = {EVAL_COM0_GPIO_CLK,  EVAL_COM1_GPIO_CLK,  EVAL_COM2_GPIO_CLK,  EVAL_COM3_GPIO_CLK,  EVAL_COM4_GPIO_CLK,  EVAL_COM5_GPIO_CLK};

/* ������ʼ�� */
static rcu_periph_enum GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK, LED4_GPIO_CLK,
                                         LED5_GPIO_CLK, LED6_GPIO_CLK, LED7_GPIO_CLK, LED8_GPIO_CLK,};

static uint32_t KEY_PORT[KEYn] 				    = {USER1_KEY_GPIO_PORT, 
																				     USER2_KEY_GPIO_PORT,
																				     USER3_KEY_GPIO_PORT,
																				     USER4_KEY_GPIO_PORT };

static uint32_t KEY_PIN[KEYn]             = {USER1_KEY_PIN, USER2_KEY_PIN, USER3_KEY_PIN, USER4_KEY_PIN};

static rcu_periph_enum KEY_CLK[KEYn]      = {USER1_KEY_GPIO_CLK, 
                                             USER2_KEY_GPIO_CLK,
                                             USER3_KEY_GPIO_CLK,
																				     USER4_KEY_GPIO_CLK};

static exti_line_enum KEY_EXTI_LINE[KEYn] = {USER1_KEY_EXTI_LINE,
                                             USER2_KEY_EXTI_LINE,
                                             USER3_KEY_EXTI_LINE,
																						 USER4_KEY_EXTI_LINE};

static uint8_t KEY_PORT_SOURCE[KEYn]      = {USER1_KEY_EXTI_PORT_SOURCE,
                                             USER2_KEY_EXTI_PORT_SOURCE,
                                             USER3_KEY_EXTI_PORT_SOURCE,
																				     USER4_KEY_EXTI_PORT_SOURCE};

static uint8_t KEY_PIN_SOURCE[KEYn]       = {USER1_KEY_EXTI_PIN_SOURCE,
                                             USER2_KEY_EXTI_PIN_SOURCE,
                                             USER3_KEY_EXTI_PIN_SOURCE,
																			       USER4_KEY_EXTI_PIN_SOURCE};

static uint8_t KEY_IRQn[KEYn]             = {USER1_KEY_EXTI_IRQn, 
                                             USER2_KEY_EXTI_IRQn,
                                             USER3_KEY_EXTI_IRQn,
																             USER4_KEY_EXTI_IRQn};

/* ���뿪�س�ʼ�� */																 
static uint32_t DIP_PORT[DIPn]         = {DIP1_GPIO_PORT, DIP2_GPIO_PORT, 
																					DIP3_GPIO_PORT, DIP4_GPIO_PORT,
																					DIP5_GPIO_PORT, DIP6_GPIO_PORT,
																					DIP7_GPIO_PORT, DIP8_GPIO_PORT};	

static uint32_t DIP_PIN[DIPn] 				 = {DIP1_PIN, DIP2_PIN, DIP3_PIN, DIP4_PIN,
																					DIP5_PIN, DIP6_PIN, DIP7_PIN, DIP8_PIN };	

static rcu_periph_enum DIP_CLK[DIPn] 	 = {DIP1_GPIO_CLK, DIP2_GPIO_CLK, DIP3_GPIO_CLK, DIP4_GPIO_CLK,
                                          DIP5_GPIO_CLK, DIP6_GPIO_CLK, DIP7_GPIO_CLK, DIP8_GPIO_CLK,};			

//LED��ʼ������																 																 															 																												 
void  gd_eval_led_init (led_typedef_enum lednum) {
    /* enable the led clock */
    rcu_periph_clock_enable(GPIO_CLK[lednum]);
    /* configure led GPIO port */ 
    gpio_mode_set(GPIO_PORT[lednum], GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN[lednum]);
    gpio_output_options_set(GPIO_PORT[lednum], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN[lednum]);

    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

//����LED��
void gd_eval_led_on(led_typedef_enum lednum) {
    GPIO_BOP(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

//�ر�LED��
void gd_eval_led_off(led_typedef_enum lednum) {
    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}


void gd_eval_led_toggle(led_typedef_enum lednum) {
    GPIO_TG(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

//������ʼ��
void gd_eval_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode) {
    /* enable the key clock */
    rcu_periph_clock_enable(KEY_CLK[key_num]);
    rcu_periph_clock_enable(RCU_SYSCFG);

    /* configure button pin as input */
    gpio_mode_set(KEY_PORT[key_num], GPIO_MODE_INPUT, GPIO_PUPD_NONE,KEY_PIN[key_num]);

    if (key_mode == KEY_MODE_EXTI) {
        /* enable and set key EXTI interrupt to the lowest priority */
        nvic_irq_enable(KEY_IRQn[key_num], 2U, 0U);

        /* connect key EXTI line to key GPIO pin */
        syscfg_exti_line_config(KEY_PORT_SOURCE[key_num], KEY_PIN_SOURCE[key_num]);

        /* configure key EXTI line */
        exti_init(KEY_EXTI_LINE[key_num], EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(KEY_EXTI_LINE[key_num]);
    }
}

//���뿪�س�ʼ��
void gd_eval_dip_init(dip_typedef_enum dip_num) {
    /* enable the key clock */
    rcu_periph_clock_enable(DIP_CLK[dip_num]);
    rcu_periph_clock_enable(RCU_SYSCFG);

    /* configure dip button pin as input */
    gpio_mode_set(DIP_PORT[dip_num], GPIO_MODE_INPUT, GPIO_PUPD_NONE,DIP_PIN[dip_num]);
}

//����״̬��ȡ
uint8_t gd_eval_key_state_get(key_typedef_enum button) {
    return gpio_input_bit_get(KEY_PORT[button], KEY_PIN[button]);
}

//���뿪��״̬��ȡ
uint8_t gd_eval_dip_state_get(dip_typedef_enum button) {
    return gpio_input_bit_get(DIP_PORT[button], DIP_PIN[button]);
}

//���ڳ�ʼ��
void gd_eval_com_init(uint32_t com,uint32_t baud) {
	/* enable GPIO clock */
	uint32_t COM_ID;

	switch(com)
	{
		case EVAL_COM0 : COM_ID = 0U;
			break;
		case EVAL_COM1 : COM_ID = 1U;
			break;
		case EVAL_COM2 : COM_ID = 2U;
			break;	
		case EVAL_COM3 : COM_ID = 3U;
			break;
		case EVAL_COM4 : COM_ID = 4U;
			break;	
		case EVAL_COM5 : COM_ID = 5U;
			break;	
	}
		
	rcu_periph_clock_enable( COM_GPIO_CLK[COM_ID]);

	/* enable USART clock */
	rcu_periph_clock_enable(COM_CLK[COM_ID]);

	/* connect port to USARTx_Tx */
	gpio_af_set(COM_GPIO_PORT[COM_ID], EVAL_COM1_AF, COM_TX_PIN[COM_ID]);

	/* connect port to USARTx_Rx */
	gpio_af_set(COM_GPIO_PORT[COM_ID], EVAL_COM1_AF, COM_RX_PIN[COM_ID]);

	/* configure USART Tx as alternate function push-pull */
	gpio_mode_set(COM_GPIO_PORT[COM_ID], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_TX_PIN[COM_ID]);
	gpio_output_options_set(COM_GPIO_PORT[COM_ID], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_TX_PIN[COM_ID]);

	/* configure USART Rx as alternate function push-pull */
	gpio_mode_set(COM_GPIO_PORT[COM_ID], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_RX_PIN[COM_ID]);
	gpio_output_options_set(COM_GPIO_PORT[COM_ID], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_RX_PIN[COM_ID]);

	/* USART configure */
	usart_deinit(com);
	usart_baudrate_set(com,baud);
	usart_receive_config(com, USART_RECEIVE_ENABLE);
	usart_transmit_config(com, USART_TRANSMIT_ENABLE);
	usart_enable(com);
}

void usart0Init(uint32_t com, uint32_t baud)
{
	/* enable GPIO clock */
	uint32_t COM_ID = 0U;
		
	rcu_periph_clock_enable( COM_GPIO_CLK[COM_ID]);

	/* enable USART clock */
	rcu_periph_clock_enable(COM_CLK[COM_ID]);

	/* connect port to USARTx_Tx */
	gpio_af_set(COM_GPIO_PORT[COM_ID], GPIO_AF_7, COM_TX_PIN[COM_ID]);

	/* connect port to USARTx_Rx */
	gpio_af_set(COM_GPIO_PORT[COM_ID], GPIO_AF_7, COM_RX_PIN[COM_ID]);

	/* configure USART Tx as alternate function push-pull */
	gpio_mode_set(COM_GPIO_PORT[COM_ID], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_TX_PIN[COM_ID]);
	gpio_output_options_set(COM_GPIO_PORT[COM_ID], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_TX_PIN[COM_ID]);

	/* configure USART Rx as alternate function push-pull */
	gpio_mode_set(COM_GPIO_PORT[COM_ID], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_RX_PIN[COM_ID]);
	gpio_output_options_set(COM_GPIO_PORT[COM_ID], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_RX_PIN[COM_ID]);

	/* USART configure */
	usart_deinit(com);
	usart_baudrate_set(com,baud);
	usart_receive_config(com, USART_RECEIVE_ENABLE);
	usart_transmit_config(com, USART_TRANSMIT_ENABLE);
	usart_enable(com);
}

void usart1Init(uint32_t com, uint32_t baud)
{
	/* enable GPIO clock */
	uint32_t COM_ID = 1U;
		
	rcu_periph_clock_enable( COM_GPIO_CLK[COM_ID]);

	/* enable USART clock */
	rcu_periph_clock_enable(COM_CLK[COM_ID]);

	/* connect port to USARTx_Tx */
	gpio_af_set(COM_GPIO_PORT[COM_ID], EVAL_COM1_AF, COM_TX_PIN[COM_ID]);

	/* connect port to USARTx_Rx */
	gpio_af_set(COM_GPIO_PORT[COM_ID], EVAL_COM1_AF, COM_RX_PIN[COM_ID]);

	/* configure USART Tx as alternate function push-pull */
	gpio_mode_set(COM_GPIO_PORT[COM_ID], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_TX_PIN[COM_ID]);
	gpio_output_options_set(COM_GPIO_PORT[COM_ID], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_TX_PIN[COM_ID]);

	/* configure USART Rx as alternate function push-pull */
	gpio_mode_set(COM_GPIO_PORT[COM_ID], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_RX_PIN[COM_ID]);
	gpio_output_options_set(COM_GPIO_PORT[COM_ID], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_RX_PIN[COM_ID]);

	/* USART configure */
	usart_deinit(com);
	usart_baudrate_set(com,baud);
	usart_receive_config(com, USART_RECEIVE_ENABLE);
	usart_transmit_config(com, USART_TRANSMIT_ENABLE);
	usart_enable(com);
}

void uart3Init(uint32_t com,uint32_t baud) {
		/* enable GPIO clock */
		uint32_t COM_ID = 3U;
			
		rcu_periph_clock_enable( COM_GPIO_CLK[COM_ID]);

		/* enable USART clock */
		rcu_periph_clock_enable(COM_CLK[COM_ID]);

		/* connect port to USARTx_Tx */
		gpio_af_set(COM_GPIO_PORT[COM_ID], GPIO_AF_8, COM_TX_PIN[COM_ID]);

		/* connect port to USARTx_Rx */
		gpio_af_set(COM_GPIO_PORT[COM_ID], GPIO_AF_8, COM_RX_PIN[COM_ID]);

		/* configure USART Tx as alternate function push-pull */
		gpio_mode_set(COM_GPIO_PORT[COM_ID], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_TX_PIN[COM_ID]);
		gpio_output_options_set(COM_GPIO_PORT[COM_ID], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_TX_PIN[COM_ID]);

		/* configure USART Rx as alternate function push-pull */
		gpio_mode_set(COM_GPIO_PORT[COM_ID], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_RX_PIN[COM_ID]);
		gpio_output_options_set(COM_GPIO_PORT[COM_ID], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_RX_PIN[COM_ID]);

		/* USART configure */
		usart_deinit(com);
		usart_baudrate_set(com,baud);
		usart_receive_config(com, USART_RECEIVE_ENABLE);
		usart_transmit_config(com, USART_TRANSMIT_ENABLE);
		usart_enable(com);
}

void usart5Init(uint32_t com,uint32_t baud) {
		/* enable GPIO clock */
		uint32_t COM_ID = 5U;
			
		rcu_periph_clock_enable( COM_GPIO_CLK[COM_ID]);

		/* enable USART clock */
		rcu_periph_clock_enable(COM_CLK[COM_ID]);

		/* connect port to USARTx_Tx */
		gpio_af_set(COM_GPIO_PORT[COM_ID], GPIO_AF_8, COM_TX_PIN[COM_ID]);

		/* connect port to USARTx_Rx */
		gpio_af_set(COM_GPIO_PORT[COM_ID], GPIO_AF_8, COM_RX_PIN[COM_ID]);

		/* configure USART Tx as alternate function push-pull */
		gpio_mode_set(COM_GPIO_PORT[COM_ID], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_TX_PIN[COM_ID]);
		gpio_output_options_set(COM_GPIO_PORT[COM_ID], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_TX_PIN[COM_ID]);

		/* configure USART Rx as alternate function push-pull */
		gpio_mode_set(COM_GPIO_PORT[COM_ID], GPIO_MODE_AF, GPIO_PUPD_PULLUP,COM_RX_PIN[COM_ID]);
		gpio_output_options_set(COM_GPIO_PORT[COM_ID], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,COM_RX_PIN[COM_ID]);

		/* USART configure */
		usart_deinit(com);
		usart_baudrate_set(com,baud);
		usart_receive_config(com, USART_RECEIVE_ENABLE);
		usart_transmit_config(com, USART_TRANSMIT_ENABLE);
		usart_enable(com);
}

//8��LED�Ƴ�ʼ��
void gd_XII_systeminit(void) {	
		gd_eval_led_init(LED1);
		gd_eval_led_init(LED2);
		gd_eval_led_init(LED3);
		gd_eval_led_init(LED4);
		gd_eval_led_init(LED5);
		gd_eval_led_init(LED6);
		gd_eval_led_init(LED7);
		gd_eval_led_init(LED8);
}

//8�����뿪�س�ʼ��
void dipinit(void) {						
		gd_eval_dip_init(DIP1);
		gd_eval_dip_init(DIP2);
		gd_eval_dip_init(DIP3);
		gd_eval_dip_init(DIP4);
		gd_eval_dip_init(DIP5);
		gd_eval_dip_init(DIP6);
		gd_eval_dip_init(DIP7);
		gd_eval_dip_init(DIP8);
}

//���ط��������̵�����ʼ��
void gd_gpio_init(void)	{				
		rcu_periph_clock_enable(RCU_GPIOD);

		gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE ,GPIO_PIN_3);						//��������
		gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_3);
		
		gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE ,GPIO_PIN_12);					//�̵�����
		gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_12);
}

void Uart0WriteByte(char c) {
		usart_data_transmit(EVAL_COM0, c);
		while (RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE));
		return;
}


void Uart0Printf(char* fmt, ...) {
		int i;
		va_list ap;
		va_start(ap, fmt);
		vsprintf(buffer1, fmt, ap);
		va_end(ap);

		for (i = 0; buffer1[i] != '\0'; i++) {
				Uart0WriteByte(buffer1[i]);
		}
				
		while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TC)){}
		return;
	
}

void Uart1WriteByte(char c) {
		usart_data_transmit(EVAL_COM1, c);
		while (RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TBE));
		return;
}

void Uart1Printf(char* fmt, ...) {
		int i;
		va_list ap;
		va_start(ap, fmt);
		vsprintf(buffer2, fmt, ap);
		va_end(ap);

		for (i = 0; buffer2[i] != '\0'; i++) {
				Uart1WriteByte(buffer2[i]);
		}
				
		while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TC)){}
		return;
}

//#ifdef __GNUC__
//  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif /* __GNUC__ */

//PUTCHAR_PROTOTYPE
//{
//    usart_data_transmit(EVAL_COM0, (uint8_t)ch);
//    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE));
//    return ch;
//}	
		
		
		
//#if defined(__CC_ARM) || defined(__ICCARM__)
//int fputc(int ch, FILE *f)
//    {
//        usart_data_transmit(EVAL_COM0, (uint8_t)ch);
//        while (RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE))
//            ;
//        return ch;
//    }
//		#elif defined(__GNUC__) /* GCC: printf will call _write to print */
//    int _put_char(int ch)
//    {
//        usart_data_transmit(EVAL_COM0, (uint8_t)ch);
//        while (usart_flag_get(EVAL_COM0, USART_FLAG_TBE) == RESET)
//        {
//        }

//        return ch;
//    }
//    __attribute__((used)) int _write(int fd, char *ptr, int len)
//    {

//        const uint8_t *current = (const uint8_t *)ptr;

//        for (size_t jj = 0; jj < len; jj++)
//        {
//            _put_char(current[jj]);
//        }
//        return len;
//    }
//#endif
		
