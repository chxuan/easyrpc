/**
 * @file sig.h
 * @brief 定义easyrpc内部信号
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-25
 */
#pragma once

#include "easyrpc/utility/qt_connect.h"

class result;
class request;
class response;

extern qt_signal<bool, std::string> session_status_changed;
extern qt_signal<int, std::shared_ptr<request>, std::shared_ptr<response>> complete_server_decode_data;
extern qt_signal<std::shared_ptr<result>> complete_client_decode_data;
