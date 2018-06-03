#ifndef SerialManager_H
#define SerialManager_H

///Trailer
#define MainTrailer 0x2F
#define EndTrailer  0x5C

///Switch  case 
#define Main 0
#define Information 1
#define End 2

#define BufferSize 10

class SerialManager
{

public:
    SerialManager();
    void Process(unsigned char  byteRx);
    bool isComplete;
    void ReturnInfo(unsigned char *buffer);
    unsigned char bufferRx[BufferSize];

private:
    unsigned char checkSum;
    int status;
    int byte_command;

};
#endif