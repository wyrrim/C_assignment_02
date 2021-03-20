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
}

void cbuffer_write(uint8_t value)
{
}

uint8_t cbuffer_read(void)
{
}

bool cbuffer_isfull(void)
{
}

uint8_t cbuffer_peek(void)
{
}

uint8_t cbuffer_available(void)
{
}
