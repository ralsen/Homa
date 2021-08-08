/*
  version.h

  !!! for history see end of file !!!

  ToDos:    -

  hints:    -
*/

#ifndef __VERSION_H
#define __VERSION_H

#define H_FALSE 0
#define H_TRUE  1

//#define H_DBG   H_FALSE
//#define VERNR   "1.30"

// #define WLAN_SWITCH_SB
// #define WLAN_SWITCH_S20
 //#define WLAN_SWITCH_NM
// #define WLAN_DS1820
// #define WLAN_PIR_NM        // noch nicht existent


#if (H_DBG == H_TRUE)
  #define RELEASE         "NOT REALESED"
#else
  #define RELEASE
#endif

/* ----------------------------------------------------------------------
;        Versionsnummern, Mediatyp
; -------------------------------------------------------------------- */

#if defined(WLAN_SWITCH_SB)
# define DEV_TYPE       "SONOFF_BASIC"
# define FNC_TYPE       "Switch"
# define DeviceAddress  int
# define H_SWITCH        H_TRUE
# define H_DS1820        H_FALSE
# define H_PIR           H_FALSE
# define H_BUTTON        H_TRUE
# define H_LED           H_TRUE
# define H_RELAY         H_TRUE
# define H_TOF           H_FALSE

#elif defined(WLAN_SWITCH_S20)
# define DEV_TYPE       "SONOFF_S20"
# define FNC_TYPE       "Switch"
# define DeviceAddress  int
# define H_SWITCH        H_TRUE
# define H_DS1820        H_FALSE
# define H_PIR           H_FALSE
# define H_BUTTON        H_TRUE
# define H_LED           H_TRUE
# define H_RELAY         H_TRUE
# define H_TOF           H_FALSE

#elif defined(WLAN_SWITCH_NM)
# define DEV_TYPE       "NODEMCU"
# define FNC_TYPE       "Switch"
# define DeviceAddress  int
# define H_SWITCH        H_TRUE
# define H_DS1820        H_FALSE
# define H_PIR           H_FALSE
# define H_BUTTON        H_TRUE
# define H_LED           H_TRUE
# define H_RELAY         H_TRUE
# define H_TOF           H_FALSE

#elif defined(WLAN_DS1820)
# define DEV_TYPE       "NODEMCU"
# define FNC_TYPE       "DS1820"
# define H_SWITCH        H_FALSE
# define H_DS1820        H_TRUE
# define H_PIR           H_FALSE
# define H_BUTTON        H_TRUE
# define H_LED           H_TRUE
# define H_RELAY         H_FALSE
# define H_TOF           H_FALSE

#elif defined(D1MINI_DS1820)
# define DEV_TYPE       "D1_Mini"
# define FNC_TYPE       "DS1820"
# define H_SWITCH        H_FALSE
# define H_DS1820        H_TRUE
# define H_PIR           H_FALSE
# define H_BUTTON        H_TRUE
# define H_LED           H_TRUE
# define H_RELAY         H_FALSE
# define H_TOF           H_FALSE

#elif defined(D1MINI_ToF)
# define DEV_TYPE       "D1_Mini"
# define FNC_TYPE       "ToF"
# define DeviceAddress  int
# define H_SWITCH        H_FALSE
# define H_DS1820        H_FALSE
# define H_PIR           H_FALSE
# define H_BUTTON        H_TRUE
# define H_LED           H_TRUE
# define H_RELAY         H_FALSE
# define H_TOF           H_TRUE

#elif defined(WLAN_PIR_NM)
# define DEV_TYPE       "NODEMCU"
# define FNC_TYPE       "PIR"
# define H_SWITCH        H_FALSE
# define H_DS1820        H_FALSE
# define H_PIR           H_TRUE
# define H_BUTTON        H_TRUE
# define H_LED           H_TRUE
# define H_RELAY         H_FALSE
# define H_TOF           H_FALSE

#else
#error "Hey! welche Version???"
#endif

#include "Hardware.h"

// -----------------------------------------------------
#endif /* #ifndef __VERSION_H */

/*
  history
  --------------------- V1.5k
  27.02.21  ToF-pagereload was constant set to 1 s. now its
            Pagereload
            default AP-SSID is now "ESPnet"
  --------------------- V1.5j
  27.02.21  average calculation for the ToF-Sensor
  --------------------- V1.5i
  26.02.21  ToF not present for new VL53L1X lib updated
            this Version has REALES-State
  --------------------- V1.5g
  25.02.21  using TOF- library "VL53L1X" instead of the old Sparcfun
  --------------------- V1.5g
  23.02.21  "mm" text un ToF-Web side added
  --------------------- V1.5f
  29.01.21  system crahed if the tof sensor failed.
            system checks if ToFPresent is TOF_TRUE or not
  --------------------- V1.5e
  29.01.21  LED on status page moved to upper information block
  --------------------- V1.5b
  29.01.21  LED is controlled via LEDControl()
  --------------------- V1.4x
  24.01.21  if TransCyc = 0 the LED shows a heartbeat
  24.01.21  the LED could be switch off on the Config-Pages
            but the LED is always during boot und WiFi-connect
  18.01.21  FNC_TYPE isnt displayed in headline of status page anymore
  18.01.21  Version is only displayed in hardline og status page
  18.01.21  first messge is send after going into MODE_STA
  17.01.21  ToF sensor added
  17.01.21  if cfg.TransmitCycle is 0 nothing is send to the Server
            MAC-Address is used for identification and is displayed during
            boot. MACAddress is appended to the device name instead of the
            Chip-Number because this number is not realy unique, espressif
            stated

  20.04.20  V1.30 from now we use platformio instead of the Arduiono stuff
            therefore we dont need the defines WLAN_SWITCH_NM and so an.
            this is now in with platformio.ini
  06.11.19  working on:
            after reboot send a message immendently
            send a long message with all infos that are needed on server

  16.04.19  V1.1x should send status to the server in adjustable time
            Versions and Hardware sitch should be clarified
            why is there a delay after the server responce in DoNormStuff in Homa_xxx.ino?
  23.02.19  first Version wirh new Version management. Released for Sonoff S20 and Sonoff Basic
  19.01.19  V0.04 first version to re released
            debuging and error-check for all versions is to be done

  ToDos:
            - statusmeldung an server senden
            - LED dauer an in blinken einbauen

  Errors:
            - warum werden 5 bewsiteausliefrungen gezaehlt wenn Seite vom iPhone aufgerufen wird
            - Warum regiert WLAN schlecht bis garnicht wenn DBG == H_FASLE
            - OTA funktioniert aof nicht  (vor allem SB und S20). Scheinbar hilft es machnmal das
              Board im bootloader zu straten und gleich wieder aus und einzuschalten
            - warum kommt 404: not found URI: /favicon.ico
*/

/* Problem:
    ich kann nicht fuer beliebige Boards beliebige z.B. Relay konfigurationen
    machen. Beim NodeMcu würde ich aber gerne alle Hardwarekombinationen machen koennen.
    Schon allien zu Testzwecken. Aber das Konzept ist aus erst rund wenn das geht. Und
    das war ja Sinn des Konzepts.
    Nochmal im alten HWDOS nachsehen. Ist im Quellcode nun Bezug auf Geraet oder auf H_-Schalter?
    */
