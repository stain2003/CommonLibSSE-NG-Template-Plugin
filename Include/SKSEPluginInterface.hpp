//----------------------------------------------------------------------------------------------
// Small interface for SKSE plugins
//

#pragma once

#include <cstdint>

using SKSEPluginHandle = uint32_t;

struct SKSEPluginInfo
{
	enum { kInfoVersion = 1 };

	uint32_t    InfoVersion;
	const char* Name;
	uint32_t    Version;
};

struct SKSEInterface
{
	enum
	{
		kInterface_Invalid = 0,
		kInterface_Scaleform,
		kInterface_Papyrus,
		kInterface_Serialization,
		kInterface_Task,
		kInterface_Messaging,
		kInterface_Object,
		kInterface_Trampoline,
		kInterface_Max,
	};

	uint32_t SkseVersion;
	uint32_t RuntimeVersion;
	uint32_t EditorVersion;
	uint32_t IsEditor;

	void* (* QueryInterface)(uint32_t ID);

	// call during your Query or Load functions to get a PluginHandle uniquely identifying your plugin
	// invalid if called at any other time, so call it once and save the result
	SKSEPluginHandle (* GetPluginHandle)(void);

	// returns the SKSE build's release index
	uint32_t (* GetReleaseIndex)(void);

	// Minimum SKSE version 2.0.18
	// returns the plugin info structure for a plugin by name, only valid to be called after PostLoad message
	const SKSEPluginInfo* (* GetPluginInfo)(const char* Name);
};

struct SKSETrampolineInterface
{
	enum { kInterfaceVersion = 1 };

	uint32_t InterfaceVersion;

	void* (*AllocateFromBranchPool)(SKSEPluginHandle Plugin, size_t Size);
	void* (*AllocateFromLocalPool )(SKSEPluginHandle Plugin, size_t Size);
};

struct SKSEPluginVersionData
{
	enum { kVersion = 1 };

	enum
	{
		// set this if you are using a post-AE version of the Address Library
		kVersionIndependent_AddressLibraryPostAE = 1 << 0,

		// set this if you exclusively use signature matching to find your addresses and have NO HARDCODED ADDRESSES
		kVersionIndependent_Signatures = 1 << 1,

		// set this if you are using 1.6.629+ compatible structure layout (likely provided by CommonLib/SKSE)
		// this also marks you as incompatible with pre-1.6.629. see kVersionIndependentEx_NoStructUse if you have a
		// plugin that only does code patches and works across many versions
		kVersionIndependent_StructsPost629 = 1 << 2
	};

	enum
	{
		// set this if your plugin either doesn't use any game structures or has put in extraordinary effort
		// to work with pre and post 1.6.629 structure layout
		kVersionIndependentEx_NoStructUse = 1 << 0
	};

	uint32_t DataVersion;       // set to kVersion
	uint32_t PluginVersion;     // version number of your plugin
	char     Name[256];         // null-terminated ASCII plugin name
	char     Author[256];       // null-terminated ASCII plugin author name (can be empty)
	char     SupportEmail[252]; // null-terminated ASCII support email address (can be empty)
		                        // this is not for showing to users, it's in case I need to contact you about
		                        // a compatibility problem with your plugin

	// version compatibility
	uint32_t VersionIndependenceEx;	 // set to one of the kVersionIndependentEx_ enums or zero
	uint32_t VersionIndependence;    // set to one of the kVersionIndependent_ enums or zero
	uint32_t CompatibleVersions[16]; // zero-terminated list of RUNTIME_VERSION_ defines your plugin is compatible with

	uint32_t SEVersionRequired; // minimum version of the script extender required, compared against PACKED_SKSE_VERSION
		                        // you probably should just set this to 0 unless you know what you are doing
};
