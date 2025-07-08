#include "unity.h"
#include "hash/hash.h"

// This function is run before each test, but is not needed for these tests.
void setUp(void) {}

// This function is run after each test, but is not needed for these tests.
void tearDown(void) {}

// Tests the djb2 hash function against known, pre-calculated values.
void test_djb2_known_values(void) {
    // Test empty string
    TEST_ASSERT_EQUAL_UINT64(5381UL, HASH.djb2(""));

    // Test known string
    TEST_ASSERT_EQUAL_UINT64(210714636441UL, HASH.djb2("hello"));

    // Test another known string
    TEST_ASSERT_EQUAL_UINT64(210732791149UL, HASH.djb2("world"));

    // Test case sensitivity
    TEST_ASSERT_NOT_EQUAL(HASH.djb2("hello"), HASH.djb2("Hello"));

    // Test long string
    const char *long_str = "The quick brown fox jumps over the lazy dog";
    TEST_ASSERT_EQUAL_UINT64(3950289020261251294UL, HASH.djb2(long_str));
}

// This test is intentionally ignored, as the function is not designed to handle NULL.
void test_djb2_null_pointer(void) {
    TEST_IGNORE_MESSAGE("The function is not designed to handle NULL pointers.");
}

// The main function that runs all the tests.
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_djb2_known_values);
    RUN_TEST(test_djb2_null_pointer);
    
    return UNITY_END();
}