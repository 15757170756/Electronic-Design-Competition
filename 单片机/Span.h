#include "NumBar_CPP.h"
extern "C"{
  #include "main.h"
  #include "Data.h"
}
extern NumBar_CPP numBar_span;
extern void Span_Value_UnitFix(NumBar_CPP* p,int value);
extern PT_THREAD(SPAN_SHOW(PT *pt));