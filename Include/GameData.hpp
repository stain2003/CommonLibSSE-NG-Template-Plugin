//----------------------------------------------------------------------------------------------
// Native types
// Copyright (c) 2023-2024 Kitsuune - Apache License 2.0
//

#pragma once

#include <AddressLibraries.hpp>

struct
{
	template<typename DatabaseT> void Initialize(const uint32_t(& RuntimeVersion)[3])
	{
		DatabaseT Database;
		if(!Database.Load()) throw std::runtime_error("Unable to load address database");

		struct IDAddressPair
		{
			IDAddressPair(uint64_t ID = 0, int64_t Offset = 0) : ID(ID), Offset(Offset) {}
			uint64_t ID;
			int64_t  Offset;
		};

		auto FindAddressById = [&](IDAddressPair Pair, IDAddressPair Pair_1_5 = {})
		{
			if constexpr(std::is_same_v<DatabaseT, OldVersionDb>)
				Pair = Pair_1_5;

			if(auto AddressOut = Database.FindAddressById(Pair.ID))
				return reinterpret_cast<uintptr_t>(AddressOut) + Pair.Offset;

			throw std::runtime_error(std::format("Unable to retrieve address from database (ID: {})", Pair.ID));
		};

		TESForm_GetEditorID = FindAddressById(10879, 10827);
		TESForm_SetEditorID = FindAddressById(10926, 10883);

		TESObjectREFR_GetEditorID = FindAddressById(19778, 19351);

		StringCacheRef_Ctor     = FindAddressById(69161, 67819);
		StringCacheRef_CopyCtor = FindAddressById(69162, 67820);
		StringCacheRef_Dtor     = FindAddressById(69192, 67822);
	
		FormStringHashTable_Insert = FindAddressById(13808, 13833);
		FindFormByEditorID         = FindAddressById(14620, 14464);
	
		EditorIDHashTablePtr  = FindAddressById(400509, 514352);
		EditorIDHashTableLock = FindAddressById(400518, 514361);
	
		RWLock_LockForReading    = FindAddressById(68233, 66976);
		RWLock_UnlockFromReading = FindAddressById(68239, 66982);
		RWLock_LockForWriting    = FindAddressById(68234, 66977);
		RWLock_UnlockFromWriting = FindAddressById(68240, 66983);

#		if SKYRIM_1_6_1130
			VsprintfCalls[ 0]     = FindAddressById({ 440412, 0x42  });
			VsprintfCalls[ 1]     = FindAddressById({ 440413, 0x42  });
			VsprintfCalls[ 2]     = FindAddressById({ 440474, 0x5C  });
			VsprintfCalls[ 3]     = FindAddressById({ 13906,  0x51  });
			VsprintfCalls[ 4]     = FindAddressById({ 23305,  0x51  }); // BufferSize +0x28
			VsprintfCalls[ 5]     = FindAddressById({ 35684,  0x65  }); // BufferSize +0x18
			VsprintfCalls[ 6]     = FindAddressById({ 35685,  0x5E  });
			VsprintfCalls[ 7]     = FindAddressById({ 51110,  0x287 });
			VsprintfCalls[ 8]     = FindAddressById({ 57117,  0x48  });
			VsprintfCalls[ 9]     = FindAddressById({ 69540,  0x51  });
			VsprintfCalls[10]     = FindAddressById({ 94541,  0x51  }); // BufferSize +0x28
			SmallVsprintfCalls[0] = FindAddressById({ 21962,  0x50  });
			SmallVsprintfCalls[1] = FindAddressById({ 23333,  0x4A  }); // BufferSize +0x10
#		else
			VsprintfCalls[0]  = FindAddressById({ 11045,  0x5C  }, { 10983, 0x44 });
			VsprintfCalls[1]  = FindAddressById({ 13906,  0x51  }, { 13837, 0x44 });
			VsprintfCalls[2]  = FindAddressById({ 23305,  0x51  }, { 15702, 0x48 });
			VsprintfCalls[3]  = FindAddressById({ 416397, 0x51  }, { 21483, 0x50 });
			VsprintfCalls[4]  = FindAddressById({ 35684,  0x65  }, { 22856, 0x44 });
			VsprintfCalls[5]  = FindAddressById({ 35685,  0x5e  }, { 34780, 0x5B });
			VsprintfCalls[6]  = FindAddressById({ 51110,  0x287 }, { 50180, 0xDB });
			VsprintfCalls[7]  = FindAddressById({ 69540,  0x51  }, { 56708, 0x48 });
			VsprintfCalls[8]  = FindAddressById({ 94541,  0x51  }, { 91676, 0x44 });
			SmallVsprintfCall = FindAddressById({ 21962,  0x50  }, { 22882, 0x4A });
#		endif

		GetEditorIDNodeNamingCalls[0] = FindAddressById({ 15683, 0x35D }, { 15506, 0x233 });
		GetEditorIDNodeNamingCalls[1] = FindAddressById({ 15688, 0x1BD }, { 15511, 0x165 });
		GetEditorIDNodeNamingCalls[2] = FindAddressById({ 15691, 0x3E3 }, { 15514, 0x323 });
		GetEditorIDNodeNamingCalls[3] = FindAddressById({ 15701, 0x126 }, { 15524, 0x126 });
		GetEditorIDNodeNamingCalls[4] = FindAddressById({ 15678, 0x7FD }, { 15501, 0x69B });
		GetEditorIDNodeNamingCalls[5] = FindAddressById({ 15678, 0x816 }, { 15501, 0x6B4 });
		
#		if SKYRIM_1_6
			if(RuntimeVersion[0] == 1 && RuntimeVersion[1] == 6 && RuntimeVersion[2] >= 629)
			{
				GetEditorIDNodeNamingCalls[4] -= 0xF;
				GetEditorIDNodeNamingCalls[5] -= 0xF;
			}
#		endif

		EmptyString              = FindAddressById({ 188366, 0x6   }, { 232409, 0x6   });
		HelpCommandNameRetrieval = FindAddressById({ 22839,  0x823 }, { 22364,  0x3E3 });

#		if SKYRIM_1_6_1130
			HelpCommandNameRetrieval -= 0x3F;
#		endif

#		ifndef NDEBUG
			uintptr_t* AsArray = reinterpret_cast<uintptr_t*>(this);
			for(int i = 0; i < (sizeof(*this) / sizeof(uintptr_t)); i++)
				if(!AsArray[i]) throw std::invalid_argument("One or more address(es) not set");
#		endif
	}


