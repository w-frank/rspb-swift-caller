#include "mp3Player.h"

void mp3Init()
{
    mySerial.begin(9600);
    delay(1000);
    isPlaying = false;
    playFirst();
    isPlaying = true;
}

void playFirst()
{
    execute_CMD(0x3F, 0, 0);
    delay(500);
    setVolume(20);
    delay(500);
    execute_CMD(0x11,0,1); 
    delay(500);
}

void pause()
{
    execute_CMD(0x0E,0,0);
    delay(500);
}

void play()
{
    execute_CMD(0x0D,0,1); 
    delay(500);
}

void playNext()
{
    execute_CMD(0x01,0,1);
    delay(500);
}

void playPrevious()
{
    execute_CMD(0x02,0,1);
    delay(500);
}

void setVolume(int volume)
{
    execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
    delay(2000);
}

// Excecute the command and parameters
void execute_CMD(byte CMD, byte Par1, byte Par2)
{
    execute_CMD(0x3F, 0, 0);
    delay(500);
    setVolume(20);
    delay(500);
    execute_CMD(0x11,0,1); 
    delay(500);
}
