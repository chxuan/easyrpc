#include <iostream>
#include <string>
#include <thread>
#include <gtest/gtest.h>
#include <easyrpc/easyrpc.hpp>
#include "user_define_classes.hpp"

std::vector<person_info_res> query_person_info(const person_info_req& req)
{
    EXPECT_EQ(12345678, req.card_id);
    EXPECT_STREQ("Jack", req.name.c_str());
    std::vector<person_info_res> vec;
    for (int i = 0; i < 2; ++i)
    {
        person_info_res res;
        res.card_id = req.card_id;
        res.name = req.name;
        res.age = 20;
        res.national = "han";
        vec.emplace_back(std::move(res));
    }
    return vec;
}

#ifdef ENABLE_JSON
std::string call_person(const std::string& str)
{
    std::cout << str << std::endl;
    DeSerializer dr;
    person_info_req req;
    dr.Parse(str);
    dr.Deserialize(req);
    EXPECT_EQ(12345678, req.card_id);
    EXPECT_STREQ("Jack", req.name.c_str());

    person_info_res res;
    res.card_id = req.card_id;
    res.name = req.name;
    res.age = 20;
    res.national = "han";
    Serializer sr;
    sr.Serialize(res);
    return sr.GetString();
}
#endif

class message_handle
{
public:
    std::string echo(const std::string& str)
    {
        EXPECT_STREQ("Hello world", str.c_str());
        return str;
    }
};

void sayHi(const std::string& str)
{
    std::cout << str << std::endl;
}

TEST(EasyRpcTest, ServerCase)
{
    message_handle hander;
    easyrpc::server app;
    try
    {
        app.bind("say_hello", []{ std::cout << "Hello" << std::endl; });
        bool ok = app.is_bind("say_hello");
        ASSERT_TRUE(ok);

        app.bind("echo", &message_handle::echo, &hander);
        ok = app.is_bind("echo");
        ASSERT_TRUE(ok);

        app.bind("query_person_info", &query_person_info);
        ok = app.is_bind("query_person_info");
        ASSERT_TRUE(ok);

        app.bind_raw("say_hi", &sayHi);
        ok = app.is_bind_raw("say_hi");
        ASSERT_TRUE(ok);

#ifdef ENABLE_JSON
        app.bind_raw("call_person", &call_person);
        ok = app.is_bind_raw("call_person");
        ASSERT_TRUE(ok);
#endif

        app.listen(50051).multithreaded(10).run();
    }
    catch (std::exception& e)
    {
        easyrpc::log_warn(e.what());
        FAIL();
    }

    std::cin.get();
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}

