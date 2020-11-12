//Êý×é²âÊÔ
#include "JsonSerialize.hpp"
#include "cJSON.h"
#include <gtest/gtest.h>

struct Char1
{
    char buff[20];

    template<typename ar>
    void SERIALIZE_FUNC(ar& a)
    {
        a& JSON_SERIALIZATION_KV_N(buff);
    }
};

//²âÊÔ»ù±¾µÄ×Ö·û´®ÐòÁÐ»¯¹¦ÄÜ
TEST(Array, CharBase)
{
    Char1 c1 = { "1234567890" };
    std::string json;
    ASSERT_EQ(JsonSerializeToJson(json, c1), true);

    Char1 c2;
    ASSERT_EQ(JsonSerializeFormJson(json, c2), true);

    EXPECT_STREQ(c1.buff, c2.buff);
}

//²âÊÔ¿Õ×Ö·û´®
TEST(Array, CharEmpty)
{
    Char1 c1 = { "" };
    std::string json;
    ASSERT_EQ(JsonSerializeToJson(json, c1), true);

    Char1 c2;
    ASSERT_EQ(JsonSerializeFormJson(json, c2), true);

    EXPECT_STREQ(c1.buff, c2.buff);
}

//²âÊÔÂúµÄ
TEST(Array, CharFull)
{
    Char1 c1 = { "1234567890123456789" };
    std::string json;
    ASSERT_EQ(JsonSerializeToJson(json, c1), true);

    Char1 c2;
    ASSERT_EQ(JsonSerializeFormJson(json, c2), true);

    EXPECT_STREQ(c1.buff, c2.buff);
}

//×Ö·û´®×ª»»
TEST(Array, CharFormJson10)
{
    Char1 c1 ;
    std::string str(10,'1');
    std::string json = "{\"buff\":\""+str+"\"}";
    
    ASSERT_EQ(JsonSerializeFormJson(json, c1), true);
    EXPECT_STREQ(c1.buff, str.c_str());
}

//×Ö·û´®×ª»»
TEST(Array, CharFormJsonEmpty)
{
    Char1 c1;
    std::string str;
    std::string json = "{\"buff\":\"" + str + "\"}";

    ASSERT_EQ(JsonSerializeFormJson(json, c1), true);
    EXPECT_STREQ(c1.buff, str.c_str());
}

//×Ö·û´®×ª»»
TEST(Array, CharFormJsonFull)
{
    Char1 c1;
    std::string str(19, '1');
    std::string json = "{\"buff\":\"" + str + "\"}";

    ASSERT_EQ(JsonSerializeFormJson(json, c1), true);
    EXPECT_STREQ(c1.buff, str.c_str());
}

//×Ö·û´®×ª»»
TEST(Array, CharFormJsonOutOfRange)
{
    Char1 c1;
    std::string str(21, '1');
    std::string json = "{\"buff\":\"" + str + "\"}";

    ASSERT_EQ(JsonSerializeFormJson(json, c1), true);
    EXPECT_STRNE(c1.buff, str.c_str());
}