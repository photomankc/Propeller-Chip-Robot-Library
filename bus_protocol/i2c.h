////////////////////////////////////////////////////////////////////////////////////////////
// Class I2C -    Defines an interface to the I2C Bus Driver cog to allow easy access
//                to I2C functions.  This object will start a COG to drive the I2C bus at
//                high speed (100 to 400KHz).
//
//
// (Based on work by David Michael Betz)
//
// Copyright Kyle Crane 2012.  See Bottom for License Terms
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __I2C_BUS_H__
#define __I2C_BUS_H__

#include "I2CDriver.h"

class I2C
{
  protected:
    PAR_S   m_par;
    int     m_ready;
    
  public:
    I2C(int scl, int sda, int freq = 100000);       // Initialize a new I2C bus
    ~I2C();
    int GetCog();                                   // Returns the cog number running the bus
    int DevPresent(uint8_t addr);                   // Returns 0 if device acknowledged it's address -1 if not
    int GetStatus();                                // Returns the status of the last bus operation
    
    int TXByte(uint8_t addr, uint8_t &bt);                             // Send a single byte to the bus 
    int TXByte(uint8_t addr, uint16_t reg, uint8_t rcnt, uint8_t bt);  // Send a single byte to the bus at specific register
    int TXWord(uint8_t addr, uint16_t &wd);                            // Send a single word to the bus 
    int TXWord(uint8_t addr, uint16_t reg, uint8_t rcnt, uint16_t wd); // Send a single word to the bus at specific register
    int TXLong(uint8_t addr, uint32_t &lg);                            // Send a single byte to the bus
    int TXLong(uint8_t addr, uint16_t reg, uint8_t rcnt, uint32_t lg); // Send a single byte to the bus at specific register   
    int TXBuf(uint8_t addr, uint8_t *buf, uint8_t count);              // Send bytes to I2C address from specified buffer
    int TXBuf(uint8_t addr, uint8_t *buf, uint8_t count, uint16_t reg, uint8_t rcnt);
    
    int RXByte(uint8_t addr);                                          // Receive a single byte from the bus
    int RXByte(uint8_t addr, uint16_t reg, uint8_t rcnt);              // Receive a single byte from the bus from specific register
    int RXWord(uint8_t addr);                                          // Receive a single word from the bus
    int RXWord(uint8_t addr, uint16_t reg, uint8_t rcnt);              // Receive a single word from the bus from specific register
    int RXLong(uint8_t addr);                                          // Receive a single byte from the bus
    int RXLong(uint8_t addr, uint16_t reg, uint8_t rcnt);              // Receive a single byte from the bus from specific register
    int RXBuf(uint8_t addr, uint8_t *buf, uint8_t count);              // Load bytes into specified buffer from I2C address
    int RXBuf(uint8_t addr, uint8_t *buf, uint8_t count, uint16_t reg, uint8_t rcnt);

  private:
    void WaitForIdle();
};


#endif


///////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012 David Michael Betz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
//substantial portions of the Software.
///////////////////////////////////////////////////////////////////////////////////////////////////
