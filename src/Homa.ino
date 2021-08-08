/*
  HOMA

  universal system for Sonoffs, NodeMCCUs and other ESP8266 devices.
!!! for history see end of file !!!

ToDos:    ???

hints:    ???
*/


#include  "version.h"
#include  "Homa.h"
#include  <ESP8266mDNS.h>
#include  <Ticker.h>
#include  "timer.h"
#include  "HomaTypes.h"
#include  "config.h"
#include  "WebServer.h"

//#include  "DS1820.h"

#if (H_DS1820 == H_TRUE)
  #include  "DS1820.h"
#endif

//------------------------------------------
const String  MyName  = {"\r\n*************************************************************************************\r\n"
                                            "*******************************        ServESP         ******************************\r\n"
                                            "*************************************************************************************"
                       };
const String  Version = {"\r\n-----> V"
                          VERNR
                         " vom "
                          __DATE__
                         " "
                          __TIME__
                         " "
                          RELEASE
                         " <-----\r\n"
                       };


// for debugging
void printUrlArg() {
  //Alle Parameter seriell ausgeben
  DBGF("printUrlArg()")
  String message = "";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
}


//------------------------------------------
void setup() {
  //Setup Serial port speed
  Serial.begin(115200);
  DBGF("setup()")
  DIG_MODE(H_LED_PIN, OUTPUT)
  DIG_MODE(H_RELAY_PIN, OUTPUT)
  Intervall = uptime = 0;
#if (H_RELAY == H_TRUE)
  ontime = offtime = cycles = 0;
#endif

  Serial.println( MyName );
  Serial.println( Version );

  Init_Key();
  Serial.println("Key service started!");
  CntmTicks.attach_ms(10, milli_ISR);

  LEDControl(BLKMODEON, BLKALLERT);

  // if many systems on the some time try to connect, some are not scheduled from DHCP
  // don´t no why but this delay depending on the ChipID my help
  /*Serial.print( "waiting " );
  Serial.print(ESP.getChipId() % 10000);
  Serial.println( "ms to subscribe to WiFi.\r\n" );
  delay (ESP.getChipId() % 10000);*/

  //Setup WIFI
  // do default configuration if conf not valid
  //EraseConfig();
  if (!TestHashConfig()) {
    Serial.println( " Hash FAILED !!!" );
    Serial.print ("load default configuration, size is: ");
    Serial.println(sizeof(cfg));
    SetToDefault();
    sysflag.mode = MODE_CHG_TO_AP;
    sysflag.status = STATUS_HASH_ERR;
  }
  else {
    Serial.printf(("Hash: 0x%lx"), cfg.hash);
    Serial.println(" is ok");
    sysflag.mode = MODE_CHG_TO_STA;
    sysflag.status = STATUS_OK;
  }
  Serial.println( "" );
  Serial.print( "Hello from device: " );
  Serial.println( cfg.hostname );
  Serial.print( "Hardware:          ");
  Serial.println(DEV_TYPE);
  Serial.print( "MAC-Adress:        " );
  Serial.println( cfg.MACAddress);
  Serial.println( "" );

  CntTicks.attach(1, sec_ISR);
  Serial.println("timer services started!");

    //Setup DS18b20 temperature sensor
#if (H_DS1820 == H_TRUE)
  SetupDS18B20();
#endif
#if (H_TOF == H_TRUE)
  SetupToF();
#endif
  delay(1000);
}

// -----------------------------------------------------------------------------

int toggle = 0;


// -----------------------------------------------------------------------------
long olduptime=0;

void loop() {

// beides wird inTasmota nicht gemacht, aber mdns.advertise, mdns.addserver u.ae.
  MDNS.update();
  server.handleClient();

  switch( sysflag.mode ){
    case MODE_STA:{
      if ((cfg.TransmitCycle) && (!sysflag.TransmitCycle)) DoNormStuff();
      #if (H_RELAY == H_TRUE)
        if ( key && ( toggle == 0 )) {
        DBGLN( "KEY" );
        DIG_WRITE( H_RELAY_PIN, !DIG_READ(H_RELAY_PIN));
        DBGL("Relay switched\r\n");
        cycles++;
        sysflag.TransmitCycle = 0; // send status immediately
        toggle = 1;
      }
      else if ( !key) toggle = 0;
      #endif
      break;
    }
    case MODE_AP:{
      if(!sysflag.APTimeout){
        DBGLN(" !! Restarting now !!!");
        ESP.restart();
      }
      break;
    }
    case MODE_CHG_TO_AP:{
      WiFiStartAP();
      WebServerStart();
      sysflag.APTimeout = cfg.APTimeout;
      sysflag.mode = MODE_AP;
      break;
    }
    case MODE_CHG_TO_STA:{
      if (!WiFiStartClient()) {
        sysflag.mode = MODE_CHG_TO_AP;
      }
      else {
        WebServerStart();
        sysflag.mode = MODE_STA;
        if(!cfg.TransmitCycle) LEDControl(BLKMODEFLASH, BLKFLASHOFF);
        else LEDControl(BLKMODEOFF, -1);
        MDNS.addService("http", "tcp", 80);
      }
      sysflag.MeasuringCycle = 0;
      if(sysflag.TransmitCycle){
        sysflag.TransmitCycle = SEND_AFTER_BOOT_SEC;
        DBGL("sending first message in ");
        DBG(SEND_AFTER_BOOT_SEC);
        DBGNL(" seconds");
      }
        break;

    }
  }
}

