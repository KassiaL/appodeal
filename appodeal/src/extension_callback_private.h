#if defined(DM_PLATFORM_ANDROID)
#pragma once

#include "extension_private.h"
#include <dmsdk/sdk.h>

namespace dmAppodealAds
{

    enum MessageId
    {
        MSG_ADS_INITED = 0,
        MSG_INTERSTITIAL = 1,
        MSG_REWARDED = 2,
        MSG_BANNER = 3
    };

    enum MessageEvent
    {
        EVENT_NOT_LOADED = 0,
        EVENT_LOADED = 1,
        EVENT_SHOWN = 2,
        EVENT_ERROR_SHOW = 3,
        EVENT_CLICKED = 4,
        EVENT_CLOSED = 5,
        EVENT_REWARDED = 6,
        EVENT_EXPIRED = 7
    };

    struct CallbackData
    {
        MessageId msg;
        char *json;
    };

    void SetLuaCallback(lua_State *L, int pos);
    void UpdateCallback();
    void InitializeCallback();
    void FinalizeCallback();

    void AddToQueueCallback(MessageId type, const char *json);

}

#endif
