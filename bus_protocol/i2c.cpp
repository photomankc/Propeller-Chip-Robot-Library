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



#include <unistd.h>
#include "I2C.h"

extern uint32_t _load_start_I2CDriver_cog[];
extern uint32_t _load_stop_I2CDriver_cog[];
extern uint8_t binary_i2c_driver_image_dat_start[];

#define pollDelay()    (usleep(30))

I2C::I2C(int scl, int sda, int freq)
{  
	// only allow speeds up to 400khz for now
	if (freq > 400000)
		freq = 400000;

    //I2C_INIT init;
    m_par.cog            		= -1;
    m_par.init.scl       		= scl;
    m_par.init.sda       		= sda;
    m_par.init.ticks_per_cycle 	= (CLKFREQ / freq)-25;
    m_par.init.mailbox   		= &m_par.mailbox;
    m_par.mailbox.cmd    		= I2C_CMD_INIT;
    
    // Start the COG according to the method needed for LMM/XMM memory models
    #if defined(__PROPELLER_XMMC__) || defined(__PROPELLER_XMM__)
        int size = _load_stop_I2CDriver_cog - _load_start_I2CDriver_cog;
        unsigned int cogbuffer[size];
        // memcpy does bytes, so we copy numbytes * 4 = num ints.      
        memcpy(cogbuffer, _load_start_I2CDriver_cog, size<<2);
    #else
        int *cogbuffer = (int*)_load_start_I2CDriver_cog;
    #endif
    
    m_par.cog = cognew(cogbuffer, &m_par.init);

    volatile int n;
    while (m_par.mailbox.cmd != I2C_CMD_IDLE)
    {
        if (n > 127)
        {
            m_ready = 0;
            return;          // Timeout waiting after ~3.8ms
        }
        pollDelay();        // Wait 30 uSecs.
        n++;
    }

    m_ready = 1;
}

I2C::~I2C()
{
    if (m_par.cog >= 0)
    {
    	cogstop(m_par.cog);
    	m_par.cog = -1;
    }
}

int I2C::GetCog()
{	// Returns the current COG that is driving the bus.  0-7 are valid
	// values. All others indicate the COG is not started.
	//
    return m_par.cog;
}

int I2C::GetStatus()
{	// Return the status value from the last I2C bus transaction.
	//
    return m_par.mailbox.sts;
}






///////////////////////////////////////////////////////////////////////////////
// Public Members - Transmit Functions
//

int I2C::DevPresent(uint8_t addr)
{	// Write out the device write address to the bus with no register and
	// no data bytes.  If there is an ACK we know the device is present
	// on the bus.  If no ACK then it may not be present or may be busy.
	//
	//     [addr] - 7 bit I2C address
	//
    if(m_ready)
    {
    	WaitForIdle();
        m_par.mailbox.hdr   = (addr << 1);
        m_par.mailbox.count = 0;
        m_par.mailbox.reg_count = 0;
        m_par.mailbox.cmd   = I2C_CMD_SEND;
        WaitForIdle();

        return m_par.mailbox.sts == I2C_OK ? 1 : 0;
    }
    else
        return 0;
}

int I2C::TXByte(uint8_t addr, uint8_t &bt)
{	// Transmit a single byte of data to the bus.  No register address.
	// Transaction is |ST|WRADR|BYTE|SP|.
	//
	//    [addr] - 7 bit I2C address   	 		[bt] - Byte of data to transmit
	//
    return TXByte(addr, 0, 0, bt);
}

