#if defined(DM_PLATFORM_ANDROID)

#pragma once

namespace dmAppodealAds
{
    void Initialize_Ext();
    void ActivateApp();

    void Initialize(const char *app_key, bool is_testing);

    bool IsInterstitialLoaded();
    void ShowInterstitial(const char *unitId);

    bool IsRewardedLoaded();
    void ShowRewarded(const char *unitId);

    void SetUserConsent(bool val);
}

#endif
