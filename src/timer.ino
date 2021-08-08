/*
  timer.ino

  !!! for history see end of file !!!

  ToDos:    ???

  hints:    ???
*/

#include  "timer.h"
#include  "ToF.h"

int key;            // holds the button codes for internal use
static int  tiki;           // counts the ticks how long a key is pressed
static long blkcnt;
int LEDCrit;

// count object for ISR
Ticker CntTicks;
Ticker CntmTicks;

long uptime;
#if (H_RELAY == H_TRUE)
long ontime;
long offtime;
long cycles;
#endif

long Intervall;

/* ---------------------------------------------------------------
 *
 *  intialize the variables for the key handling
 *
 *  only one key is supported yet. for the 4 channel Sonoff
 *  there is to do more
 *
 * --------------------------------------------------------------- */
void Init_Key()
{
 // Setup the button with an internal pull-up :
  pinMode(H_BUTTON_PIN,INPUT_PULLUP);
  sysflag.mode = MODE_STA;       // normal mode at start
  key = KEY_NO;           // no key is pressed
  blkcnt = BLKMODEOFF;
  tiki = KEY_WAIT;
  LEDCrit = H_FALSE;
}

int k=0;
void milli_ISR()
{

  #if( H_TOF == H_TRUE)
  if(sysflag.mode == MODE_STA){
    ToFDistance();
  }
  #endif

  key = !DIG_READ(H_BUTTON_PIN);

  if( key ){
    if( tiki ) tiki--;
  }
  else {
    if( tiki == 0 ){
      if( (sysflag.mode == MODE_AP) || (sysflag.mode == MODE_STA) ){
        sysflag.mode = (sysflag.mode == MODE_AP) ? MODE_CHG_TO_STA:MODE_CHG_TO_AP;
      }
    }
    tiki = KEY_WAIT;
  }

  if (blkcnt) blkcnt--;

  while (LEDCrit);

  if (!blkcnt){
    if(sysflag.blinkmode == BLKMODEFLASH){
      sysflag.blinktime == BLKFLASHOFF ? sysflag.blinktime = BLKFLASHON : sysflag.blinktime = BLKFLASHOFF;
    }

    if( sysflag.blinkmode != BLKMODEOFF ){
      blkcnt = sysflag.blinktime;
      if(!((sysflag.mode == MODE_STA) && (cfg.LED == H_FALSE)))
        DIG_WRITE (H_LED_PIN, !DIG_READ(H_LED_PIN));
    }
  }
}



//  ISR_count() counts the life tickers
void sec_ISR()
{
# if (H_RELAY == H_TRUE)
  if( DIG_READ(H_RELAY_PIN) )
    ontime++;
  else
    offtime++;
# endif

/*DBGL("LED: ")
DBGF(cfg.LED)
DBGL("blink: ")
DBGF(sysflag.blinkmode)
DBGL("Mode: ")
DBGF(sysflag.mode)
*/
  uptime++;
  if( sysflag.APTimeout ) sysflag.APTimeout--;

  if( sysflag.TransmitCycle ) sysflag.TransmitCycle--;

  if(sysflag.mode == MODE_STA){
    if( sysflag.MeasuringCycle )
      sysflag.MeasuringCycle--;
      else{
        sysflag.MeasuringCycle = cfg.MeasuringCycle;
        #if( H_DS1820 == H_TRUE )
        DS1820_Measuring();
        #endif
        #if( H_TOF == H_TRUE)
        sysflag.CntMeasCyc++;
        ToFRange = ToFAvr;
        #endif
      }
  }
}


void LEDControl(long mode, long time){
  DBGF( "LEDControl()" );

  LEDCrit = H_TRUE;
  #if defined(WLAN_SWITCH_SB)
    DIG_WRITE (H_LED_PIN, !DIG_READ(H_RELAY_PIN));
  #elif defined(WLAN_SWITCH_S20)
    DIG_WRITE (H_LED_PIN, HIGH);
  #elif defined (WLAN_DS1820)
    DIG_WRITE (H_LED_PIN, HIGH);
  #elif defined(D1MINI_ToF)
    DIG_WRITE (H_LED_PIN, HIGH);
  #elif defined(D1MINI_DS1820)
    DIG_WRITE (H_LED_PIN, HIGH);
  #endif

  sysflag.blinktime = time;
  if(cfg.LED)
    sysflag.blinkmode = mode;
  else sysflag.blinkmode = BLKMODEOFF;

  LEDCrit = H_FALSE;
  DBGF( "LEDControl() end" );

}

/*
  history
  --------------------- V1.5l
  17.03.21  LED was not switch correct for D!Mini-Boards
  --------------------- V1.4
  29.01.21  LEDControl() added
  19.01.21  D1MINI_ToF added
  --------------------- V1.4

  06.05.20  V1.30a: new WiFi-state machine, sending a message after boot
  20.04.20  V1.30 from now we use platformio instead of the Arduiono stuff
            sysflag.TransmitCycle is always decremented
  26.04.19  V1.11 removed an error on switching the LED. Sonoff-Basic has no
            LED in parallel with the relay so we have to use the same one for
            blinking and relais state. On Sonoff S20 the LED is dedicated for
            blinking and the relais state is indicated with the blue (sometimes red)
            LED parallel to the relays
  03.03.19  V1.02: renamed TempLoop to MeasuringLoop which runs from now in sec_ISR.
                    renamed ISR_count to sec_ISR which contains now Measuring and transmit counters
  23.02.19  first Version wirh new Version management. Released for Sonoff S20 and Sonoff Basic
  19.01.19  V0.04 first version to re released
            debuging and error-check for all versions is to be done
  16.12.18  porting/modificatios to/for SensClient
  19.05.18  Version 1.14
            all lifwtimw stuff moved to timer.ino/timer.h
  17.05.18  version 1.13
            lifetimer added
  27.03.18  version 1.11
            Sonoff-Basic needs different LED control
  21.02.18  back to time control :-P. there are only 2 modes: AP and STA.
            thats easy toggle with key pressed time
  20.02.18  key detection via key count not time pressed
  16.02.18  first version
*/