int I2C::TXByte(uint8_t addr, uint16_t reg, uint8_t rcnt, uint8_t bt)
{	// Transmite a single byte of data to the bus at the register value
	// specified.  Transaction is |ST|WRADR|REGVAL|BYTE|SP|.
	//
	//     [addr] - 7 bit I2C address    		[reg] - Register value to transmit
	//	   [rcnt] - 1|2 register byte count 	[bt]  - Byte of data to transmit
	//
    if(m_ready)
    	return TXBuf(addr, &bt, 1, reg, rcnt);
    else
        return -1;
}

 int I2C::TXWord(uint8_t addr, uint16_t &wd)
 {	// Transmit a single word of data to the bus
	// Transaction is |ST|WRADR|BYTE1|BYTE0|SP|.
	//
	//     [addr] - 7 bit I2C address			[wd] - Word of data to transmit
	//
	return TXWord(addr, 0, 0, wd);
}

int I2C::TXWord(uint8_t addr, uint16_t reg, uint8_t rcnt, uint16_t wd)
{	// Transmit a single word of data to the bus at the register value
	// specified.  Transaction is |ST|WRADR|REGVAL|BYTE1|BYTE0|SP|.
	//
	//     [addr] - 7 bit I2C address    		[reg] - Register value to transmit
	//	   [rcnt] - 1|2 register byte count 	[wd]  - Word of data to transmit
	//
    uint8_t  bytes[2];
    
    bytes[1] = wd;
    wd >>= 8;
    bytes[0] = wd;  

    if(m_ready)
    	return TXBuf(addr, bytes, 2, reg, rcnt);
    else
        return -1;
}

 int I2C::TXLong(uint8_t addr, uint32_t &lg)
{	// Transmit a single long of data to the bus at the register value
	// specified.  Transaction is |ST|WRADR|BYTE3|BYTE2|BYTE1|BYTE0|SP|.
	//
	//     [addr] - 7 bit I2C address			[lg] - Long of data to transmit
	//
    return TXLong(addr, 0, 0, lg);
}

int I2C::TXLong(uint8_t addr, uint16_t reg, uint8_t rcnt, uint32_t lg)
{	// Transmit a single long of data to the bus at the register value
	// specified.  Transaction is |ST|WRADR|REGVAL|BYTE3|BYTE2|BYTE1|BYTE0|SP|.
	//
	//     [addr] - 7 bit I2C address    		[reg] - Register value to transmit
	//	   [rcnt] - 1|2 register byte count 	[lg]  - Word of data to transmit
	//
    uint8_t  bytes[4];
    
    for(int i=3; i>=0; i--)
    {
        bytes[i] = lg;
        lg >>= 8;
    } 
    if(m_ready)
    	return TXBuf(addr, bytes, 4, reg, rcnt);
    else
        return -1;
}

int I2C::TXBuf(uint8_t addr, uint8_t *buf, uint8_t count)
{	// Transmit the bytes contained in the buffer to the bus.  No register value is used.
	// Transaction is |ST|WRADR|BUF[0]|...|BUF[CNT-1]|SP|.
	//
	//	  [addr] - 7 bit I2C address			[buf] - pointer to the buffer to be used.
	//	  [count] - Number of bytes to TX
	//
    return TXBuf(addr, buf, count, 0, 0);
}

int I2C::TXBuf(uint8_t addr, uint8_t *buf, uint8_t count, uint16_t reg, uint8_t rcnt=1)
{	// Transmit the bytes contained in the buffer to the bus at the specified register value.
	// Transaction is |ST|WRADR|REGVAL|BUF[0]|...|BUF[CNT-1]|SP|.
	//
	//	  [addr] - 7 bit I2C address			[buf] - pointer to the buffer to be used.
	//	  [count] - Number of bytes to TX		[reg] - Register value to write to
	//	  [rcnt]  - 0|1|2 Register bytes
	//
    if(m_ready)
    {
    	WaitForIdle();
        m_par.mailbox.cmd       = I2C_CMD_LOCKED;
        m_par.mailbox.hdr       = (addr << 1);
        m_par.mailbox.buffer    = buf;
        m_par.mailbox.count     = count;
        m_par.mailbox.reg       = reg;
        m_par.mailbox.reg_count = rcnt;
        m_par.mailbox.cmd       = I2C_CMD_SEND;
        WaitForIdle();

        return m_par.mailbox.sts == I2C_OK ? 0 : -1;
    }
    else
        return -1;
}