	uintptr_t TESForm_GetEditorID = 0;
	uintptr_t TESForm_SetEditorID = 0;

	uintptr_t TESObjectREFR_GetEditorID = 0;

	uintptr_t StringCacheRef_Ctor     = 0;
	uintptr_t StringCacheRef_CopyCtor = 0;
	uintptr_t StringCacheRef_Dtor     = 0;

	uintptr_t FormStringHashTable_Insert = 0;
	uintptr_t FindFormByEditorID         = 0;

	uintptr_t EditorIDHashTablePtr  = 0;
	uintptr_t EditorIDHashTableLock = 0;

	uintptr_t RWLock_LockForReading    = 0;
	uintptr_t RWLock_UnlockFromReading = 0;
	uintptr_t RWLock_LockForWriting    = 0;
	uintptr_t RWLock_UnlockFromWriting = 0;

#	if SKYRIM_1_6_1130
		uintptr_t VsprintfCalls[11]     = {};
		uintptr_t SmallVsprintfCalls[2] = {};
#	else
		uintptr_t VsprintfCalls[9]  = {};
		uintptr_t SmallVsprintfCall = 0;
#	endif

	uintptr_t GetEditorIDNodeNamingCalls[6] = {};
	uintptr_t EmptyString                   = 0;

	uintptr_t HelpCommandNameRetrieval = 0;
} Addresses;


namespace Skyrim
{
	struct TESForm
	{
		/*virtual*/ const char* GetEditorID() const
		{
			using VFunc = const char*(*)(const TESForm*);
			return reinterpret_cast<VFunc>(VTable[50])(this);
		}

		/*virtual*/ bool SetEditorID(const char* NewEditorID)
		{
			using VFunc = bool(*)(TESForm*, const char*);
			return reinterpret_cast<VFunc>(VTable[51])(this, NewEditorID);
		}

		uintptr_t* VTable;    // 00
		void*      Unk8;      // 08
		uint32_t   FormFlags; // 10
		uint32_t   FormID;    // 14
		uint16_t   Unk18;     // 18
		uint8_t    FormType;  // 1A
		uint8_t    Pad1B[3];  // 1B
	};

	struct StringCacheRef
	{
		StringCacheRef() = default;
		StringCacheRef(const char* String)
		{
			using NativeFunc = void(*)(StringCacheRef*, const char*);
			reinterpret_cast<NativeFunc>(Addresses.StringCacheRef_Ctor)(this, String);
		}

		StringCacheRef(const StringCacheRef& In)
		{
			using NativeFunc = void(*)(StringCacheRef*, const StringCacheRef*);

			if(In.Data)
				reinterpret_cast<NativeFunc>(Addresses.StringCacheRef_CopyCtor)(this, &In);
			else
				Data = nullptr;
		}

		StringCacheRef(StringCacheRef&& In) noexcept
			: Data(std::exchange(In.Data, nullptr)) {}


		StringCacheRef& operator=(const StringCacheRef& In)
		{
			if(this != &In)
			{
				using NativeFunc = void(*)(StringCacheRef*, const StringCacheRef*);

				if(In.Data)
					reinterpret_cast<NativeFunc>(Addresses.StringCacheRef_CopyCtor)(this, &In);
				else
					Data = nullptr;
			}
			return *this;
		}

