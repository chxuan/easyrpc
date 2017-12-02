/**
 * @file rpc_server_test.h
 * @brief rpc服务端测试程序
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-12-02
 */
#pragma once

#include <memory>
#include <thread>
#include <atomic>

class rpc_server;
class request;
class response;

class rpc_server_test
{
public:
    rpc_server_test();
    void run();
    void stop();

private:
    void session_status_callback(bool established, const std::string& session_id);
    void echo(const std::shared_ptr<request>& req, const std::shared_ptr<response>& rsp);
    void publish_thread();

private:
    std::shared_ptr<rpc_server> server_;
    std::shared_ptr<std::thread> pub_thread_;
    std::atomic<bool> server_stoped_{ false };
    std::string client_session_id_;
};
