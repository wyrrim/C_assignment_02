/**
 * @file cbuffer.c
 * @author Sergey Kalinichenko (sergey.kalinichenko@yaelev.se)
 * @brief C assignment 02
 * 
 * 
 * @version 0.2
 * @date 2021-03-21
 * 
 * @copyright Copyright (c) 2021
 * Implement a circular buffer according to the requirements below:
 * 1.	The buffer is an array of type uint8_t
 * 2.	The module shall have an initialization function
 * 3.	It shall be possible to have a function to read data from the buffer
 * 4.	It shall be possible to have a function to peek data in the buffer
 * 5.	It shall be possible to have a function to write data to the buffer
 * 6.	It shall be possible to have a function to return the number of avaiable elements in the buffer
 * 7.	It shall be possible to have a function to check if the buffer is full or not
 * 8.	Don’t change the header file or include extra libraries in the implementation file of the module.
 * 
 * For VG, all the requirements shall be fulfilled and the module shall be tested fully.
 *
 */
#include "cbuffer.h"

#if ((BUFFER_SIZE < 8U) || (BUFFER_SIZE > 32U))
#error BUFFER_SIZE should be an integer between 8 and 32
#endif

static bool full = false;
static uint8_t tail = 0U;
static uint8_t head = 0U;
static uint8_t buffer[BUFFER_SIZE] = {0};

void cbuffer_init(void)
{
    tail = head;
    full = false;
}

void cbuffer_write(uint8_t value)
{
    buffer[tail] = value;
    tail = (tail + 1) % BUFFER_SIZE;
    if (full)
    {
        head = tail;
    }
    else if (tail == head)
    {
        full = true;
    }
}

uint8_t cbuffer_read(void)
{
    uint8_t value = 0;

    if (cbuffer_available())
    {
        value = buffer[head];
        full = false;
        head = (head + 1) % BUFFER_SIZE;
    }

    return value;
}

bool cbuffer_isfull(void)
{
    return full;
}

uint8_t cbuffer_peek(void)
{
    return cbuffer_available() ? buffer[head] : 0;
}

uint8_t cbuffer_available(void)
{
    return full ? BUFFER_SIZE : (tail - head) % BUFFER_SIZE;
}
