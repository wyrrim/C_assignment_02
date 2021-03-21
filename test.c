/**
 * @file test.c
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief For VG, all the requirements shall be fulfilled and the module shall be tested fully.
 *        Some suggestions for testing:
  *           1) Test the initialization of the buffer
 *            2) Test the empty/full buffer case
 *            3) Test read/peek/write when the buffer is empty
 *            4) Test read/peek/write when the buffer is full (an overwritten occurres when we write to a full buffer)
 *            5) Test read/peek/write when the buffer is not empty and full
 * 
 * @version 0.1
 * @date 2021-03-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <unity.h>
#include "cbuffer.h"

#define HALF_OF_BUFFER_SIZE (BUFFER_SIZE / 2U)
#define QUARTER_OF_BUFFER_SIZE (HALF_OF_BUFFER_SIZE / 2U)
#define SOME_VAL_1 11U
#define SOME_VAL_2 21U
#define SOME_VAL_3 31U
#define SOME_VAL_4 41U

void setUp()
{
}

void tearDown()
{
}

void test_init(void) // Test the initialization of the buffer (incl. 'empty', 'non-empty' and 'full' cases)
{
    // "start" init -- EMPTY BUFFER
    cbuffer_init();
    TEST_ASSERT_FALSE(cbuffer_isfull());              // AFTER INIT: 1) buffer is NOT full;
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_available()); //             2) number of available elem. is zero
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_read());      //             3) attempt to read returns zero
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_peek());      //             4) attempt to peek returns zero

    // HALF-FULL BUFFER
    for (uint8_t i = 0U; i < HALF_OF_BUFFER_SIZE; i++) // write: fill half of the buffer
    {                                                  //   (full == false, head != tail)
        cbuffer_write(i + SOME_VAL_1);                 //
    }                                                  //
    cbuffer_init();
    TEST_ASSERT_FALSE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_available());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_peek());

    // FULL BUFFER
    for (uint8_t i = 0U; i < BUFFER_SIZE + HALF_OF_BUFFER_SIZE; i++) // fill the buffer with overwriting
    {                                                                //   (full == true, head == tail)
        cbuffer_write(i + SOME_VAL_1);                               //
    }                                                                //
    cbuffer_init();
    TEST_ASSERT_FALSE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_available());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_peek());

    // NEITHER EMPTY NOR FULL BUFFER
    for (uint8_t i = 0U; i < QUARTER_OF_BUFFER_SIZE; i++) // read something from the full buffer
    {                                                     //   to make head != tail, full == false
        (void)cbuffer_read();                             //
    }                                                     //
    cbuffer_init();
    TEST_ASSERT_FALSE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_available());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_peek());

    // EMPTY BUFFER
    while (cbuffer_available()) // read till the buffer is empty
    {                           //
        (void)cbuffer_read();   //
    }                           //
    cbuffer_init();
    TEST_ASSERT_FALSE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_available());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_peek());

    // "double" init (init after init) -- EMPTY BUFFER
    cbuffer_init();
    TEST_ASSERT_FALSE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_available());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_read());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_peek());
}

void test_empty_full(void) // Test number of accessible elem. in the buffer
{
    // EMPTY BUFFER
    cbuffer_init();
    TEST_ASSERT_FALSE(cbuffer_isfull());              // SET OF TESTS OF EMPTY/FULL: 1) is buffer full
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_available()); //                             2) how many elem. are available

    // HALF-FULL BUFFER
    for (uint8_t i = 0U; i < HALF_OF_BUFFER_SIZE; i++) // write: fill half of the buffer
    {                                                  //
        cbuffer_write(i + SOME_VAL_1);                 //
    }                                                  //
    TEST_ASSERT_FALSE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(HALF_OF_BUFFER_SIZE, cbuffer_available());

    // 1 STEP BEFORE FULL BUFFER
    for (uint8_t i = 0U; i < HALF_OF_BUFFER_SIZE - 1U; i++) // write till 1 step before the buffer is full
    {                                                       //
        cbuffer_write(i + SOME_VAL_2);                      //
    }                                                       //
    TEST_ASSERT_FALSE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(BUFFER_SIZE - 1U, cbuffer_available());

    // BUFFER JUST BECOMES FULL
    cbuffer_write(SOME_VAL_3); // write: the buffer becomes full
    TEST_ASSERT_TRUE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(BUFFER_SIZE, cbuffer_available());

    // FULL BUFFER (OVERWRITTEN)
    for (uint8_t i = 0U; i < HALF_OF_BUFFER_SIZE; i++) // write: overwrite buffer
    {                                                  //
        cbuffer_write(i + SOME_VAL_4);                 //
    }                                                  //
    TEST_ASSERT_TRUE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(BUFFER_SIZE, cbuffer_available());

    // NEITHER EMPTY NOR FULL BUFFER
    for (uint8_t i = 0U; i < QUARTER_OF_BUFFER_SIZE; i++) // read something from the full buffer
    {                                                     //
        (void)cbuffer_read();                             //
    }                                                     //
    TEST_ASSERT_FALSE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(BUFFER_SIZE - QUARTER_OF_BUFFER_SIZE, cbuffer_available());

    // 1 STEP BEFORE EMPTY BUFFER
    while (cbuffer_available() > 1U) // read till 1 step before the buffer is empty
    {                                //
        (void)cbuffer_read();        //
    }                                //
    TEST_ASSERT_FALSE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(1U, cbuffer_available());

    // BUFFER JUST BECOMES EMPTY
    (void)cbuffer_read(); // read: the buffer becomes empty
    TEST_ASSERT_FALSE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_available());

    // BUFFER REMAINS EMPTY AFTER SOME MORE ATTEMPTS TO PEEK/READ
    (void)cbuffer_read(); // read: the buffer remains empty
    (void)cbuffer_read();
    TEST_ASSERT_FALSE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_available());
    (void)cbuffer_peek(); // peek: the buffer remains empty
    (void)cbuffer_peek();
    TEST_ASSERT_FALSE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_available());
}

void test_op_when_empty(void) // Test read/peek/write when the buffer is empty
{
    // BUFFER BECOMES EMPTY AFTER INIT
    cbuffer_init();
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_peek());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_read());
    cbuffer_write(SOME_VAL_1);
    TEST_ASSERT_EQUAL_UINT8(SOME_VAL_1, cbuffer_peek());

    // BUFFER BECOMES EMPTY AFTER READ
    (void)cbuffer_read();
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_peek());
    TEST_ASSERT_EQUAL_UINT8(0U, cbuffer_read());
    cbuffer_write(SOME_VAL_1);
    TEST_ASSERT_EQUAL_UINT8(SOME_VAL_1, cbuffer_peek());
}

void test_op_when_full(void) // Test read/peek/write when the buffer is full
{
    // BUFFER JUST BECOMES FULL (NO OVERWRITING)
    uint8_t i = SOME_VAL_1;
    while (!cbuffer_isfull()) // write till the buffer is full
    {
        cbuffer_write(i++);
    }
    TEST_ASSERT_EQUAL_UINT8(SOME_VAL_1, cbuffer_peek());
    TEST_ASSERT_EQUAL_UINT8(SOME_VAL_1, cbuffer_read()); // 1 place in buffer becomes empty
    cbuffer_write(SOME_VAL_2);                           // filling the buffer

    TEST_ASSERT_TRUE(cbuffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(8U, cbuffer_available());

    TEST_ASSERT_EQUAL_UINT8(SOME_VAL_1 + 1U, cbuffer_read()); // no values are lost

    //TEST_ASSERT_EQUAL_UINT8(8U, cbuffer_available());

    //TEST_ASSERT_EQUAL_UINT8(SOME_VAL_1 + 2U, cbuffer_read()); // no values are lost
    //cbuffer_write(SOME_VAL_3);                               // writing to full buffer (overwriting),
    //TEST_ASSERT_EQUAL_UINT8(SOME_VAL_1 + 3, cbuffer_peek()); //   so (SOME_VAL_1 + 2) is lost
    //TEST_ASSERT_EQUAL_UINT8(SOME_VAL_1 + 3, cbuffer_read());
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_init);
    RUN_TEST(test_empty_full);
    RUN_TEST(test_op_when_empty);
    RUN_TEST(test_op_when_full);

    return UNITY_END();
}
