
#include <stdlib.h>
#include <Arduino.h>
//#include <Arduino.h>
#define ROWSIZE 38
#define BUFFER_SIZE 15 
class DataBuffer {
private:
//uint8_t buffer[5][38]; // max size definieren
    uint8_t buffer[BUFFER_SIZE][ROWSIZE];
    size_t row_load;
    size_t col_load;
	size_t head;
	size_t tail;
	size_t max; //of the buffer
	bool full;
public:
    DataBuffer(int);
    uint32_t getRowBufferSize();
    uint32_t getColBufferSize();
    uint32_t getHead();
    uint32_t getTail();
    uint8_t* getData();
    bool appendData(uint8_t);
    bool isFull();
    void resetBuffer();

};