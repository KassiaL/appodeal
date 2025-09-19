# Appodeal SDK for Defold

This is the Appodeal SDK extension for the Defold game engine. **Only Android is supported.**

## Lua API

The global `appodeal` object provides the following methods and constants:

### Methods

- `appodeal.initialize(app_key: string, is_testing: boolean)`
  - Initializes the Appodeal SDK with your app key and test mode flag.
- `appodeal.set_callback(callback: function)`
  - Sets a Lua callback function to receive ad events.
- `appodeal.is_interstitial_loaded() -> boolean`
  - Returns `true` if an interstitial ad is loaded and ready to show.
- `appodeal.show_interstitial(placement: string)`
  - Shows an interstitial ad for the given placement name.
- `appodeal.is_rewarded_loaded() -> boolean`
  - Returns `true` if a rewarded ad is loaded and ready to show.
- `appodeal.show_rewarded(placement: string)`
  - Shows a rewarded ad for the given placement name.

### Constants

- `appodeal.MSG_ADS_INITED`
- `appodeal.MSG_INTERSTITIAL`
- `appodeal.MSG_REWARDED`
- `appodeal.MSG_BANNER`
- `appodeal.EVENT_NOT_LOADED`
- `appodeal.EVENT_LOADED`
- `appodeal.EVENT_SHOWN`
- `appodeal.EVENT_ERROR_SHOW`
- `appodeal.EVENT_CLICKED`
- `appodeal.EVENT_CLOSED`
- `appodeal.EVENT_REWARDED`
- `appodeal.EVENT_EXPIRED`

## Usage Example

```lua
appodeal.initialize("your_appodeal_key", false)
appodeal.set_callback(function(self, message_id, message)
    if message_id == appodeal.MSG_REWARDED then
        if message.event == appodeal.EVENT_REWARDED then
            -- Reward the user
        end
    end
end)
if appodeal.is_rewarded_loaded() then
    appodeal.show_rewarded("default")
end
```
