#include "mp3Player.h"

void mp3Init()
{
    mp3Serial.begin(9600);
    delay(1000);
    isPlaying = false;

}

void mp3PlayFirst()
{
    execute_CMD(0x3F, 0, 0);
    delay(500);
    mp3SetVolume(20);
    delay(500);
    execute_CMD(0x11, 0, 1); 
    delay(500);
    isPlaying = true;
}

void mp3Pause()
{
    execute_CMD(0x0E, 0, 0);
    delay(500);
}

void mp3Play()
{
    execute_CMD(0x0D, 0, 1); 
    delay(500);
}

void mp3PlayNext()
{
    execute_CMD(0x01, 0, 1);
    delay(500);
}

void mp3PlayPrevious()
{
    execute_CMD(0x02, 0, 1);
    delay(500);
}

void mp3SetVolume(int volume)
{
    execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
    delay(2000);
}

// Excecute the command and parameters
void execute_CMD(byte CMD, byte Par1, byte Par2)
{
    word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
    // Build the command line
    byte commandLine[10] = {Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
                            Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
    
    // Send the command line to module (byte-by-byte)
    for (byte k = 0; k < 10; k++)
    {
        mp3Serial.write(commandLine[k]);
    }
}