///////////////////////////////////////////////////////////////////////////////
// Public Members - Receive Functions
//

int I2C::RXByte(uint8_t addr)
{	// Read a single byte of data from the bus.  No register address.
  
    return RXByte(addr, 0, 0);
}

int I2C::RXByte(uint8_t addr, uint16_t reg, uint8_t rcnt=1)
{	// Read a single byte of data from the bus at the register specified.
	// Register can be 1 or 2 bytes [rcnt].
	// Transaction is |ST|WRADR|REGVAL|RS|RDADR|BYTE|SP|.

    uint8_t bt;

    if(m_ready)
    {
    	RXBuf(addr, &bt, 1, reg, rcnt);

        if (m_par.mailbox.sts == I2C_OK)
            return bt;
        else
            return -1;

    }

    return -1;
}

int I2C::RXWord(uint8_t addr)
{
    return RXWord(addr, 0, 0);
}

int I2C::RXWord(uint8_t addr, uint16_t reg, uint8_t rcnt=1)
{// Read a single word of data from the bus at the register specified.  
 // Register can be 1 or 2 bytes -> [rcnt].  Default is 1 byte. 
 // Transaction is |ST|WRADR|REGVAL|RS|RDADR|BYTE1|BYTE0|SP|.
 
    uint8_t  bytes[2];
    uint16_t wd = 0;

    if(m_ready)
    {
    	RXBuf(addr, bytes, 2, reg, rcnt);

        if (m_par.mailbox.sts == I2C_OK)
        {
            wd = bytes[0];
            wd <<= 8;
            wd |= bytes[1];
            return wd;
        }
        else
        	return -1;

    }

    return -1;
}

int I2C::RXLong(uint8_t addr)
{
    return RXLong(addr, 0, 0);
}

int I2C::RXLong(uint8_t addr, uint16_t reg, uint8_t rcnt=1)
{// Read a single long of data from the bus at the register specified.  
 // Register can be 1 or 2 bytes -> [rcnt].  Default is 1 byte. 
 // Transaction is |ST|WRADR|REGVAL|RS|RDADR|BYTE3|BYTE2|BYTE1|BYTE0|SP|.
  
    uint8_t  bytes[4];
    uint32_t lg = 0;

    if(m_ready)
    {
    	RXBuf(addr, bytes, 2, reg, rcnt);

        if (m_par.mailbox.sts == I2C_OK)
        {
            lg |= bytes[0];
            lg <<= 8;
            lg |= bytes[1];
            lg <<= 8;
            lg |= bytes[2];
            lg <<= 8;
            lg |= bytes[3];

            return lg;
        }
        else
            return -1;
    }

    return -1;
}

int I2C::RXBuf(uint8_t addr, uint8_t *buf, uint8_t count)
{
    return RXBuf(addr, buf, count, 0, 0);
}

int I2C::RXBuf(uint8_t addr, uint8_t *buf, uint8_t count, uint16_t reg, uint8_t rcnt=1)
{
    if(m_ready)
    {
    	WaitForIdle();
        m_par.mailbox.cmd       = I2C_CMD_LOCKED;
        m_par.mailbox.hdr       = (addr << 1);
        m_par.mailbox.buffer    = buf;
        m_par.mailbox.count     = count;
        m_par.mailbox.reg       = reg;
        m_par.mailbox.reg_count = rcnt;
        m_par.mailbox.cmd       = I2C_CMD_RECEIVE;
        WaitForIdle();

        return m_par.mailbox.sts == I2C_OK ? 0 : -1;
    }
    else
        return -1;
}





///////////////////////////////////////////////////////////////////////////////
// Private Members
//

void I2C::WaitForIdle()
{
	while (m_par.mailbox.cmd != I2C_CMD_IDLE)
		usleep(30);
}




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
