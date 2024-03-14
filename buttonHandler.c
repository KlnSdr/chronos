#include "buttonHandler.h"

void rightButtonPressed(){
    if (state == SET_HOUR) {
        if(hour == 23){
            setHour(0);
        }else{
            setHour(hour+1);
        }
    } else if (state == SET_MINUTE) {
        if (minutes == 59) {
            setMinute(0);
        } else {
            setMinute(minutes + 1);
        }
    } else if (state == SLEEP) {
        state = DEFAULT;
    } else if (state == DEFAULT) {
        state = SLEEP;
    }
}

void leftButtonPressed(){
    if (prellLeft == 0) {
        prellLeft = 13;
        if (state == SET_HOUR) {
            if(hour == 0){
                setHour(23);
            }else{
                setHour(hour-1);
            }
        } else if (state == SET_MINUTE) {
            if (minutes == 0) {
                setMinute(59);
            } else {
                setMinute(minutes - 1);
            }
        }
    }
}

void middleButtonPressed(){
    switch (state) {
        case DEFAULT:
            stopTimer();
            state = SET_HOUR;
            break;
        case SET_HOUR:
            state = SET_MINUTE;
            break;
        case SET_MINUTE:
            seconds = 0;
            startTimer();
            state = DEFAULT;
            break;
        case SLEEP:
            state = DEFAULT;
            break;
    }
}
