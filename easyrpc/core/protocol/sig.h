/**
 * @file sig.h
 * @brief 定义easyrpc内部信号
 * @author chxuan, 787280310@qq.com
 * @version 1.0.0
 * @date 2017-11-25
 */
#pragma once

#include "easyrpc/utility/qt_connect.h"

extern qt_signal<bool, std::string> session_status_changed;
