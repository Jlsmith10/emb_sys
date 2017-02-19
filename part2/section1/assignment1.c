#include "assignment1.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>
#define DEBUG          1 // Debug flag

int button_debouce_time = 2500;

void init_shared_variable(SharedVariable* sv) {
  // You can initialize the shared variable if needed.
  sv->bProgramExit = 0;
  sv->state = STATE_INITIAL;
  sv->button_prev;
  sv->temp_high;
  sv->touch_hit;
  sv->last_button_press;
}

void init_sensors(SharedVariable* sv) {
  printf("Initializing sensors...\n");

  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_YELLOW, OUTPUT);
  pinMode(PIN_TEMP, INPUT);
  pinMode(PIN_TRACK, INPUT);
  pinMode(PIN_TOUCH, INPUT);
  pinMode(PIN_RED, PWM_OUTPUT);
  pinMode(PIN_GREEN, PWM_OUTPUT);
  pinMode(PIN_BLUE, PWM_OUTPUT);
  pinMode(PIN_ALED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  softPwmCreate(PIN_RED, 0, 0xFF);
  softPwmCreate(PIN_GREEN, 0, 0xFF);
  softPwmCreate(PIN_BLUE, 0, 0xFF);


  sv->state = STATE_INITIAL;
  sv->button_prev = 1;
  sv->temp_high = 0;
  sv->touch_hit = 0;
  sv->last_button_press = 0;


}

void body_button(SharedVariable* sv) {
  static int button_count = 0;

  // Debugging state info
  if(DEBUG) {
    switch(sv->state) {
      case STATE_INITIAL:
        fprintf(stderr, "STATE = STATE_INITIAL\n");
        break;
      case STATE_TRACK:
        fprintf(stderr, "STATE = STATE_TRACK\n");
        break;
      case STATE_TOUCH:
        fprintf(stderr, "STATE = STATE_TOUCH\n");
        break;
      case STATE_DRIVING:
        fprintf(stderr, "STATE = STATE_DRIVING\n");
        break;
      default:
        fprintf(stderr, "STATE = UNDEFINED, exiting program\n");
        sv->bProgramExit = 1;
        break;
    }
  }


  switch(sv->state) {
    case STATE_INITIAL:
      if(digitalRead(PIN_BUTTON) == LOW){
        //fprintf(stderr, "BUTTON: Button pressed!\n");
        if(sv->button_prev == 1) {
          sv->button_prev = 0;
          if(millis() - button_debouce_time > sv->last_button_press) {
            sv->state = STATE_DRIVING;
            sv->last_button_press = millis();
          }
        }
        else {
          sv->button_prev = 1;
        }
      }
      break;

    case STATE_TRACK:
    case STATE_TOUCH:
    case STATE_DRIVING:
      if(digitalRead(PIN_BUTTON) == LOW) {
        if(sv->button_prev == 1) {
          sv->button_prev = 0;
          if(millis() - button_debouce_time > sv->last_button_press) {
            sv->state = STATE_INITIAL;
            sv->last_button_press = millis();
          }
        }
        else {
          sv->button_prev = 1;
        }
      }
      break;

    default:
      break;

  }
}

void body_twocolor(SharedVariable* sv) {

  digitalWrite(PIN_YELLOW, LOW);
  switch(sv->state) {
    case STATE_INITIAL:
      // Turn yellow led off
      digitalWrite(PIN_YELLOW, LOW);
      break;

    case STATE_DRIVING:
      // Turn yellow led on
      digitalWrite(PIN_YELLOW, HIGH);

      break;

    case STATE_TRACK:
      break;

    case STATE_TOUCH:
      break;

    default:
      break;
  }
}

void body_temp(SharedVariable* sv) {
  switch(sv->state) {
    case STATE_INITIAL:
      sv->temp_high = 0;

      break;

    case STATE_DRIVING:
      if(digitalRead(PIN_TEMP) == HIGH) {
        printf("TEMP HIGH!\n");
        sv->temp_high = 1;
      } else {
        sv->temp_high = 0;
      }

      break;

    case STATE_TOUCH:
      break;

    case STATE_TRACK:
      break;

    default:
      break;


  }
}

