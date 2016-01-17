#ifndef _PUBSUBPROTOCOLDEFINE_H
#define _PUBSUBPROTOCOLDEFINE_H

#include <RCF/RCF.hpp>
#include "WeatherInfoMessage.h"

RCF_BEGIN(I_PubSubMessageHandler, "I_PubSubMessageHandler")
    RCF_METHOD_V1(void, pushWeather, WeatherInfoMessage&)
RCF_END(I_PubSubMessageHandler)

#endif
