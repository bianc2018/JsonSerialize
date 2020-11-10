#include "JsonSerialize.hpp"
#include "cJSON.h"
#include <gtest/gtest.h>
struct Test1
{
    bool bool_val = false;
    char char_val = 0;
    unsigned char uchar_val = 0;
    int int_val=0;
    unsigned int uint_val = 0;
    double double_val = 0;
    //size_t size_t_val = 0;
    long long_val = 0;
    long long llong_val = 0;
    unsigned long long ullong_val = 0;
    std::string string_val;
};
template<typename Archive>
void Serialize(Archive& ar, Test1& t)
{
    ar& JSON_NI_SERIALIZATION_KV(t, bool_val,true)
        & JSON_NI_SERIALIZATION_KV_N(t, char_val)
        & JSON_NI_SERIALIZATION_KV_N(t, uchar_val)
        & JSON_NI_SERIALIZATION_KV_N(t, int_val)
        & JSON_NI_SERIALIZATION_KV_N(t, uint_val)
        & JSON_NI_SERIALIZATION_KV_N(t, double_val)
        //& JSON_NI_SERIALIZATION_KV_N(t, size_t_val)
        & JSON_NI_SERIALIZATION_KV_N(t, long_val)
        & JSON_NI_SERIALIZATION_KV_N(t, llong_val)
        & JSON_NI_SERIALIZATION_KV_N(t, ullong_val)
        & JSON_NI_SERIALIZATION_KV_N(t, string_val);
}
// case1
TEST(Base, js_form_json)
{
    Test1 t1;
    std::string json = "{"
        "\"bool_val\":true,"
        "\"char_val\":65,"
        "\"uchar_val\":1,"
        "\"int_val\":1,"
        "\"uint_val\":1,"
        "\"double_val\":1.100001,"
        "\"long_val\":1,"
        "\"llong_val\":1,"
        "\"ullong_val\":1,"
        "\"string_val\":\"string\""
        "}";
    ASSERT_EQ(js_form_json(json, t1), true);
    EXPECT_EQ(t1.bool_val, true);
    EXPECT_EQ(t1.char_val, 'A');
    EXPECT_EQ(t1.uchar_val, 1);
    EXPECT_EQ(t1.int_val, 1);
    EXPECT_EQ(t1.uint_val, 1);
    EXPECT_EQ(t1.double_val, 1.100001);
  //  EXPECT_EQ(t1.size_t_val, 1);
    EXPECT_EQ(t1.long_val, 1);
    EXPECT_EQ(t1.llong_val, 1);
    EXPECT_EQ(t1.ullong_val, 1);
    EXPECT_EQ(t1.string_val, "string");

    std::string json_1;
    ASSERT_EQ(js_to_json(json_1, t1), true);
    printf("js_to_json :%s\n", json_1.c_str());
}