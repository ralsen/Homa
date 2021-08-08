/*
  Config.ino

  !!! for history see end of file !!!

  ToDos:

  hints:    ???
*/


#include  "Config.h"
#include  "WebServer.H"

#include <EEPROM.h>

configData_t  cfg;
sysflagData_t sysflag;

void EraseConfig() {
  // Reset EEPROM bytes to '0' for the length of the data structure
  DBGF( "EraseConfig()");
  EEPROM.begin(sizeof(cfg));
  for (int i = CFGSTART; i < (int)sizeof(cfg); i++) {
    EEPROM.write(i, 0);
  }
  delay(200);
  EEPROM.commit();
  EEPROM.end();
}

void SaveConfig() {
  // Save configuration from RAM into EEPROM
  cfg.hash = CalcHashConfig();
  DBGF( "SaveConfig()");
  DBGLN("EEPROM.begin()");
  EEPROM.begin(sizeof(cfg));
  DBGLN("EEPROM.put()");
  EEPROM.put( CFGSTART, cfg );
  delay(200);
  DBGLN("EEPROM.commit()");
  EEPROM.commit(); // Only needed for ESP8266 to get data written
  DBGLN("EEPROM.end()");
  EEPROM.end();                         // Free RAM copy of structure
  delay(500);     // wird nach EEPROM.end scheinbar gebraucht sonst kommt der WTD
  DBGLN(cfg.hash);
}

void LoadConfig() {
  // Loads configuration from EEPROM into RAM

  DBGF( "LoadConfig()")

  EEPROM.begin(sizeof(cfg));
  EEPROM.get( CFGSTART, cfg );
  EEPROM.end();
  sysflag.MeasuringCycle = SEND_AFTER_BOOT_SEC; //cfg.MeasuringCycle;
  sysflag.TransmitCycle = SEND_AFTER_BOOT_SEC+1;
  sysflag.APTimeout = DEFAULT_APTIMEOUT;
  sysflag.blinkmode = BLKMODEOFF;
}

void SetToDefault(){
  DBGF( "SetToDefault()" );
  // CHN:
  EraseConfig();
  cfg.LED = DEFAULT_LED;
  strcpy (cfg.SSID, DEFAULT_SSID);
  strcpy (cfg.password,DEFAULT_PASSWORD);
  strcpy (cfg.hostname, DEFAULT_HOSTNAME);
  strcpy (cfg.APname, DEFAULT_APNAME);
  strcpy (cfg.MACAddress, (WiFi.macAddress()).c_str());
  cfg.LocalIP[0]=0;
  cfg.fixip[0]=0;
  strcpy (cfg.server, DEFAULT_SERVER);
  strcpy (cfg.service, DEFAULT_SERVICE);
  cfg.MeasuringCycle = DEFAULT_MEASCYCLE;
  cfg.TransmitCycle = DEFAULT_TRANSCYCLE;
  cfg.PageReload  = DEFAULT_PAGERELOAD;
  cfg.APTimeout = DEFAULT_APTIMEOUT;
  cfg.hash = CalcHashConfig();
  SaveConfig();
}


long CalcHashConfig(){
  int i;
  long hash = 0;
  uint8_t *ptr=(uint8_t*)&cfg;

  DBGF( "CalcHashConfig()")

  for (i=CFGSTART; i<((int)sizeof(cfg)-(int)sizeof(cfg.hash)); i++){
    hash += *ptr++ * i + 1;
  }
  return hash;
}

int TestHashConfig(){
  DBGF( "TestHashConfig()")
  LoadConfig();
  return ((CalcHashConfig()-cfg.hash) ? 0:1);
}

/*
  history
  20.04.20  V1.30 from now we use platformio instead of the Arduiono stuff
            many changes for debugging and some type castings
            SEND_AFTER_BOOT implemented in sysflag
            setToDefaults erases the EEPROM at first
  18.04.20  some debug output changed
            send a message after boot
  08.03.19  server and service now variable
  23.02.19  first Version wirh new Version management. Released for Sonoff S20 and Sonoff Basic
  19.01.19  V0.04 first version to re released
            debuging and error-check for all versions is to be done

  19.02.18  first version
            routines are from http://www.kriwanek.de/index.php/de/homeautomation/esp8266/364-eeprom-f%C3%BCr-parameter-verwenden

*/
