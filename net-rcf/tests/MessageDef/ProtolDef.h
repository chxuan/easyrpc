/*************************************************************************
	> File Name: ProtolDef.h
	> Author: 
	> Mail: 
	> Created Time: 2016年01月09日 星期六 20时25分20秒
 ************************************************************************/

#ifndef _PROTOLDEF_H
#define _PROTOLDEF_H

#include <RCF/RCF.hpp>

RCF_BEGIN(I_RCFMessageHandler, "I_RCFMessageHandler")
    RCF_METHOD_V1(void, print, const std::string&)
RCF_END(I_RCFMessageHandler)

#endif
