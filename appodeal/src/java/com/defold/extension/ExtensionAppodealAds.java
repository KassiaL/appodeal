package com.defold.extension;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import java.util.List;

import com.appodeal.ads.Appodeal;
import com.appodeal.ads.RewardedVideoCallbacks;
import com.appodeal.ads.initializing.ApdInitializationCallback;
import com.appodeal.ads.initializing.ApdInitializationError;
import com.appodeal.ads.utils.Log.*;

import com.appodeal.ads.BannerCallbacks;
import com.appodeal.ads.InterstitialCallbacks;
import com.appodeal.ads.MrecCallbacks;
import com.appodeal.ads.NativeCallbacks;



import org.json.JSONArray;
import org.json.JSONObject;
import org.json.JSONException;

public class ExtensionAppodealAds {

    private static final String TAG = ExtensionAppodealAds.class.getSimpleName();
	public static native void AddToQueue(int msg, String json);

	private static final int MSG_ADS_INITED = 0;
	private static final int MSG_INTERSTITIAL = 1;
	private static final int MSG_REWARDED = 2;
	private static final int MSG_BANNER = 3;

	private static final int EVENT_NOT_LOADED = 0;
	private static final int EVENT_LOADED = 1;
	private static final int EVENT_SHOWN = 2;
	private static final int EVENT_ERROR_SHOW = 3;
	private static final int EVENT_CLICKED = 4;
	private static final int EVENT_CLOSED = 5;
	private static final int EVENT_REWARDED = 6;
	private static final int EVENT_EXPIRED = 7;

    private Activity activity;
	private static final int LOCATION_PERMISSION_CODE = 1001;

    public ExtensionAppodealAds(Activity mainActivity) {
        activity = mainActivity;
    }

	public void initialize(String app_key, boolean testing) {
        Appodeal.setLogLevel(LogLevel.verbose);
		Appodeal.setTesting(testing);
        Appodeal.initialize(
            activity,
            app_key,
            Appodeal.INTERSTITIAL | Appodeal.REWARDED_VIDEO,
            new ApdInitializationCallback() {
                @Override
                public void onInitializationFinished(List<ApdInitializationError> errors) {
                    String initResult = errors == null || errors.isEmpty() ? "successfully" : "with " + errors.size() + " errors";
                    Log.d(TAG, "Appodeal initialized " + initResult);
                    if (errors != null) {
                        for (ApdInitializationError error : errors) {
                            Log.e(TAG, "onInitializationFinished: " + error);
                        }
						sendSimpleMessage(MSG_ADS_INITED, EVENT_NOT_LOADED);
                    }else{
						sendSimpleMessage(MSG_ADS_INITED, EVENT_LOADED);
					}
                }
        	}
		);

		Appodeal.setRewardedVideoCallbacks(new RewardedVideoCallbacks() {
            @Override
            public void onRewardedVideoLoaded(boolean isPrecache) {
                Log.d(TAG, "Rewarded video was loaded, isPrecache: " + isPrecache);
				sendSimpleMessage(MSG_REWARDED, EVENT_LOADED);
            }

            @Override
            public void onRewardedVideoFailedToLoad() {
                Log.d(TAG, "Rewarded video failed to load");
				sendSimpleMessage(MSG_REWARDED, EVENT_NOT_LOADED);
            }

            @Override
            public void onRewardedVideoClicked() {
                Log.d(TAG, "Rewarded video was clicked");
				sendSimpleMessage(MSG_REWARDED, EVENT_CLICKED);
            }

            @Override
            public void onRewardedVideoShowFailed() {
                Log.d(TAG, "Rewarded video failed to show");
				sendSimpleMessage(MSG_REWARDED, EVENT_ERROR_SHOW);
            }

            @Override
            public void onRewardedVideoShown() {
                Log.d(TAG, "Rewarded video was shown");
				sendSimpleMessage(MSG_REWARDED, EVENT_SHOWN);
            }

            @Override
            public void onRewardedVideoClosed(boolean finished) {
                Log.d(TAG, "Rewarded video was closed, isVideoFinished: " + finished);
				sendSimpleMessage(MSG_REWARDED, EVENT_CLOSED);
            }

            @Override
            public void onRewardedVideoFinished(double amount, String name) {
                Log.d(TAG, "Rewarded video was finished, amount: " + amount + ", currency: " + name);
				sendSimpleMessage(MSG_REWARDED, EVENT_REWARDED, "amount", amount, "currency", name);
            }

            @Override
            public void onRewardedVideoExpired() {
                Log.d(TAG, "Rewarded video was expired");
				sendSimpleMessage(MSG_REWARDED, EVENT_EXPIRED);
            }
        });

		Appodeal.setInterstitialCallbacks(new InterstitialCallbacks() {
			@Override
			public void onInterstitialLoaded(boolean isPrecache) {
				Log.d(TAG, "Called when interstitial is loaded");
				sendSimpleMessage(MSG_INTERSTITIAL, EVENT_LOADED);
			}
			@Override
			public void onInterstitialFailedToLoad() {
				Log.d(TAG, "Called when interstitial failed to load");
				sendSimpleMessage(MSG_INTERSTITIAL, EVENT_NOT_LOADED);
			}
			@Override
			public void onInterstitialShown() {
			   Log.d(TAG, "Called when interstitial is shown");
			   sendSimpleMessage(MSG_INTERSTITIAL, EVENT_SHOWN);
			}
			@Override
			public void onInterstitialShowFailed() {
			   Log.d(TAG, "Called when interstitial show failed");
			   sendSimpleMessage(MSG_INTERSTITIAL, EVENT_ERROR_SHOW);
			}
			@Override
			public void onInterstitialClicked() {
			   Log.d(TAG, "Called when interstitial is clicked");
			   sendSimpleMessage(MSG_INTERSTITIAL, EVENT_CLICKED);
			}
			@Override
			public void onInterstitialClosed() {
			   Log.d(TAG, "Called when interstitial is closed");
			   sendSimpleMessage(MSG_INTERSTITIAL, EVENT_CLOSED);
			}
			@Override
			public void onInterstitialExpired() {
			   Log.d(TAG, "Called when interstitial is expired");
			   sendSimpleMessage(MSG_INTERSTITIAL, EVENT_EXPIRED);
			}
		  });
    }

