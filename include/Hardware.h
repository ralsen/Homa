/*
  Hardware.h

  !!! for history see end of file !!!

  all defines depending of what is defined in version.h

  ToDos:    -

  hints:    -
*/

#ifndef _HARDWARE_H
#define _HARDWARE_H


#if (H_LED == H_TRUE) || (H_RELAY == TRUE)
# define  DIG_WRITE(pin, val)   digitalWrite(pin, val)
# define  DIG_READ(pin)         digitalRead(pin)
# define  DIG_MODE(pin, inout)  pinMode(pin, inout);

# if defined(WLAN_SWITCH_SB) || defined(WLAN_SWITCH_S20)
#   define H_LED_PIN      13
#   define H_RELAY_PIN    12
# elif    defined(WLAN_SWITCH_NM) || defined(WLAN_DS1820) || defined(D1MINI_DS1820) || defined(D1MINI_ToF)
#   define H_LED_PIN      2
#   define H_RELAY_PIN    3
# else
#   error   "H_LED_PIN undefined"
# endif
#else     // if (H_LED == FALSE)
# define  H_LED_PIN
# define  H_RELAY_PIN
# define  DIG_WRITE(pin, val)
# define  DIG_READ(pin)
# define  DIG_MODE(pin, inout)
#endif



#if (H_DS1820 == H_TRUE)
# define H_DS1820_PIN     D3
# define H_DS180_MAX_DEV  10
#endif


#if (H_BUTTON == H_TRUE)
# define H_BUTTON_PIN     0
#endif

#endif // #endif _HARDWARE_H

/*
  History:
  --------------------- V1.4
  19.01.21  D1MINI_ToF added
  --------------------- V1.4

  20.04.20  V1.30 from now we use platformio instead of the Arduiono stuff
  23.02.19  first Version wirh new Version management. Released for Sonoff S20 and Sonoff Basic
  19.01.19  V0.04 first version to re released
            debuging and error-check for all versions is to be done
  16.12.18  first version

  todos:
 */
