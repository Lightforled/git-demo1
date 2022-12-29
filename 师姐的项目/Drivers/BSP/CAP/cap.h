#ifndef _CAP_H
#define _CAP_H

#include "./SYSTEM/sys/sys.h"

#define CAP1(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); \
                  }while(0)       
#define CAP2(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); \
                  }while(0)  
#define CAP3(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET); \
                  }while(0)
#define CAP4(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_RESET); \
                  }while(0)
#define CAP5(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET); \
                  }while(0)
#define CAP6(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_11, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_11, GPIO_PIN_RESET); \
                  }while(0)
#define CAP7(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_RESET); \
                  }while(0)                                        
#define CAP8(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET); \
                  }while(0)
#define CAP9(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET); \
                  }while(0)
#define CAP10(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET); \
                  }while(0)
#define CAP11(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET); \
                  }while(0)
#define CAP12(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET); \
                  }while(0)
#define CAP13(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); \
                  }while(0)
#define CAP14(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); \
                  }while(0)
#define CAP15(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_RESET); \
                  }while(0)
#define CAP16(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, GPIO_PIN_RESET); \
                  }while(0)
#define CAP17(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_RESET); \
                  }while(0)
#define CAP18(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_RESET); \
                  }while(0)                  
#define CAP19(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET); \
                  }while(0)
#define CAP20(x)   do{ x ? \
                      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET); \
                  }while(0)




void cap_init(void);
void cap_change(uint8_t flag, uint8_t id);

#endif // _CAP_H
