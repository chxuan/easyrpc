/*************************************************************************
	> File Name: ProtolDef.h
	> Author: 
	> Mail: 
	> Created Time: 2016年01月09日 星期六 20时25分20秒
 ************************************************************************/

#ifndef _PROTOLDEF_H
#define _PROTOLDEF_H

#include <RCF/RCF.hpp>
#include "PeopleInfoMessage.h"

RCF_BEGIN(I_RCFMessageHandler, "I_RCFMessageHandler")
    RCF_METHOD_R2(bool, queryPeopleInfoByID, int, PeopleInfoMessage&)
RCF_END(I_RCFMessageHandler)

#endif
