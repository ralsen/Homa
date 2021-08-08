/*
  Homa.h

  !!! for history see end of file !!!

  all defines depending of what is defined in version.h

  ToDos:    -

  hints:    -
*/


#ifndef _HOMA_H_
#define _HOMA_H_

#include  "version.h"

#define FOREVER         while(1)delay(1);
#define DEAD            while(1);

#define FALSE           0
#define TRUE            1

#if (H_DBG == H_TRUE)
  #define DBG(x)            Serial.print(x);
  #define DBGF(x)           {Serial.print( millis() ); Serial.print(" File: " ); Serial.print (__FILE__); Serial.print( " Line:");Serial.print (__LINE__); Serial.print( ": -> " ); Serial.print( x ); Serial.println( " <-" );}
  #define DBGLN(x)          {Serial.print( " ---> " ); Serial.println( x );}
  #define DBGL(x)           {Serial.print( " ---> " ); Serial.print( x );}
  #define DBGNL(x)          Serial.println( x );
#else
  #define DBG(x)
  #define DBGF(x)
  #define DBGL(x)
  #define DBGLN(x)
  #define DBGNL(x)
#endif

#define MAXCHAR        30


// these are the blink modes
#define BLKMODEOFF      0
#define BLKMODEON       1
#define BLKPRIOMODEON   2
#define BLKMODEFLASH    4
#define BLKPRIOMODEFLAH 8


// these are the blink modes timings
#define BLKALLERT       5
#define BLKCONF         150 //???
#define BLKNORM         250000
#define BLKWIFIAP       10
#define BLKWIFISTA      50
#define BLKFIRM         100 //???
#define BLKFLASHON      3
#define BLKFLASHOFF     500


// mode value for sysflag.mode
#define MODE_CHG_TO_STA 1
#define MODE_CHG_TO_AP  2
#define MODE_AP         4
#define MODE_STA        8

// values for sysflag.status
#define STATUS_ALLERT   1
#define STATUS_OK       2
#define STATUS_CONF_ERR 4
#define STATUS_HASH_ERR 8
#define STATUS_LED      16

#endif // _HOMA_H_

/*
  history:
  06.05.20  V1.30a: new WiFi-state machine, sending a message after boot
  20.04.20  V1.30 from now we use platformio instead of the Arduiono stuff
  23.02.19  first Version wirh new Version management. Released for Sonoff S20 and Sonoff Basic
  19.01.19  V0.04 first version to re released
            ebuging and error-check for all versions is to be done
  16.12.18  Version 0.01  first version
 */
