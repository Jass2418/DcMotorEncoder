#include "SerialManager.h"

SerialManager::SerialManager()
{
    isComplete=false;
    checkSum=0;
    byte_command=0;
}

void SerialManager::Process(unsigned char byteRx)
{
    switch(status)
    {
        case Main:
            if(byteRx==MainTrailer) status++;
            else status=0;
            break;

        case Information:
            if(byte_command!=BufferSize)
            {
            bufferRx[byte_command]=byteRx;
            byte_command++;
            break;
            }
            else
            {
            status++;
            byte_command=0;
            }

        case End:
            if(byteRx==EndTrailer){
                status=0;
                byte_command=0;
                isComplete=true;
            }
            else status=0;
            break;

    }
}

void SerialManager::ReturnInfo(unsigned char *buffer)
{
    for(int i=0; i < BufferSize; ++i){
        buffer[i]=bufferRx[i];
    }
}