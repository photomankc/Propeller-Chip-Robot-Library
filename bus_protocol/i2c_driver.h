/* 
 * Definitions for supporting data structures to COGC driver.
 */

#ifndef __I2C_DRIVER_H__
#define __I2C_DRIVER_H__

#include <propeller.h>

#define I2C_READ        1
#define I2C_WRITE       0

// I2C Commands
typedef enum I2C_CMD
{
    I2C_CMD_IDLE,           // Bus quiet and ready for new command
    I2C_CMD_INIT,           // Initialize the bus
    I2C_CMD_LOCKED,         // Marks the bus as locked but not yet active
    I2C_CMD_SEND,           // Send data bytes to the bus at a register address
    I2C_CMD_RECEIVE         // Recieve data bytes from the bus at a register address
} I2C_CMD;


// I2C status return types
typedef enum I2C_RESULT
{
    I2C_OK = 0,				// Transmission completed sucessfully
    I2C_ERR_UNKNOWN_CMD,	// Last command was not valid
    I2C_ERR_SEND_HDR,		// Address or Register as not acknowleged
    I2C_ERR_SEND,			// Data transmitted was not acknowleged
    I2C_ERR_RECEIVE_HDR,	// Address or register is not acknowleged
    I2C_ERR_RECEIVE
} I2C_RESULT;

 
///////////////////////////////////////////////////////////////////////////////////////
// I2C_MAILBOX structure - 	Groups parameters used to pass commands to the I2C cog for
//							control and data transmission.
//
typedef struct I2C_MAILBOX
{
    volatile uint32_t cmd;       // Primative I2C command (SEE I2C_CMD Enum)
    volatile uint32_t sts;       // Last command status (SEE I2C_RESULT Enum)
    volatile uint8_t  hdr;       // I2C address header (The 8bit WRITE address of the I2C device)
    volatile uint16_t reg;       // Register address for register read/write (16 bit max)
    volatile uint8_t  reg_count; // Number of register bytes to send   (1|2)
    volatile uint8_t  count;     // Number of bytes to be sent/recv'd
    uint8_t*          buffer;    // Pointer to data to be sent/recv'd
} I2C_MAILBOX;


//////////////////////////////////////////////////////////////////////////////////////
// Initialization structure - 	Groups parameters used to setup the operation
//   							of the I2C cog
typedef struct I2C_INIT
{
    volatile I2C_MAILBOX *mailbox;  // Pointer to the cogs HUB mailbox
    uint32_t scl;                   // SCL IO Pin
    uint32_t sda;                   // SDA IO Pin
    uint32_t ticks_per_cycle;       // Clock delay time
} I2C_INIT;


//////////////////////////////////////////////////////////////////////////////////////
// PAR_S Structure -	Creates the reserved memory locations for the mailbox, the init
//						structure, and a small stack for the COG.  This structure must
//						remain in scope for the life of the COG attached to it.
typedef struct PAR_S
{
    uint32_t stack[8];		// COG Execution stack
    I2C_INIT init; 			// COG Initialization Parameters
    I2C_MAILBOX mailbox;  	// COG Communication mailbox
    int32_t cog; 			// COG Number used for this bus (if started)
} PAR_S;


#endif

/*
 Copyright (C) 2013 Kyle Crane
 Based on work by David Betz
 
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
