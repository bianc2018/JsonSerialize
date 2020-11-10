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
        & JSON_NI_SERIALIZATION_KV_N(t, long_val)
        & JSON_NI_SERIALIZATION_KV_N(t, llong_val)
        & JSON_NI_SERIALIZATION_KV_N(t, ullong_val)
        & JSON_NI_SERIALIZATION_KV_N(t, string_val);
}

// case1
TEST(Base, JsonSerializeFormJson)
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
    ASSERT_EQ(JsonSerializeFormJson(json, t1), true);
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
}

// case
TEST(Base, JsonSerializeToJson)
{
    //转换为json，然后反过来
    Test1 t1;
    t1.bool_val = false;
    t1.char_val = 'B';
    t1.double_val = 0.002;
    t1.int_val = 233;
    t1.llong_val = 23333;
    t1.long_val = 2233;
    t1.string_val = "2333";
    t1.uchar_val = 0;
    t1.uint_val = 100;
    t1.ullong_val = 100000;
    std::string json;
    ASSERT_EQ(JsonSerializeToJson(json, t1), true);
    ASSERT_EQ(json.empty(), false);

    //转换成JSON2
    Test1 t2;
    ASSERT_EQ(JsonSerializeFormJson(json, t2), true);
    
    EXPECT_EQ(t1.bool_val, t2.bool_val);
    EXPECT_EQ(t1.char_val, t2.char_val);
    EXPECT_EQ(t1.double_val, t2.double_val);
    EXPECT_EQ(t1.int_val, t2.int_val);
    EXPECT_EQ(t1.llong_val, t2.llong_val);
    EXPECT_EQ(t1.long_val, t2.long_val);
    EXPECT_EQ(t1.string_val, t2.string_val);
    EXPECT_EQ(t1.uchar_val, t2.uchar_val);
    EXPECT_EQ(t1.uint_val, t2.uint_val);
    EXPECT_EQ(t1.ullong_val, t2.ullong_val);
}

// case
TEST(Base, JsonSerializeJsonFile)
{
    const std::string jsonfilepath = "JsonSerializeJsonFile.json";
    //转换为json，然后反过来
    Test1 t1;
    t1.bool_val = false;
    t1.char_val = 'B';
    t1.double_val = 0.002;
    t1.int_val = 233;
    t1.llong_val = 23333;
    t1.long_val = 2233;
    t1.string_val = "2333";
    t1.uchar_val = 0;
    t1.uint_val = 100;
    t1.ullong_val = 100000;
    ASSERT_EQ(JsonSerializeToJsonFile(jsonfilepath, t1), true);

    //转换成JSON2
    Test1 t2;
    ASSERT_EQ(JsonSerializeFormJsonFile(jsonfilepath, t2), true);

    EXPECT_EQ(t1.bool_val, t2.bool_val);
    EXPECT_EQ(t1.char_val, t2.char_val);
    EXPECT_EQ(t1.double_val, t2.double_val);
    EXPECT_EQ(t1.int_val, t2.int_val);
    EXPECT_EQ(t1.llong_val, t2.llong_val);
    EXPECT_EQ(t1.long_val, t2.long_val);
    EXPECT_EQ(t1.string_val, t2.string_val);
    EXPECT_EQ(t1.uchar_val, t2.uchar_val);
    EXPECT_EQ(t1.uint_val, t2.uint_val);
    EXPECT_EQ(t1.ullong_val, t2.ullong_val);
}