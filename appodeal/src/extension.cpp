#define EXTENSION_NAME AppodealSdkAds
#define LIB_NAME "dmAppodealAds"
#define MODULE_NAME "appodeal"

#define DLIB_LOG_DOMAIN LIB_NAME
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_ANDROID)

#include "extension_private.h"
#include "extension_callback_private.h"
#include "utils/LuaUtils.h"

namespace dmAppodealAds
{

	static int Lua_Initialize(lua_State *L)
	{
		DM_LUA_STACK_CHECK(L, 0);

		// Проверяем, что первый аргумент - строка
		if (lua_type(L, 1) != LUA_TSTRING)
		{
			return DM_LUA_ERROR("Expected string, got %s. Wrong type for APP KEY '%s'.", luaL_typename(L, 1), lua_tostring(L, 1));
		}
		const char *app_key_lua = luaL_checkstring(L, 1);

		// Проверяем, что второй аргумент - булевое значение
		if (lua_type(L, 2) != LUA_TBOOLEAN)
		{
			return DM_LUA_ERROR("Expected boolean, got %s. Wrong type for is_testing.", luaL_typename(L, 2));
		}
		int is_testing_lua = lua_toboolean(L, 2);

		// Теперь можно вызвать Initialize с двумя аргументами
		Initialize(app_key_lua, is_testing_lua);

		return 0;
	}

	static int Lua_SetCallback(lua_State *L)
	{
		DM_LUA_STACK_CHECK(L, 0);
		SetLuaCallback(L, 1);
		return 0;
	}

	// ------------------------------------------------------------------------------------------

	static int Lua_IsInterstitialLoaded(lua_State *L)
	{
		DM_LUA_STACK_CHECK(L, 1);
		bool is_loaded = IsInterstitialLoaded();
		lua_pushboolean(L, is_loaded);
		return 1;
	}

	static int Lua_ShowInterstitial(lua_State *L)
	{
		DM_LUA_STACK_CHECK(L, 0);
		if (lua_type(L, 1) != LUA_TSTRING)
		{
			return DM_LUA_ERROR("Expected string, got %s. Wrong type for Place '%s'.", luaL_typename(L, 1), lua_tostring(L, 1));
		}
		const char *unitId_lua = luaL_checkstring(L, 1);
		ShowInterstitial(unitId_lua);
		return 0;
	}

	// ------------------------------------------------------------------------------------------

	static int Lua_IsRewardedLoaded(lua_State *L)
	{
		DM_LUA_STACK_CHECK(L, 1);
		bool is_loaded = IsRewardedLoaded();
		lua_pushboolean(L, is_loaded);
		return 1;
	}

	static int Lua_ShowRewarded(lua_State *L)
	{
		DM_LUA_STACK_CHECK(L, 0);
		if (lua_type(L, 1) != LUA_TSTRING)
		{
			return DM_LUA_ERROR("Expected string, got %s. Wrong type for Place '%s'.", luaL_typename(L, 1), lua_tostring(L, 1));
		}
		const char *unitId_lua = luaL_checkstring(L, 1);
		ShowRewarded(unitId_lua);
		return 0;
	}

	// static int Lua_SetUserConsent(lua_State *L)
	// {
	// 	DM_LUA_STACK_CHECK(L, 0);
	// 	bool enable_rdp = luaL_checkbool(L, 1);
	// 	SetUserConsent(enable_rdp);
	// 	return 0;
	// }

	// ------------------------------------------------------------------------------------------

	static const luaL_reg Module_methods[] =
		{
			{"initialize", Lua_Initialize},
			{"set_callback", Lua_SetCallback},

			{"is_interstitial_loaded", Lua_IsInterstitialLoaded},
			{"show_interstitial", Lua_ShowInterstitial},

			{"is_rewarded_loaded", Lua_IsRewardedLoaded},
			{"show_rewarded", Lua_ShowRewarded},

			// {"set_user_consent", Lua_SetUserConsent},
			{0, 0}};

	static void LuaInit(lua_State *L)
	{
		DM_LUA_STACK_CHECK(L, 0);
		luaL_register(L, MODULE_NAME, Module_methods);

#define SETCONSTANT(name)                \
	lua_pushnumber(L, (lua_Number)name); \
	lua_setfield(L, -2, #name);
		SETCONSTANT(MSG_ADS_INITED)
		SETCONSTANT(MSG_INTERSTITIAL)
		SETCONSTANT(MSG_REWARDED)
		SETCONSTANT(MSG_BANNER)

		SETCONSTANT(EVENT_NOT_LOADED)
		SETCONSTANT(EVENT_LOADED)
		SETCONSTANT(EVENT_SHOWN)
		SETCONSTANT(EVENT_ERROR_SHOW)
		SETCONSTANT(EVENT_CLICKED)
		SETCONSTANT(EVENT_CLOSED)
		SETCONSTANT(EVENT_REWARDED)
		SETCONSTANT(EVENT_EXPIRED)

#undef SETCONSTANT

		lua_pop(L, 1);
	}

	static dmExtension::Result AppInitializeApp(dmExtension::AppParams *params)
	{
		return dmExtension::RESULT_OK;
	}

	static dmExtension::Result InitializeApp(dmExtension::Params *params)
	{
		LuaInit(params->m_L);
		Initialize_Ext();
		InitializeCallback();
		return dmExtension::RESULT_OK;
	}

	static dmExtension::Result AppFinalizeApp(dmExtension::AppParams *params)
	{
		return dmExtension::RESULT_OK;
	}

	static dmExtension::Result FinalizeApp(dmExtension::Params *params)
	{
		FinalizeCallback();
		return dmExtension::RESULT_OK;
	}

	static dmExtension::Result UpdateApp(dmExtension::Params *params)
	{
		UpdateCallback();
		return dmExtension::RESULT_OK;
	}

	static void OnEventApp(dmExtension::Params *params, const dmExtension::Event *event)
	{
		switch (event->m_Event)
		{
		case dmExtension::EVENT_ID_ACTIVATEAPP:
			ActivateApp();
			break;
		}
	}
}

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, dmAppodealAds::AppInitializeApp, dmAppodealAds::AppFinalizeApp, dmAppodealAds::InitializeApp, dmAppodealAds::UpdateApp, dmAppodealAds::OnEventApp, dmAppodealAds::FinalizeApp)

#else

static dmExtension::Result InitializeApp(dmExtension::Params *params)
{
	dmLogInfo("Registered extension Appodeal (null)");
	return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeApp(dmExtension::Params *params)
{
	return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, 0, 0, InitializeApp, 0, 0, FinalizeApp)

#endif // Android
