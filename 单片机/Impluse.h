#ifndef IMPLUS_H_
#define IMPLUS_H_
#include "NumBar_CPP.h"
extern "C"{
  #include "main.h"
  #include "Data.h"
}
extern NumBar_CPP numBar_impluse;
extern PT_THREAD(IMPLUSE_SHOW(PT *pt));
#endif