#include <unistd.h>
#include "i2c.h"

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
        m_ready = 0;
    }
}

int I2C::openBus(uint8_t slaveAdr)
{
    m_adr = slaveAdr;
}

int I2C::closeBus()
{
    m_adr = 0;
    return m_adr;
}

int I2C::isReady()
{
    if (m_ready & m_adr)
        return 1;
    else
        return 0;
}

int I2C::getCog()
{
    return m_par.cog;
}

int I2C::getStatus()
{
    return m_par.mailbox.sts;
}


int I2C::devPresent(uint8_t addr)
{
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


int I2C::txByte(uint8_t bt)
{
    return txByte(-1, bt);
}


int I2C::txByte(int32_t reg, uint8_t bt)
{
    if(m_ready)
    	return tx(reg, &bt, 1);
    else
        return -1;
}


int I2C::txWord(uint16_t wd)
{
	return txWord(-1, wd);
}


int I2C::txWord(int32_t reg, uint16_t wd)
{
    uint8_t  bytes[2];
    
    bytes[1] = wd;
    wd >>= 8;
    bytes[0] = wd;  

    if(m_ready)
    	return tx(reg, bytes, 2);
    else
        return -1;
}


/*
int I2C::txLong(uint8_t addr, uint32_t& lg)
{	// Transmit a single long of data to the bus at the register value
	// specified.  Transaction is |ST|WRADR|BYTE3|BYTE2|BYTE1|BYTE0|SP|.
	//
	//     [addr] - 7 bit I2C address			[lg] - Long of data to transmit
	//
    return TXLong(addr, 0, 0, lg);
}

int I2C::txLong(int32_t reg, uint8_t rcnt, uint32_t lg)
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
}*/

int I2C::tx(uint8_t *buf, int count)
{
    return tx(-1, buf, count);
}


int I2C::tx(int32_t reg, uint8_t *buf, int count)
{
    int rcnt = getRegByteCount(reg);
    
    if(m_ready)
    {
    	WaitForIdle();
        m_par.mailbox.cmd       = I2C_CMD_LOCKED;
        m_par.mailbox.hdr       = (m_adr << 1);
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


int8_t I2C::rxByte()
{	// Read a single byte of data from the bus.  No register address.
  
    return rxByte(-1);
}


int8_t I2C::rxByte(int32_t reg)
{	// Read a single byte of data from the bus at the register specified.
	// Register can be 1 or 2 bytes [rcnt].
	// Transaction is |ST|WRADR|REGVAL|RS|RDADR|BYTE|SP|.

    uint8_t bt;

    if(m_ready)
    {
    	rx(reg, &bt, 1);

        if (m_par.mailbox.sts == I2C_OK)
            return bt;
        else
            return -1;

    }

    return -1;
}


int16_t I2C::rxWord()
{
    return rxWord(-1);
}


int16_t I2C::rxWord(int32_t reg)
{// Read a single word of data from the bus at the register specified.  
 // Register can be 1 or 2 bytes -> [rcnt].  Default is 1 byte. 
 // Transaction is |ST|WRADR|REGVAL|RS|RDADR|BYTE1|BYTE0|SP|.
 
    uint8_t  bytes[2];
    uint16_t wd = 0;

    if(m_ready)
    {
    	rx(reg, bytes, 2);

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


/*int I2C::RXLong(uint8_t addr)
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
}*/


int I2C::rx(uint8_t* bytes, int count)
{
    return rx(-1, bytes, count);
}


int I2C::rx(int32_t reg, uint8_t *buf, int count)
{
    int rcnt = getRegByteCount(reg);
    
    if(m_ready)
    {
    	WaitForIdle();
        m_par.mailbox.cmd       = I2C_CMD_LOCKED;
        m_par.mailbox.hdr       = (m_adr << 1);
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


int I2C::getRegByteCount(int32_t reg)
{
    if (reg < 0)
        return 0;
    
    if (reg > 0xFFFFFF)
        return 4;
    if (reg > 0xFFFF)
        return 3;
    if (reg > 0xFF)
        return 2;
    
    return 1;
}




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
