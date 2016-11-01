#include "NumBar_CPP.h"
#include "Freq.h";
extern NumBar_CPP numBar_freq;
extern NumBar_CPP numBar_tim;
//Ƶ������
double_sequeue FREQ_SEQ[CHANNEL_LEN][2];
double _FREQ_SEQ_BASE[CHANNEL_LEN*2][SHIFT_FITHER_LEN+1];
u8 FREQ_USED_POS = 2;
double FREQ_RESULT;
//ͨ��1����
double FREQ_CHINNAL_1_RECV(){
  long t_b = FPGA_Read_Register(0,2);
  long t_t = FPGA_Read_Register(0,3);
  return ((double)t_t)/t_b*100000000;
}
//ͨ��2����
double FREQ_CHINNAL_2_RECV(){
  long t_b = FPGA_Read_Register(0,0);
  long t_t = FPGA_Read_Register(0,1);
  return ((double)t_t)/t_b*100000000;
}
//ͨ��3����
double FREQ_CHINNAL_3_RECV(){
  long t_b2 = FPGA_Read_Register(0,4);
  return ((double)100000000)/t_b2;
}
//��ȡ�Ĵ���
u8 FREQ_READ_REGISTER(int i){
  if(1 == i){
    return (FPGA_Read_Register(0,5)&0xff0000)>>16;
  }
  else if(2 == i){
    return (FPGA_Read_Register(0,5)&0xff00)>>8;
  }
  else if(3 == i){
    return (FPGA_Read_Register(0,5)&0xff);
  }
}