void DoNormStuff() {
// !!!hier muss noch zwischen Mess+Transmitzyklen unterschieden werden!!!
  DBGF( "############################ DoNormStuff() #######################################" );
  DBGF(cfg.TransmitCycle);

  LEDControl (BLKMODEON, BLKALLERT);

  WiFiClient client;
  const int httpPort = MyServerPort;

  DBGL("\r\n------------------------------------------------------------------------------------\r\n");
  DBGL("Verbindungsaufbau zu Server: ");
  DBGLN(cfg.server);
  DBGL("Port: ");
  DBGLN(httpPort);

  if (client.connect(cfg.server, httpPort))
  {
    DBGLN("connected]");
    String url = cfg.service; //Url wird zusammengebaut
    url += BuildURL();

    DBGLN("sending this URL: ");
    DBGLN(cfg.server + url);

    DBGLN("[Sending a request]");
    client.print(String("GET /") + url + " HTTP/1.1\r\n" +
                 "Host: " + cfg.server + "\r\n" +
                 "Connection: close\r\n" +
                 "\r\n"
                );

    DBGLN("[Response:]");
    while (client.connected() || client.available())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\n');
        DBGLN(line);
      }
    }
    client.stop();
    sysflag.CntGoodTrans++;
    DBGLN("\n[Disconnected]");
  }
  else
  {
    DBGLN("[connection failed!]");
    client.stop();
    sysflag.CntBadTrans++;
  }

  // Auswertung was der Server gemeldet hat und entsprechend handeln
  DBGL("\r\n------------------------------------------------------------------------------------\r\n");
  sysflag.TransmitCycle = cfg.TransmitCycle;
  LEDControl(BLKMODEOFF, -1);
}

/*
  History:
  --------------------- V1.5
  09.02.21  ToF-Sensor is working
            some Values in the URL of all devices have had "==" instead of "="
  --------------------- V1.4
  19.01.21  pretty up the hello message
  17.01.21  if cfg.TransmitCycle is 0 nothing is send to the Server
            MAC-Address is used for identification and is displayed during
            boot. MACAddress is appended to the device name instead of the
            Chip-Number because this number is not realy unique, espressif
            stated
  --------------------- V1.4
  12.08.20  V1.30d: hash is displayed in hex on debug port
            Transcyc and MeasCyc is transmitted additionaly to the server
  14.05.20  V1.30C: bonjour service added
  06.05.20  V1.30a: new WiFi-state machine, sending a message after boot
  20.04.20  V1.30 from now we use platformio instead of the Arduiono stuff
            SEND_AFTER_BOOT_SEC
  18.06.19  switches status when their status every time when they are switched
  16.06.19  V1.20 switches send their status after TransmitCycle
  17.04.19  V1.10 using new routine for server communication in DoNormStuff
            -> https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/client-examples.html?highlight=client.available
  16.04.19  V1.06 found the error why the devices disconnected from WLAN
            in DoNormStuff was a Wfifi.disconnect() on communication errors
  31.03.19  error the brings the WiFi connection down found
  05.03.19  V1.03: config or DS1820 added (Meascyx, Transcyc and pagereload)
            count the messages to server, the pagereloads (good and bad trys)
            server and service now variable
  03.03.19  V1.02: renamed TempLoop to MeasuringLoop which runs from now in sec_ISR.
  02.03.19  V1.01 Website optimization (code and design)
            Serer- and Servicename input added
  23.02.19  first Version wirh new Version management. Released for Sonoff S20 and Sonoff Basic
  19.01.19  V0.04 first version to re released
            debuging and error-check for all versions is to be done

  22.12.18  Version 0.03  implementation of first version control functionality
  20.12.18  Version 0.02  cleaned up the WiFi and System.Mode switching
            debug enhancements
  16.12.18  Version 0.01  the DS1820-stuff from here to DS1820.ino/h
            AP-Mode dont work now
  28.11.18  Version 0.00  first version. sends a url-string to the php-server

  todos:
            done: zyklische Statusmeldung an Server senden, z.B. täglich
*/
