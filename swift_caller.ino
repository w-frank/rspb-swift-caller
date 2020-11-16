// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <RTClib.h>
#include "mp3Player.h"

DateTime DATE_WINDOW_START = DateTime(0, 5, 1, 0, 0, 0);
DateTime DATE_WINDOW_STOP = DateTime(99, 12, 1, 0, 0, 0);
DateTime MORNING_CALL_TIME = DateTime(0, 0, 0, 6, 20, 0);
DateTime EVENING_CALL_TIME = DateTime(0, 0, 0, 22, 28, 0);

RTC_DS3231 rtc;


void setup ()
{

    Serial.begin(115200);
    Serial.println("Initialising swift caller");

    if (! rtc.begin()) 
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        abort();
    }

    if (rtc.lostPower()) 
    {
        Serial.println("RTC lost power, let's set the time!");
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }

    mp3Init();

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop () 
{

    if (checkTimeToCall())
    {
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(1000);                       // wait for a second
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        mp3PlayFirst();

    }

}

bool checkTimeToCall()
{
    bool timeToCall = 0;
    DateTime now = rtc.now();

    Serial.println(now.timestamp());

    if (now.month() >= DATE_WINDOW_START.month() && now.month() <= DATE_WINDOW_STOP.month())
    {

        if (now.timestamp(now.TIMESTAMP_TIME) == MORNING_CALL_TIME.timestamp(MORNING_CALL_TIME.TIMESTAMP_TIME) || 
            now.timestamp(now.TIMESTAMP_TIME) == EVENING_CALL_TIME.timestamp(EVENING_CALL_TIME.TIMESTAMP_TIME))
        {
            timeToCall = 1;
        }
    }

    return timeToCall;
}
