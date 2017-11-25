/**
 * @file sig.h
 * @brief 定义easyrpc内部信号
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-25
 */
#pragma once

#include "easyrpc/utility/qt_connect.h"
#include "easyrpc/core/protocol/protocol_define.h"

class tcp_session;

extern qt_signal<bool, const std::string&> session_status_changed;
extern qt_signal<const request_body&, const std::shared_ptr<tcp_session>&> complete_server_decode_data;
extern qt_signal<const response_body&> complete_client_decode_data;
