#include "data_buffer.h"

    DataBuffer::DataBuffer(int buffer_size)
    {   
        //buffer[buffer_size][ROWSIZE];
        row_load = 0;
        col_load = 0;
        head = 0;
        tail = 0;
        max  = buffer_size;
    }

    bool DataBuffer::appendData(uint8_t data_chunk)  // 38 data chunks 
    {         
        if(col_load == max){
            return false;
            }
            buffer[tail][row_load] = data_chunk;
            row_load++;
            if (row_load == ROWSIZE)
            {
                row_load = 0;
                col_load++;
                tail++;
                if (tail > max)
                    {
                    tail = 0;
                    }
            }
        return true;
    }

    uint8_t* DataBuffer::getData()
    {
        if (col_load == 0)
        {
            return 0;
        }
        int temp_head = head;
        col_load--;
        if (head >= max)
        {
            head = 0;
        }
        else
        {
            head++;
        }
        return buffer[temp_head];
    }

    void DataBuffer::resetBuffer()
    {
        for (int i = 0;i < max; i++)
         for (int j = 0; j < ROWSIZE; j++)
         { buffer[i][j] = 0;}
    }

    uint32_t DataBuffer::getHead()
    {
        return head;
    }

    uint32_t DataBuffer::getTail()
    {
        return tail;
    }

        uint32_t DataBuffer::getRowBufferSize()
    {
        return row_load;
    }
            uint32_t DataBuffer::getColBufferSize()
    {
        return col_load;
    }
    
    bool DataBuffer::isFull()
    {
        if (col_load == max)
            {
                return true;
            }
        else
        {
            return false;
        }        
    }
