#include "NumBar_CPP.h"
extern "C"{
  #include "main.h"
  #include "Data.h"
}
extern void Freq_Value_UnitFix(NumBar_CPP* p,int value);
extern void Tim_Value_UnitFix(NumBar_CPP* p,int value);
extern PT_THREAD(FREQ_SHOW(PT *pt));
extern PT_THREAD(FREQ_GET_SERVICE(PT *pt));