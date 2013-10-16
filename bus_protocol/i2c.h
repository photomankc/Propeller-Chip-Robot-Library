#ifndef __I2C_H__
#define __I2C_H__

#include "i_i2c.h"
#include "i2c_driver.h"

class I2C : public I_I2C
{
protected:
    PAR_S   m_par;
    int     m_ready;
    
public:
    I2C(int scl, int sda, int freq = 100000);
    ~I2C();
    
    int openBus(uint8_t slaveAdr);
    int closeBus();
    int isReady();
    int tx(uint8_t* bytes, int count);
    int rx(uint8_t* bytes, int count);
    
    
    int DevPresent(uint8_t addr);
    int GetCog();
    int GetStatus();
    
    int tx(int32_t reg, uint8_t* bytes, int count);
    int txByte(uint8_t bt);
    int txByte(int32_t reg, uint8_t byte);
    int txWord(uint16_t wd);
    int txWord(int32_t reg, uint16_t word);
    
    int         rx(int32_t reg, uint8_t* bytes, int count);
    int8_t      rxByte();
    int8_t      rxByte(int32_t reg);
    int16_t     rxWord();
    int16_t     rxWord(int32_t reg);
    

    
private:
    void WaitForIdle();
};



/*
 Copyright (C) 2013 Kyle Crane
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#endif