//��ʼ��ȫ��������
void Freq_Init_ALL(){
  for(int i=0;i<CHANNEL_LEN;i++){
    for(int j=0;j<2;j++){
      Freq_Init(i,j);
    }
  }
}
//��ʼ������������
void Freq_Init(int channel,int x){
  Sequeue_Init(&(FREQ_SEQ[channel][x]),_FREQ_SEQ_BASE[channel*2+x],SHIFT_FITHER_LEN+1);
}
//���뵥��double�����ݵ�ͨ��
void Freq_Input(int channel ,int x,double freq){
  if(freq>0){
    if(Sequeue_Getlen(&(FREQ_SEQ[channel][x]))>SHIFT_FITHER_LEN-1){
      Sequeue_Out_Queue(&(FREQ_SEQ[channel][x]));
    }
    Sequeue_In_Queue(&(FREQ_SEQ[channel][x]),freq);
    //1ͨ�� ����RESULT����
    if(channel == 0 && x == 0 &&FREQ_USED_POS == 1){
      FREQ_RESULT = freq;
    }
    //2ͨ�� ����RESULT����
    if(channel == 1 && x == 0 &&FREQ_USED_POS == 2){
      FREQ_RESULT = freq;
    }
    //3ͨ�� ����RESULT����
    else if(channel == 1 && x == 1 &&FREQ_USED_POS == 3){
      FREQ_RESULT = freq;
    }
  }  
}
//����Ƶ��ֵ��λ
void Freq_Value_UnitFix(NumBar_CPP* p,int value){
  static u8 UnitLevel = 0;
  double *value_t = (double*)(void*)value;
  if(*value_t>1050000){
    p->setValueReal(*value_t/1000000);
    if(UnitLevel != 2){
      p->setRtag(String_L(0,0,"MHz",WHITE));
    }
    UnitLevel = 2;
  }
  else if(*value_t>1050){
    p->setValueReal(*value_t/1000);
    if(UnitLevel != 1){
      p->setRtag(String_L(0,0,"kHz",WHITE));
    }
    UnitLevel = 1;
  }
  else{
    if(UnitLevel != 0){
      p->setRtag(String_L(0,0,"Hz ",WHITE));
    }
    UnitLevel = 0;
  }
}
//��������ֵ��λ
void Tim_Value_UnitFix(NumBar_CPP* p,int value){
  static u8 UnitLevel = 0;
  double *value_t = (double*)(void*)value;
  *value_t *= 1000020000;
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
//�õ����� ���û�Ԥ��
double Freq_Get(){
  if(FREQ_USED_POS == 1){
    return getMid(&(FREQ_SEQ[0][0]));
  }
  else if(FREQ_USED_POS == 2){
    return getMid(&(FREQ_SEQ[1][0]));
  }
  else if(FREQ_USED_POS == 3){
    if(FREQ_RESULT<10&&
       abs(Sequeue_Get_Rear(&(FREQ_SEQ[1][1]))-Sequeue_Get_Front(&(FREQ_SEQ[1][1])))>0.1){
      return FREQ_RESULT;
    }else{
      return getMid(&(FREQ_SEQ[1][1]));
    }
  }
}
//FREQ���ݻ�÷�����
PT_THREAD(FREQ_GET_SERVICE(PT *pt)){
  static u8 counter_1 = 0;
  static u8 counter_2 = 0;
  static u8 counter_3 = 0;
  PT_BEGIN(pt);
  while(1){
    PT_WAIT_UNTIL(pt,pt->ready);
    pt->ready = 0;
    u8 reg_1 = FREQ_READ_REGISTER(1);
    u8 reg_2 = FREQ_READ_REGISTER(2);
    u8 reg_3 = FREQ_READ_REGISTER(3);
    //reg_2�Ѿ����������� ����ֵ̫С
    if((counter_2 == reg_2 || FREQ_RESULT<FREQ_LEVEL_D)
       &&counter_3 != reg_3){
      //����ͨ��3
      FREQ_USED_POS = 3;
      while(1){
        PT_WAIT_UNTIL(pt,pt->ready);
        pt->ready = 0;
        FREQ_RESULT = FREQ_CHINNAL_3_RECV();
        if(FREQ_RESULT>FREQ_LEVEL_U){
          //�����Զ�
          FREQ_USED_POS = 2;
          break;
        }
        Freq_Input(1,1,FREQ_RESULT);
      }
    }
    //ʹ�ø�Ƶ
    else if(FREQ_RESULT>FREQ_LEVEL_U_2 
            || (getDif(&(FREQ_SEQ[0][0]))<5000)){
      //����ͨ��1
      __disable_irq();
      Freq_Init(0,0);
      __enable_irq();
      FREQ_USED_POS = 1;
      while(1){
        PT_WAIT_UNTIL(pt,pt->ready);
        pt->ready = 0;
        FREQ_RESULT = FREQ_CHINNAL_1_RECV();
        if(FREQ_RESULT<FREQ_LEVEL_D_2||getDif(&(FREQ_SEQ[0][0]))>50000){
          FREQ_USED_POS = 2;
          break;
        }
        Freq_Input(0,0,FREQ_RESULT);
      }
    }
    counter_1 = reg_1;
    counter_2 = reg_2;
    counter_3 = reg_3;
  }
  PT_END(pt);
}
void Freq_Get_Service(){
    //��ʱ�� �Ⱦ��Ȳ�Ƶ
    static u8 counter_1 = 0;
    static u8 counter_2 = 0;
    static u8 counter_3 = 0;
    u8 reg_1 = FREQ_READ_REGISTER(1);
    u8 reg_2 = FREQ_READ_REGISTER(2);
    u8 reg_3 = FREQ_READ_REGISTER(3);
    //���ȸ��¸�Ƶͨ��
    if(reg_1 != counter_1){
        Freq_Input(0,0,FREQ_CHINNAL_1_RECV());
    }
    //Ȼ����´θ�Ƶͨ��
    if(reg_2 != counter_2){
        Freq_Input(1,0,FREQ_CHINNAL_2_RECV());
    }
    //Ȼ����µ�Ƶͨ��
    if(reg_3 != counter_3){
        Freq_Input(1,1,FREQ_CHINNAL_3_RECV());
    }
    counter_1 = reg_1;
    counter_2 = reg_2;
    counter_3 = reg_3;
}
//FREQ������ʾ����
PT_THREAD(FREQ_SHOW(PT *pt)){
  PT_BEGIN(pt);
  while(1){
    PT_WAIT_UNTIL(pt,pt->ready);
    pt->ready = 0;
    double t_freq = Freq_Get();
    if(numBar_freq.getShow()){
      numBar_freq.setValue(t_freq);
    if(t_freq>0.5&&numBar_tim.getShow()){
      numBar_tim.setValue(1/t_freq);
    }
    }
    PT_END(pt);
  }
}