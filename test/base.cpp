#include "JsonSerialize.hpp"
#include "cJSON.h"
#include <gtest/gtest.h>
struct Test1
{
    int val=0;
};
template<typename Archive>
void Serialize(Archive& ar, Test1& t)
{
    ar& JSON_NI_SERIALIZATION_KV(t, val);
}
// case1
TEST(Base, test1)
{
    Test1 t1;
    std::string json = "{\"val\":1}";
    EXPECT_EQ(js_form_json(json, t1), true);
    EXPECT_EQ(t1.val, 1);
}