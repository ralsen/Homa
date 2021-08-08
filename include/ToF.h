/*
  ToF.h

  !!! for history see end of file !!!

  ToDos:    -

  hints:    -
*/

#if (H_TOF == H_TRUE)
# ifndef _TOF_H_
# define _TOF_H_

//#include "wire.h"
#include <VL53L1X.h>

# define TOF_FALSE    0
# define TOF_TRUE     0x5a5a
# define TOF_AVR_CNT  10
# define TOF_INTERVAL 5

  extern int  ToFPresent;
  extern int  ToFRange;
  extern int  ToFAvr;

  int   SetupToF(void);
  void  ToFDistance(void);

# endif // _TOF_H_
#endif //(H_TOF == H_TRUE)
/*
  history:
  13.01.21  first initial versions for V1.4xx

  todo:   -

  hibts:  -
*/
