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

    void ShowBanner(const char *position, const char *placement);
    void HideBanner();
    const char *GetBannerState();

    void SetUseSafeArea(bool val);

    void SetUserConsent(bool val);
}

#endif
