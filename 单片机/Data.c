#include "Data.h"
const u32 MODE_REG[MODE_LEN] = {0x20,0x40,0x60};
//FPGA中断初始化
void FPGA_EXIT_INIT(void){
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  //Enable GPIOB clock 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);
  // Enable SYSCFG clock 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  /* Configure PB0 pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /* Connect EXTI Line0 to PB0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);
  /* Configure EXTI Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_ClearFlag(EXTI_Line0);
  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
u32 FPGA_Read_Register(u16 ch,u16 i){
  i *= 2;
  u32 t;
  t = *(EXTERN_RAM)(MODE_REG[ch]+0x600a0000+(i<<1));
  t |= *(EXTERN_RAM)(MODE_REG[ch]+0x600a0000+((i+1)<<1))<<16;
  return t;
}
//队列复制到数组
void Sequeue_Copy(int* len,double* r,double_sequeue* s){
  __disable_irq();
  *len = Sequeue_Getlen(s);
  for(int i=0;i<*len;i++){
    r[i] = Sequeue_Get_One(s,i);
  }
  __enable_irq();
}
//得到中点值
double getMid(double_sequeue* s){
  double temp_data[SHIFT_FITHER_LEN];
  int len;
  Sequeue_Copy(&len,temp_data,s);
  for(int i=0;i<len;i++){
    for(int j=len-1;j>i;j--){
      if(temp_data[j]>temp_data[j-1]){
        double ADC_1 = temp_data[j];
        temp_data[j] = temp_data[j-1];
        temp_data[j-1] = ADC_1;
      }
    }
  }
  if(len > 3){
    return (temp_data[len/2-1]
        +temp_data[len/2]
        +temp_data[len/2+1])/3;
  }else{
    return temp_data[len/2];
  }
}
double getDif(double_sequeue* s){
  double temp_data[SHIFT_FITHER_LEN];
  int len;
  Sequeue_Copy(&len,temp_data,s);
  for(int i=0;i<len;i++){
    for(int j=len-1;j>i;j--){
      if(temp_data[j]>temp_data[j-1]){
        double ADC_1 = temp_data[j];
        temp_data[j] = temp_data[j-1];
        temp_data[j-1] = ADC_1;
      }
    }
  }
  if(len > 3){
    return (temp_data[0] - temp_data[len-1]);
  }
  return 100;
}