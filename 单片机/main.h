/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/

  #include "stm32f4xx.h"
  #include "stm32f4xx_fsmc.h"
  #include "LCD.h"
  #include "exti.h"
  #include "USART.h"
  #include "AD.h"
  #include "DA.h"
  #include "Timer.h"
  #include "Pingpang.h"
  #include "stm32f4xx_dac.h"
  #include <stdbool.h>
  #include "malloc.h"
  #include "math.h"
  //滑动滤波长度
  #define SHIFT_FITHER_LEN 20
  //通道数 4个
  #define CHANNEL_LEN 2
  //模式数 3个
  #define  MODE_LEN 3
  extern const u32 MODE_REG[MODE_LEN];
  extern u32 FPGA_Read_Register(u16 ch,u16 i);
  //FREQ
  #define FREQ_LEVEL_U 160
  #define FREQ_LEVEL_D 100
  #define FREQ_LEVEL_U_2 15000000
  #define FREQ_LEVEL_D_2 13000000
  extern double FREQ_RESULT;
  extern u8 FREQ_USED_POS;
  extern double_sequeue FREQ_SEQ[CHANNEL_LEN][2];
  extern void FPGA_EXIT_INIT(void);
  extern double getFreq();
  extern void Freq_Init_ALL();
  extern void Freq_Init(int channel,int x);
  extern double Freq_Get();
  extern void Freq_Input(int channel ,int x,double freq);
  extern u8 FREQ_READ_REGISTER(int i);
  
  extern double FREQ_CHINNAL_1_RECV();
  extern double FREQ_CHINNAL_2_RECV();
  extern double FREQ_CHINNAL_3_RECV();
  
  extern PT_THREAD(FREQ_GET_SERVICE(PT *pt));
  extern void Freq_Get_Service(void);
  //IMPLUSE
  extern double IMPLUSE_RESULT;
  extern u8 IMPLUSE_USED_POS;
  extern double_sequeue IMPLUSE_SEQ[CHANNEL_LEN][2];
  extern void Impluse_Init_ALL();
  extern void Impluse_Init(int channel,int x);
  extern double Impluse_Get();
  extern void Impluse_Input(int channel ,int x,double percent);
  extern u8 IMPLUSE_READ_REGISTER(int i);
  
  extern double IMPLUSE_CHINNAL_1_RECV();
  extern double IMPLUSE_CHINNAL_2_RECV();
  extern double IMPLUSE_CHINNAL_3_RECV();
  extern PT_THREAD(IMPLUSE_GET_SERVICE(PT *pt));
  extern void Implus_Get_Service();
  //SPAN
  extern double SPAN_RESULT;
  extern u8 SPAN_USED_POS;
  extern double_sequeue SPAN_SEQ;
  extern void Span_Init_ALL();
  extern double Span_Get();
  extern void Span_Input(double input);
  extern u8 SPAN_READ_REGISTER(int i);
  
  extern double SPAN_RECV();
  
  
#endif /* __MAIN_H */
