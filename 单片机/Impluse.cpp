#include "NumBar_CPP.h"
#include "Impluse.h"
//Ƶ������
double_sequeue IMPLUSE_SEQ[CHANNEL_LEN][2];
double _IMPLUSE_SEQ_BASE[CHANNEL_LEN*2][SHIFT_FITHER_LEN+1];
u8 IMPLUSE_USED_POS = 1;
double IMPLUSE_RESULT;
//ͨ��1����
double IMPLUSE_CHINNAL_1_RECV(){
  long t_b = FPGA_Read_Register(1,3);
  long t_t = FPGA_Read_Register(1,4);
  return ((double)t_b)/(t_b+t_t);
}
//ͨ��2����
double IMPLUSE_CHINNAL_2_RECV(){
  long t_b = FPGA_Read_Register(1,0);
  long t_t = FPGA_Read_Register(1,1);
  return ((double)t_b)/(t_b+t_t);
}
//ͨ��3����
double IMPLUSE_CHINNAL_3_RECV(){
  long t_b2 = FPGA_Read_Register(1,2);
    
  long t_t2  = FPGA_Read_Register(0,4);
  if(t_t2 > 0){
    return ((double)(t_t2-t_b2))/t_t2;
  }else{
    return -1;
  }
}
//��ȡ�Ĵ���
u8 IMPLUSE_READ_REGISTER(int i){
  if(1 == i){
    return (FPGA_Read_Register(1,5)&0xff0000)>>16;
  }
  else if(2 == i){
    return (FPGA_Read_Register(1,5)&0xff00)>>8;
  }else if(3 == i){
    return FPGA_Read_Register(1,5)&0xff;
  }
}
//��ʼ��ȫ��������
void Impluse_Init_ALL(){
  for(int i=0;i<CHANNEL_LEN;i++){
    for(int j=0;j<2;j++){
      Impluse_Init(i,j);
    }
  }
}

//��ʼ������������
void Impluse_Init(int channel,int x){
  Sequeue_Init(&(IMPLUSE_SEQ[channel][x]),_IMPLUSE_SEQ_BASE[channel*2+x],SHIFT_FITHER_LEN+1);
}
//���뵥��double�����ݵ�ͨ��
void Impluse_Input(int channel ,int x,double percent){
  if(percent>0.001&&percent<0.98){
    if(Sequeue_Getlen(&(IMPLUSE_SEQ[channel][x]))>SHIFT_FITHER_LEN-1){
      Sequeue_Out_Queue(&(IMPLUSE_SEQ[channel][x]));
    }
    Sequeue_In_Queue(&(IMPLUSE_SEQ[channel][x]),percent);
    //0ͨ�� ����RESULT����
    if(channel == 0 && x == 0 &&IMPLUSE_USED_POS == 1){
      IMPLUSE_RESULT = percent;
    }
    //0ͨ�� ����RESULT����
    if(channel == 1 && x == 0 &&IMPLUSE_USED_POS == 2){
      IMPLUSE_RESULT = percent;
    }
    //3ͨ�� ����RESULT����
    else if(channel == 1 && x == 1 &&IMPLUSE_USED_POS == 3){
      IMPLUSE_RESULT = percent;
    }
  }
}
//�õ����� ���û�Ԥ��
double Impluse_Get(){
  if(IMPLUSE_USED_POS == 1){
    double t_t = 100*getMid(&(IMPLUSE_SEQ[0][0]));
    double t_f = Freq_Get();
    if(t_f>100000&&(t_t<26||t_t>74)){
      double t_t2 = t_t;
      if(t_t2>50){
        t_t2 = 100-t_t2;
      }
      double fix_t = 0;
      double t_ft = t_f/1000000;
      if(t_t2>1.14*t_ft+ 9&&t_t2<0.98*t_ft + 14.88){
        fix_t = 6.2-0.1*(t_t2-10.1);
      }else if(t_t2>=0.98*t_ft + 14.88&&t_t2<0.67*t_ft + 19.85){
        fix_t = 4.8-0.47*(t_t2-19.8);
      }
      fix_t *= (Freq_Get()-100000)/(5000000-100000);
      if(t_t > 50){
        t_t += fix_t;
      }else{
        t_t -= fix_t;
      }
    }
    return t_t;
  }
  else if(IMPLUSE_USED_POS == 2){
    return 100*getMid(&(IMPLUSE_SEQ[1][0]));
  }
  else if(IMPLUSE_USED_POS == 3){
    return 100*IMPLUSE_RESULT;
  }
}
//IMPLUSE���ݻ�÷�����
PT_THREAD(IMPLUSE_GET_SERVICE(PT *pt)){
  static u8 counter_1 = 0;
  static u8 counter_2 = 0;
  static u8 counter_3 = 0;
  PT_BEGIN(pt);
  while(1){
    PT_WAIT_UNTIL(pt,pt->ready);
    pt->ready = 0;
    if(Freq_Get()<100){
      IMPLUSE_USED_POS = 3;
    }else if(Freq_Get()<10000){
      IMPLUSE_USED_POS = 2;
    }else{
      IMPLUSE_USED_POS = 1;
    }
  }
  PT_END(pt);
}
void Implus_Get_Service(){
  //��ʱ�� �Ⱦ��Ȳ�Ƶ
  static u8 counter_1 = 0;
  static u8 counter_2 = 0;
  static u8 counter_3 = 0;
  u8 reg_1 = IMPLUSE_READ_REGISTER(1);
  u8 reg_2 = IMPLUSE_READ_REGISTER(2);
  u8 reg_3 = IMPLUSE_READ_REGISTER(3);
  //���ȸ��¸�Ƶͨ��
  if(reg_1 != counter_1){
      Impluse_Input(0,0,IMPLUSE_CHINNAL_1_RECV());
  }
  //Ȼ����µ�Ƶͨ��
  if(reg_2 != counter_2){
      Impluse_Input(1,0,IMPLUSE_CHINNAL_2_RECV());
  }
  //Ȼ����µ�Ƶͨ��
  if(reg_3 != counter_3){
      Impluse_Input(1,1,IMPLUSE_CHINNAL_3_RECV());
  }
  counter_1 = reg_1;
  counter_2 = reg_2;
  counter_3 = reg_3;
}
//FREQ������ʾ����
PT_THREAD(IMPLUSE_SHOW(PT *pt)){
  PT_BEGIN(pt);
  while(1){
    PT_WAIT_UNTIL(pt,pt->ready);
    pt->ready = 0;
    double t_t = Impluse_Get();
    if(numBar_impluse.getShow()){
      numBar_impluse.setValue(t_t);
    }
    PT_END(pt);
  }
}