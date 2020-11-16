#ifndef mp3Player_H
#define mp3Player_H

#include <SoftwareSerial.h>
#include <Arduino.h>

SoftwareSerial mySerial(10, 11);

extern boolean isPlaying;

#define Start_Byte 0x7E
#define Version_Byte 0xFF
#define Command_Length 0x06
#define End_Byte 0xEF
#define Acknowledge 0x00 // Returns info with command 0x41 [0x01: info, 0x00: no info]

void mp3Init();
void playFirst();
void pause();
void play();
void playNext();
void playPrevious();
void setVolume(int volume);
void execute_CMD(byte CMD, byte Par1, byte Par2);

#endif