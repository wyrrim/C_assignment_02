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
    //for (uint8_t i = 0; i < BUFFER_SIZE; buffer[i++] = 0U)
    //    ;
    head = tail = 0U;
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
    return !cbuffer_available() ? 0 : buffer[head];
}

uint8_t cbuffer_available(void)
{
    return full ? BUFFER_SIZE : (tail - head) % BUFFER_SIZE;
}
