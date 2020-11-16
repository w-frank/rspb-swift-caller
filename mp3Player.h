#ifndef mp3Player_H
#define mp3Player_H

#include <SoftwareSerial.h>
#include <Arduino.h>

SoftwareSerial mp3Serial(10, 11);

boolean isPlaying;

#define Start_Byte 0x7E
#define Version_Byte 0xFF
#define Command_Length 0x06
#define End_Byte 0xEF
#define Acknowledge 0x00 // Returns info with command 0x41 [0x01: info, 0x00: no info]

void mp3Init();
void mp3PlayFirst();
void mp3Pause();
void mp3Play();
void mp3PlayNext();
void mp3PlayPrevious();
void mp3SetVolume(int volume);
void execute_CMD(byte CMD, byte Par1, byte Par2);

#endif