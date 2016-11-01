#include "Span.h"
double SPAN_RESULT;
u8 SPAN_USED_POS = 1;
double_sequeue SPAN_SEQ;
double _SPAN_SEQ_BASE[SHIFT_FITHER_LEN+1];

//通道接收
double SPAN_RECV(){
  long t_b = FPGA_Read_Register(2,0);
  long t_t = FPGA_Read_Register(2,1);
  double t = Freq_Get();
  if(t >0.2){
    if(PCin(4)){
      return 500000000*((double)t_t)/(t_t+t_b)/Freq_Get();
    }else{
      if((2-((double)t_t)/(t_t+t_b))>1.99){
        return 0;
      }
      else{
        return (1000000000-500000000*((double)t_t)/(t_t+t_b))/Freq_Get();
      }
    }
  }
}
//初始化全部缓冲区
void Span_Init_ALL(){
  Sequeue_Init(&SPAN_SEQ,_SPAN_SEQ_BASE,SHIFT_FITHER_LEN+1);
}
//输入单个double型数据到通道
void Span_Input(double input){
  if(input>=0 && input <100000000){
    if(Sequeue_Getlen(&SPAN_SEQ)>SHIFT_FITHER_LEN-1){
      Sequeue_Out_Queue(&SPAN_SEQ);
    }
    Sequeue_In_Queue(&SPAN_SEQ,input);
    SPAN_RESULT = input;
  }
}
//修正频率值单位
void Span_Value_UnitFix(NumBar_CPP* p,int value){
  static u8 UnitLevel = 0;
  double *value_t = (double*)(void*)value;
  if(*value_t>1050000){
    p->setValueReal(*value_t/1000000);
    if(UnitLevel != 0){
      p->setRtag(String_L(0,0,"ms",WHITE));
    }
    UnitLevel = 0;
  }
  else if(*value_t>1050){
    p->setValueReal(*value_t/1000);
    if(UnitLevel != 1){
      p->setRtag(String_L(0,0,"us",WHITE));
    }
    UnitLevel = 1;
  }
  else{
    if(UnitLevel != 2){
      p->setRtag(String_L(0,0,"ns",WHITE));
    }
    UnitLevel = 2;
  }
}
//得到数据 给用户预留
double Span_Get(){
  return getMid(&(SPAN_SEQ));
}
//FREQ数据显示函数
PT_THREAD(SPAN_SHOW(PT *pt)){
  PT_BEGIN(pt);
  while(1){
    PT_WAIT_UNTIL(pt,pt->ready);
    pt->ready = 0;
    numBar_span.setValue(Span_Get());
    PT_END(pt);
  }
}