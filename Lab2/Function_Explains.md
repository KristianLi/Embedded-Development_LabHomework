# 函数功能记录
芯力量服务器IP 47.99.144.16 端口 8051

void system_init(void) 初始化

## gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_8); 设置GPIOA8为输出模式
行参：

- GPIO_MODE_INPUT 输入模式
- GPIO_MODE_OUTPUT 普通输出
- GPIO_MODE_ANALOG 模拟模式
- GPIO_MODE_AF 复用模式 
- GPIO_PUPD_NONE 不带上下拉 
- GPIO_PUPD_PULLUP 上拉 
- GPIO_PUPD_PULLDOWN 下拉
- pin为引脚号，可以为多个引脚或运算

## gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8); 设置GPIOA8为推挽输出，速度50MHz
行参：
- GPIO_OTYPE_PP 推挽输出
- GPIO_OTYPE_OD 开漏输出
- GPIO_OSPEED_2MHZ 2MHz
- GPIO_OSPEED_25MHZ 25MHz
- GPIO_OSPEED_50MHZ 50MHz
- GPIO_OSPEED_100MHZ 100MHz
- GPIO_OSPEED_MAX 最大速度
- pin为引脚号，可以为多个引脚或运算

## gpio_bit_set(GPIOA, GPIO_PIN_8); 设置GPIOA8为高电平

## gpio_bit_reset(GPIOA, GPIO_PIN_8); 设置GPIOA8为低电平

## gd_eval_led_on(LED1); 点亮LED1

## gd_eval_led_off(LED1); 熄灭LED1

## gd_eval_led_toggle(LED1); LED1翻转

## gd_eval_key_state_get(KEY_WAKEUP); 获取按键状态

