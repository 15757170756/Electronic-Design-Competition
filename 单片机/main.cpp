/* Includes ------------------------------------------------------------------*/
#include "NumBar_CPP.h"
#include "String_L.h"

#include "SelectBar_L.h"
#include "FPGA.h"
#include "Freq.h"
#include "Span.h"
#include "Impluse.h"
extern "C"{
  #include "main.h"
}
PT PT_thread[8];
//频率显示
NumBar_CPP numBar_freq(27,0,(float)100000,(float)0);  
//周期显示
NumBar_CPP numBar_tim(27,1,(float)100000,(float)0);
//占空比显示
NumBar_CPP numBar_impluse(27,2,(float)9999,(float)0);
//时间间隔显示
NumBar_CPP numBar_span(27,2,(float)100000,(float)0);
//菜单选择
SelectBar_L t_select_bar(0,6,true);
void Select_Item_Fun1(SelectItem_L* p){
  numBar_impluse.hide();
  numBar_span.hide();
}
void Select_Item_Fun2(SelectItem_L* p){
  numBar_impluse.hide();
  numBar_span.show();
}
void Select_Item_Fun3(SelectItem_L* p){
  numBar_span.hide();
  numBar_impluse.show();
}

void Select_Item_Up(){
  t_select_bar.shiftLast();
}
void Select_Item_Down(){
  t_select_bar.shiftNext();
}
int main(void){
  /* SysTick end of count event each 10ms */
  LCD_Init();
  EXTI_init();
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
  Freq_Init_ALL();
  Impluse_Init_ALL();
  
  Span_Init_ALL();
  PT_INIT(&PT_thread[0],100,FREQ_GET_SERVICE);
  PT_ADD_THREAD(&PT_thread[0]);
  PT_INIT(&PT_thread[1],40,FREQ_SHOW);
  PT_ADD_THREAD(&PT_thread[1]);
  PT_INIT(&PT_thread[2],100,IMPLUSE_GET_SERVICE);
  PT_ADD_THREAD(&PT_thread[2]);
  PT_INIT(&PT_thread[3],1,KEY_SERVICE);
  PT_ADD_THREAD(&PT_thread[3]);
  PT_INIT(&PT_thread[4],40,IMPLUSE_SHOW);
  PT_ADD_THREAD(&PT_thread[4]);
  PT_INIT(&PT_thread[5],40,SPAN_SHOW);
  PT_ADD_THREAD(&PT_thread[5]);
  FPGA_EXIT_INIT();
  u16 i;
  numBar_freq.setLtag(String_L(0,0,0,2,WHITE));
  numBar_freq.setRtag(String_L(0,0,"Hz",WHITE));
  numBar_freq.show();
  numBar_freq.setFunc(Freq_Value_UnitFix);
  

  numBar_tim.setLtag(String_L(3,0,2,2,WHITE));
  numBar_tim.setRtag(String_L(3,0,"ms",WHITE));
  numBar_tim.show();
  numBar_tim.setFunc(Tim_Value_UnitFix);
  

  numBar_impluse.setLtag(String_L(3,0,4,3,WHITE));
  numBar_impluse.setRtag(String_L(3,0,"%",WHITE));
  //numBar_impluse.show();
  

  numBar_span.setLtag(String_L(3,0,7,4,WHITE));
  numBar_span.setRtag(String_L(3,0,"ms",WHITE));
  //numBar_span.show();
  numBar_span.setFunc(Span_Value_UnitFix);
  String_L(LCD_STRING_WIDTH/2+3,8,11,5,LCD_STRING_MID,WHITE).show();
  t_select_bar.setTitle(String_L(3,0,16,2,BLUE));
  
  t_select_bar.addSelect(String_L(0,0,18,4,YELLOW));
  t_select_bar.addSelect(String_L(0,0,27,6,YELLOW));
  t_select_bar.addSelect(String_L(0,0,22,5,YELLOW));
  t_select_bar.getSelect(0)->setActive(true);
  t_select_bar.getSelect(0)->setFunc_En(Select_Item_Fun1);
  t_select_bar.getSelect(1)->setFunc_En(Select_Item_Fun2);
  t_select_bar.getSelect(2)->setFunc_En(Select_Item_Fun3);
  t_select_bar.show();
  Set_Key_Func(Select_Item_Up,4);
  Set_Key_Func(Select_Item_Down,8);
  while(1){
    
    PT_SERVICE();
    //PC4
    //PB0
    Implus_Get_Service();  
    Freq_Get_Service();
  }
}