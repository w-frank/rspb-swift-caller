/**
 * Copyright (C) 2022 Will Frank
 *                                             
 * This file is part of the RSPB Swift Caller.                                          
 * 
 *    This program is free software: you can redistribute it and/or modify  
 *    it under the terms of the GNU General Public License as published by  
 *    the Free Software Foundation, version 3.
 *
 *    This program is distributed in the hope that it will be useful, but 
 *    WITHOUT ANY WARRANTY; without even the implied warranty of 
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License 
 *    along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file swift_caller.ino
 * @author Will Frank
 * @date 22 Nov 2020
 * @brief Main program for the Swift Caller.
 */

// Arduino low power modes
#include <LowPower.h>
// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <RTClib.h>
#include <TimeLib.h>

const int INTERRUPT_PIN = 2;
const int MP3_PLAYER_PIN = 3;

const DateTime MORNING_CALL_TIME = DateTime(0, 0, 0, 6, 30, 0);  // 6:30 am BST
const DateTime EVENING_CALL_TIME = DateTime(0, 0, 0, 18, 00, 0); // 18:00 pm BST
const DateTime SEASON_START_TIME = DateTime(0, 5, 1, 6, 20, 0);  // May 1st at 6:20 am BST
const DateTime SEASON_END_TIME   = DateTime(0, 8, 1, 0, 0, 0);   // August 1st

const tmElements_t playForTime = {0, 0, 1}; // How long to play swift calls for (secs, mins, hours)

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

/**
 * Initialisation.
 * Called once when program starts.
 */
void setup() {
    Serial.begin(115200);
    Serial.println("--- Swift Caller ---");

    pinMode(MP3_PLAYER_PIN, OUTPUT);

    initRTC();
    setAlarms();

}

/**
 * Main program loop.
 */
void loop() {

    sendToSleep();

    // -- sleeping -- //

    detachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN));

    Serial.println("Awake! Playing swift calls...");

    // Turn on MP3 player
    digitalWrite(MP3_PLAYER_PIN, HIGH);

    static unsigned long timer = millis();
  
    while (millis() - timer < timeToMillis(playForTime)) {
        // continue looping mp3s for playForTime
    }

    // Turn OFF MP3 player
    digitalWrite(MP3_PLAYER_PIN, LOW);
 
    setAlarms();

}

/**
 * RTC initialisation.
 */
void initRTC() {

    if (!rtc.begin()) {
        Serial.println("Fatal error: couldn't find RTC");
        Serial.flush();
        abort();
    }

    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, let's set the time!");
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }

    // get and print current date & time
    DateTime now = rtc.now();

    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);

    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    // not using the 32K pin, so disable it
    rtc.disable32K();
    
    // configure the alarm to trigger an interrupt
    pinMode(INTERRUPT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), onAlarm, FALLING);
    
    // set alarm 1, 2 flag to false (so alarm 1, 2 didn't happen so far)
    // if not done, this easily leads to problems, as both register aren't 
    // reset on reboot/recompile
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);
    
    // stop oscillating signals at SQW Pin
    // otherwise setAlarm1 will fail
    rtc.writeSqwPinMode(DS3231_OFF);

}

/**
 * Send Arduino to sleep and attach wake interrupt.
 */
void sendToSleep() {

    Serial.println("Sending to sleep... zzz");
    delay(1000); // wait for serial print

    // Turn off MP3 player
    digitalWrite(MP3_PLAYER_PIN, LOW);
    // wait for MP3 player to switch off
    delay(1000);

    // Enter power down state with ADC and BOD module disabled
    // Wake up when falling edge on interrupt pin
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

}

/**
 * Alarm Interrupt Service Routine (ISR).
 * Wake Arduino and detach interrupt.
 */
void onAlarm() {

    Serial.println("Alarm call!");

}

/**
 * Set AM and PM alarms for swift calls.
 * If currently outside of swift season then set alarm
 * to start of swift season.
 */
void setAlarms() {

    // check if month is in swift season or not
    if((rtc.now().month() >= SEASON_START_TIME.month()) && (rtc.now().month() <= SEASON_END_TIME.month())) {
        // in swift season
        Serial.println("In swift season");
        // set Alarm 1 when hours, minutes and seconds = AM call time
        if(!rtc.setAlarm1(MORNING_CALL_TIME, DS3231_A1_Hour)) {
            Serial.println("Error: failed to set AM alarm");
        }
        else {
            Serial.print("AM alarm set: ");
            Serial.print(MORNING_CALL_TIME.hour(), DEC);
            Serial.print(':');
            Serial.print(MORNING_CALL_TIME.minute(), DEC);
            Serial.print(':');
            Serial.print(MORNING_CALL_TIME.second(), DEC);
            Serial.println();  
        }

        // set Alarm 2 when hours and minutes = PM call time
        if(!rtc.setAlarm2(EVENING_CALL_TIME, DS3231_A2_Hour)) {
            Serial.println("Error: failed to set PM alarm");
        }
        else {
            Serial.print("PM alarm set: ");
            Serial.print(EVENING_CALL_TIME.hour(), DEC);
            Serial.print(':');
            Serial.print(EVENING_CALL_TIME.minute(), DEC);
            Serial.print(':');
            Serial.print(EVENING_CALL_TIME.second(), DEC);
            Serial.println();  
        }
    }
    else {
        // outside swift season
        Serial.println("Outside of swift season");
        // set alarm when date (day of month), hours and minutes = season start
        if(!rtc.setAlarm1(SEASON_START_TIME, DS3231_A1_Date)) {
            Serial.println("Error: failed to set season start alarm");
        }
        else {
            Serial.println("Season start alarm set");  
        }
    }
}

/**
 * Convert time (H, M, S) to milliseconds. 
 * 
 * Maximum is 4294967295 ms = 49 days, 17 hours, 2 minutes, 47 seconds.
 * 
 * @param time tmElements_t of hours, minutes and seconds to convert.
 * @return time in milliseconds.
 */
unsigned long timeToMillis(tmElements_t time) {

    unsigned long timeMillis = time.Hour * 60UL * 60UL * 1000UL;
    timeMillis += (time.Minute * 60UL * 1000UL);
    timeMillis += (time.Second * 1000UL);
    return timeMillis;

}

/**
 * Set Arduino built-in LED state (LOW/HIGH) 
 *
 * @param state LED state (LOW/HIGH = OFF/ON)
 */
void builtinLEDControl(bool state) {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, state);
}
