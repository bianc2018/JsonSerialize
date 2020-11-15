//�������
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

//���Ի������ַ������л�����
TEST(Array, CharBase)
{
    Char1 c1 = { "1234567890" };
    std::string json;
    ASSERT_EQ(JsonSerializeToJson(json, c1), true);

    Char1 c2;
    ASSERT_EQ(JsonSerializeFormJson(json, c2), true);

    EXPECT_STREQ(c1.buff, c2.buff);
}

//���Կ��ַ���
TEST(Array, CharEmpty)
{
    Char1 c1 = { "" };
    std::string json;
    ASSERT_EQ(JsonSerializeToJson(json, c1), true);

    Char1 c2;
    ASSERT_EQ(JsonSerializeFormJson(json, c2), true);

    EXPECT_STREQ(c1.buff, c2.buff);
}

//��������
TEST(Array, CharFull)
{
    Char1 c1 = { "1234567890123456789" };
    std::string json;
    ASSERT_EQ(JsonSerializeToJson(json, c1), true);

    Char1 c2;
    ASSERT_EQ(JsonSerializeFormJson(json, c2), true);

    EXPECT_STREQ(c1.buff, c2.buff);
}

//�ַ���ת��
TEST(Array, CharFormJson10)
{
    Char1 c1 ;
    std::string str(10,'1');
    std::string json = "{\"buff\":\""+str+"\"}";
    
    ASSERT_EQ(JsonSerializeFormJson(json, c1), true);
    EXPECT_STREQ(c1.buff, str.c_str());
}

//�ַ���ת��
TEST(Array, CharFormJsonEmpty)
{
    Char1 c1;
    std::string str;
    std::string json = "{\"buff\":\"" + str + "\"}";

    ASSERT_EQ(JsonSerializeFormJson(json, c1), true);
    EXPECT_STREQ(c1.buff, str.c_str());
}

//�ַ���ת��
TEST(Array, CharFormJsonFull)
{
    Char1 c1;
    std::string str(19, '1');
    std::string json = "{\"buff\":\"" + str + "\"}";

    ASSERT_EQ(JsonSerializeFormJson(json, c1), true);
    EXPECT_STREQ(c1.buff, str.c_str());
}

//�ַ���ת��
TEST(Array, CharFormJsonOutOfRange)
{
    Char1 c1;
    std::string str(21, '1');
    std::string json = "{\"buff\":\"" + str + "\"}";

    ASSERT_EQ(JsonSerializeFormJson(json, c1), true);
    EXPECT_STRNE(c1.buff, str.c_str());
}