	// public void setUserConsent(boolean enable_rdp) {
	// 	Log.d(TAG, "setUserConsent:"+enable_rdp);
	// 	if (enable_rdp) {
	// 		Appodeal.updateGDPRUserConsent(GDPRUserConsent.Personalized);
	// 	} else {
	// 		Appodeal.updateGDPRUserConsent(GDPRUserConsent.NonPersonalized);
	// 	}

	// 	if (enable_rdp) {
	// 		Appodeal.updateCCPAUserConsent(CCPAUserConsent.OptIn);
	// 	} else {
	// 		Appodeal.updateCCPAUserConsent(CCPAUserConsent.OptOut);
	// 	}
	// }

	public boolean isInterstitialLoaded() {
		if (Appodeal.isLoaded(Appodeal.INTERSTITIAL)) {
			return true;
		}
		
		return false;
	}

	public void showInterstitial(final String place) {
		activity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if (Appodeal.isLoaded(Appodeal.INTERSTITIAL)) {
					Appodeal.show(activity, Appodeal.INTERSTITIAL, place);
				}else{
					sendSimpleMessage(MSG_INTERSTITIAL, EVENT_NOT_LOADED, "error", "Can't show INTERSTITIAL AD that wasn't loaded.");
				}
			}
		});
	}

	public boolean isRewardedLoaded() {
		if (Appodeal.isLoaded(Appodeal.REWARDED_VIDEO)) {
			return true;
		}
		
		return false;
	}

	public void showRewarded(final String place) {
		activity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if (Appodeal.isLoaded(Appodeal.REWARDED_VIDEO)) {
					Appodeal.show(activity, Appodeal.REWARDED_VIDEO, place);
				}else{
					sendSimpleMessage(MSG_REWARDED, EVENT_NOT_LOADED, "error", "Can't show REWARDED_VIDEO AD that wasn't loaded.");
				}
			}
		});
	}

	// ------------------------------------------------------------------------------------------
	private String getJsonConversionErrorMessage(String messageText) {
		String message = null;
		try {
			JSONObject obj = new JSONObject();
			obj.put("error", messageText);
			message = obj.toString();
		} catch (JSONException e) {
			message = "{ \"error\": \"Error while converting simple message to JSON.\" }";
		}
		return message;
	}

	private void sendSimpleMessage(int msg, int eventId) {
		String message = null;
		try {
			JSONObject obj = new JSONObject();
			obj.put("event", eventId);
			message = obj.toString();
		} catch (JSONException e) {
			message = getJsonConversionErrorMessage(e.getLocalizedMessage());
		}
		AddToQueue(msg, message);
	}

	private void sendSimpleMessage(int msg, int eventId, String key_2, String value_2) {
		String message = null;
		try {
			JSONObject obj = new JSONObject();
			obj.put("event", eventId);
			obj.put(key_2, value_2);
			message = obj.toString();
		} catch (JSONException e) {
			message = getJsonConversionErrorMessage(e.getLocalizedMessage());
		}
		AddToQueue(msg, message);
	}

	private void sendSimpleMessage(int msg, int eventId, String key_2, Number value_2, String key_3, String value_3) {
		String message = null;
		try {
			JSONObject obj = new JSONObject();
			obj.put("event", eventId);
			obj.put(key_2, value_2);
			obj.put(key_3, value_3);
			message = obj.toString();
		} catch (JSONException e) {
			message = getJsonConversionErrorMessage(e.getLocalizedMessage());
		}
		AddToQueue(msg, message);
	}
}
