/*
  Config.h

  !!! for history see end of file !!!

  ToDos:    -

  hints:    -
*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include  "Homa.h"
#include  "HomaTypes.h"

extern  configData_t  cfg;
extern  sysflagData_t sysflag;

void eraseConfig();
void saveConfig();
void loadConfig();

#define SEND_AFTER_BOOT_SEC   5

#endif //_CONFIG_H_

/*
  history:
  20.04.20  V1.30 from now we use platformio instead of the Arduiono stuff
            SEND_AFTER_BOOT_SEC defines the time in sec to transmit the first messageafter boot
  23.02.19  first Version wirh new Version management. Released for Sonoff S20 and Sonoff Basic
  19.01.19  V0.04 first version to re released
            debuging and error-check for all versions is to be done

  19.02.18  first version

 */