void body_track(SharedVariable* sv) {
  switch(sv->state) {
    case STATE_INITIAL:
      // Do nothing
      break;

    case STATE_DRIVING:
      if(digitalRead(PIN_TRACK) == LOW) { 
        sv->state = STATE_TRACK;
      }
      break;

    case STATE_TRACK:
      // Do nothing
      
      break;

    case STATE_TOUCH:
      // Do nothing
      break;

    default:
      //fprintf(stderr, "TWO_COLOR: State variable corrupted\n");
      //      sv->bProgramExit = 1;
      break;
  }

}

void body_touch(SharedVariable* sv) { 
  sv->touch_hit = 0;
  switch(sv->state) {
    case STATE_INITIAL:
      // Do nothing
      break;

    case STATE_DRIVING:
      if(digitalRead(PIN_TOUCH) == HIGH) {
        sv->state = STATE_TOUCH;
      }

      break;
    case STATE_TRACK:
      // Do nothing
      break;

    case STATE_TOUCH:
      // Do nothing
      if(digitalRead(PIN_TOUCH) == HIGH) { 
        sv->touch_hit = 1;
      }
      break;

    default:
      break;
  }
}

void body_rgbcolor(SharedVariable* sv) {
  switch(sv->state) {
    case STATE_INITIAL:
      // Set RGB LED to blue
      softPwmWrite(PIN_RED, 0x0);
      softPwmWrite(PIN_GREEN, 0x0);
      softPwmWrite(PIN_BLUE, 0xFF);

      break;

    case STATE_DRIVING:
      // Set RGB LED to yellow
      softPwmWrite(PIN_RED, 0xFF);
      softPwmWrite(PIN_GREEN, 0xFF);
      softPwmWrite(PIN_BLUE, 0x0);

      break;

    case STATE_TRACK:
      // Set RGB LED to red
      softPwmWrite(PIN_RED, 0xFF);
      softPwmWrite(PIN_GREEN, 0x0);
      softPwmWrite(PIN_BLUE, 0x0);

      break;

    case STATE_TOUCH:
      // Set RGB LED to purple
      softPwmWrite(PIN_RED, 0xC8);
      softPwmWrite(PIN_GREEN, 0x3B);
      softPwmWrite(PIN_BLUE, 0xFF);

      break;

    default:
      break;


  }
}

void body_aled(SharedVariable* sv) {
  switch(sv->state) {
    case STATE_INITIAL:
      digitalWrite(PIN_ALED, LOW);
      break;

    case STATE_DRIVING:
      if(sv->temp_high == 1) {
        printf("Setting ALED to high\n");
        digitalWrite(PIN_ALED, HIGH);
      }
      else {
        digitalWrite(PIN_ALED, LOW);
      }
      break;

    case STATE_TRACK:
      digitalWrite(PIN_ALED, LOW);
      break;

    case STATE_TOUCH:
      digitalWrite(PIN_ALED, LOW);
      break;

    default:
      //fprintf(stderr, "TWO_COLOR: State variable corrupted\n");
      //sv->bProgramExit = 1;
      break;
  }
}

void body_buzzer(SharedVariable* sv) {
  switch(sv->state) {
    case STATE_INITIAL:
      // Turn off buzzer
      digitalWrite(PIN_BUZZER, LOW);
      break;

    case STATE_DRIVING:
      // Turn off buzzer
      digitalWrite(PIN_BUZZER, LOW);
      break;

    case STATE_TRACK:
      // Turn off buzzer
      digitalWrite(PIN_BUZZER, LOW);
      break;

    case STATE_TOUCH:
      // Turn on buzzer
      if(sv->touch_hit == 1) {
        digitalWrite(PIN_BUZZER, HIGH);
      }
      else {
        digitalWrite(PIN_BUZZER, LOW);
      }
      break;

    default:
      break;
  }
}