		StringCacheRef& operator=(StringCacheRef&& In) noexcept
		{
			if(this != &In)
				Data = std::exchange(In.Data, nullptr);
			
			return *this;
		}

		~StringCacheRef()
		{
			using NativeFunc = void(*)(StringCacheRef*);
			if(Data)
				reinterpret_cast<NativeFunc>(Addresses.StringCacheRef_Dtor)(this);
		}

		const char* Data = nullptr;
	};


	struct ReadWriteLock
	{
		void LockForReading()
		{
			using NativeFunc = void(*)(ReadWriteLock*);
			reinterpret_cast<NativeFunc>(Addresses.RWLock_LockForReading)(this);
		}

		void UnlockFromReading()
		{
			using NativeFunc = void(*)(ReadWriteLock*);
			reinterpret_cast<NativeFunc>(Addresses.RWLock_UnlockFromReading)(this);
		}

		void LockForWriting()
		{
			using NativeFunc = void(*)(ReadWriteLock*);
			reinterpret_cast<NativeFunc>(Addresses.RWLock_LockForWriting)(this);
		}

		void UnlockFromWriting()
		{
			using NativeFunc = void(*)(ReadWriteLock*);
			reinterpret_cast<NativeFunc>(Addresses.RWLock_UnlockFromWriting)(this);
		}

	private:
		// Thread ID is 0 while reading. It's only set when a thread wants to write data.
		// This allows multiple threads read-only access without blocking
		std::atomic_uint32_t ThreadID;
		std::atomic_uint32_t LockCount;
	};

	struct ReadWriteLockReadAdaptor : ReadWriteLock
	{
		void lock()   { LockForReading();    }
		void unlock() { UnlockFromReading(); }
	};

	struct ReadWriteLockWriteAdaptor : ReadWriteLock
	{
		void lock()   { LockForWriting();    }
		void unlock() { UnlockFromWriting(); }
	};



	struct EditorIDHashTable
	{
		static EditorIDHashTable& GetInstance()
		{
			auto* Instance = *reinterpret_cast<EditorIDHashTable**>(Addresses.EditorIDHashTablePtr);
			if(!Instance) throw std::runtime_error("EditorID hashtable not initialized");
			return *Instance;
		}

		static ReadWriteLockReadAdaptor& GetReadLock()
		{
			return *reinterpret_cast<ReadWriteLockReadAdaptor*>(Addresses.EditorIDHashTableLock);
		}

		static ReadWriteLockWriteAdaptor& GetWriteLock()
		{
			return *reinterpret_cast<ReadWriteLockWriteAdaptor*>(Addresses.EditorIDHashTableLock);
		}

		
		bool Insert(const StringCacheRef& EditorID, const TESForm& Form)
		{
#			if SKYRIM_1_6
				using NativeFunc = bool(*)(void* ThisTable, const StringCacheRef& EditorID, const TESForm*& FormPtr, TESForm*& UnkR9);
				TESForm*       TempForm = nullptr;
				const TESForm* FormPtr  = &Form;
				return reinterpret_cast<NativeFunc>(Addresses.FormStringHashTable_Insert)(&Table, EditorID, FormPtr, TempForm);
#			else
				return Insert(EditorID.Data, Form);
#			endif
		}

		bool Insert(const char* EditorID, const TESForm& Form)
		{
#			if SKYRIM_1_6
				return Insert(StringCacheRef(EditorID), Form);
#			else
				using NativeFunc = bool(*)(EditorIDHashTable*, const char* EditorID, const TESForm& Form);
				return reinterpret_cast<NativeFunc>(Addresses.FormStringHashTable_Insert)(this, EditorID, Form);
#			endif
		}


		// The hash table seems to have a sub-struct that's the actual table. Depending on which member a
		// native function belongs to, it either needs the base address or one that's shifted by 8 bytes

		void* Unk0;
		struct
		{
			uint32_t Unk0;          // 00 (08)
			uint32_t Capacity;      // 04 (0C)
			uint32_t FreeCount;     // 08 (10)
			uint32_t NextFreeIndex; // 0C (14)
			void*    Sentinel;      // 10 (18)
			uint64_t Unk18;         // 18 (20)
			void*    Data;          // 20 (28)
		} Table;
	};


	TESForm* FindFormByEditorID(const char* EditorID)
	{
#		if SKYRIM_1_6
			using NativeFunc = bool(*)(EditorIDHashTable&, const char* EditorID, TESForm*& FormOut);

			TESForm* Res = nullptr;
			if(reinterpret_cast<NativeFunc>(Addresses.FindFormByEditorID)(EditorIDHashTable::GetInstance(), EditorID, Res))
				return Res;
			return nullptr;
#		else
			using NativeFunc = TESForm*(*)(const char* EditorID);
			return reinterpret_cast<NativeFunc>(Addresses.FindFormByEditorID)(EditorID);
#		endif
	}
}
