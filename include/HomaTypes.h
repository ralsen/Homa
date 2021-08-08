/*
  HomaTypes.h

  !!! for history see end of file !!!

  ToDos:    -

  hints:    -
*/

#ifndef _SENSCLIENTTYPES_H_
#define _SENSCLIENTTYPES_H_

//#include <WString.h>
//#include "Homa.h"

#define CFGSTART              0         // startadress og cfg-block in EEPROM
#define DEFAULT_LED           H_TRUE

#define DEFAULT_SSID          "unknown"
#define DEFAULT_PASSWORD      "unknown"

// #define DEFAULT_SSID          "janzneu"
// #define DEFAULT_PASSWORD      "D1AFFE1234!"

#define DEFAULT_HOSTNAME      "No-Name"
#define DEFAULT_APNAME        "ESPnet"
#define DEFAULT_MEASCYCLE     150       // Measurement cycle in sec.
#define DEFAULT_PAGERELOAD    10        // page reload cycle in sec.
#define DEFAULT_APTIMEOUT     300       // stay for this in AP-Mode before
                                        // retrying STA-Mode again

#if (WLAN_SWITCH_SB)
  #define DEFAULT_TRANSCYCLE    300     // transmit cycle to server in sec.
#else
  #define DEFAULT_TRANSCYCLE    3600
#endif

#define DEFAULT_SERVICE       "/DummyServ.php/"
#define DEFAULT_SERVER        "192.168.1.6"


typedef struct {
  int   vald;
  int   LED;                     // 0=no configuration, 1=valid configuration
//  int   dummy;                 // for hash testings
  char  SSID[MAXCHAR+4];         // SSID of WiFi
  char  password[MAXCHAR+4];     // Password of WiFi
  char  hostname[MAXCHAR+4];     // hostname
  char  APname[MAXCHAR+4];       // Access point name
  char  MACAddress[MAXCHAR+4];   // holds MAC
  char  LocalIP[MAXCHAR+4];
  char  fixip[MAXCHAR+4];       // if fixed IP is used store it here
  char  server[MAXCHAR+4];
  char  service[MAXCHAR+4];
  long  MeasuringCycle;
  long  TransmitCycle;
  long  PageReload;
  long  APTimeout;
  long  hash;                        // hash, must be the very last value
} configData_t;

typedef struct {
  int   mode;
  long  blinkmode;
  long  blinktime;
  int   status;
  int   wifi;
  long  MeasuringCycle;
  long  MeasuringReloadCycle;
  long  TransmitCycle;
  long  CntGoodTrans;
  long  CntBadTrans;
  long  CntPageDelivered;
  long  CntMeasCyc;
  long  APTimeout;
} sysflagData_t;

#endif // _SENSCLIENTTYPES_H_

/*
  history:
  --------------------- V1.4
  19.01.21  ChipID removed
            char[] for macAddress was to short, enlarged
  --------------------- V1.4

  06.05.20  V1.30a: new WiFi-state machine, sending a message after boot
  05.03.19  V1.03: config or DS1820 added (Meascyx, Transcyc and pagereload)
            count the messages to server, the pagereloads (good and bad trys)
            server and service now variable
  23.02.19  first Version wirh new Version management. Released for Sonoff S20 and Sonoff Basic
  19.01.19  V0.04 first version to re released
            debuging and error-check for all versions is to be done

  30.12.18  new variables for pagerload, MeasuringCylces etc.
  30.03.18  version 1.12
            MAC, IP and chip number added in cfg structure
  21.02.18  version 1.03
            wifi in sysflagData added
  19.02.18  first version
 */
