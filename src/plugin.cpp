//#include "../include/NativeEditorIDFixAPI.hpp"
//#include "../Include/editorID.hpp"
#include <string>
#include "../Include/SKYRO.hpp"

using InventoryItemMap = RE::TESObjectREFR::InventoryItemMap;
using _GetFormEditorID = const char* (*)(std::uint32_t);
static InventoryItemMap SavedInventoryMap;


bool bisDebugBuild()
{
	auto DebugBuildVar = RE::TESForm::LookupByEditorID("GVSR_DebugEnabled")->As<RE::TESGlobal>();
	return DebugBuildVar->value == 1;
}

const RE::TESForm* GetFormByEditorID(RE::StaticFunctionTag*, const std::string refEditorID) {
    const RE::TESForm* NPCForm = RE::TESForm::LookupByEditorID<RE::TESForm>(refEditorID);
    return NPCForm;
}

void SKSEGetNPCInventory(RE::StaticFunctionTag*, RE::Actor* TargetNPC) {
	SavedInventoryMap = TargetNPC->GetInventory();
	RE::ConsoleLog::GetSingleton()->Print("Showing %s's inventory:", TargetNPC->GetDisplayFullName());

	//static auto tweaks = GetModuleHandle(L"po3_Tweaks");
	//static auto func = reinterpret_cast<_GetFormEditorID>(GetProcAddress(tweaks, "GetFormEditorID"));

	//for (const auto& [item, dataB] : SavedInventoryMap)
	//{
	//	RE::TESForm* form = RE::TESForm::LookupByID(item->GetFormID());
	//	if (form)
	//	{
	//		RE::ConsoleLog::GetSingleton()->Print("Valid: %s", func(form->formID));
	//	}
	//	else
	//	{
	//		RE::ConsoleLog::GetSingleton()->Print("Failed");
	//	}
	//}

	return;
}

RE::Actor* SKSEGetBarterNPC(RE::StaticFunctionTag*)
{
	REL::Relocation<RE::ActorHandle*> merchant{ RELOCATION_ID(519283, 405823) };
	if (auto m = *merchant; m) {
		return m.get().get();
	}
	return nullptr;
	return;
}

void GetAddedItems(RE::StaticFunctionTag*, RE::Actor* TargetNPC)
{
	InventoryItemMap addedItems;
	InventoryItemMap B = TargetNPC->GetInventory();
	//Loop through the current inventory and compare items that are newly added
	for (const auto& [item, dataB] : B) {
		auto itA = SavedInventoryMap.find(item);

		std::uint32_t countB = dataB.first;
		std::uint32_t countA = (itA != SavedInventoryMap.end()) ? itA->second.first : 0;

		if (countB > countA) {  // Item is new or has increased count
			addedItems[item] = { countB - countA, std::make_unique<RE::InventoryEntryData>(*dataB.second) };
		}
	}

	//Write newly addeditems to json file
	std::vector<std::string> ItemsKey;
	std::vector<int> Counts;

	static auto tweaks = GetModuleHandle(L"po3_Tweaks");
	static auto func = reinterpret_cast<_GetFormEditorID>(GetProcAddress(tweaks, "GetFormEditorID"));
	RE::ConsoleLog::GetSingleton()->Print("Gift are given to %s:", TargetNPC->GetDisplayFullName());
	for (const auto& [item, dataB] : addedItems)
	{
		const char* ItemName;
		if (item)
		{
			//ItemName = item->GetFormEditorID();
			ItemName = func(item->formID);
		}
		else
		{
			ItemName = "NULL";
		}

		uint32_t ItemCount = dataB.first;
		
		ItemsKey.push_back(ItemName);
		Counts.push_back(ItemCount);
		
		//Console Log
		if (ItemName != "" && bisDebugBuild())
		{
			RE::ConsoleLog::GetSingleton()->Print("SKSE: %s x %d", ItemName, ItemCount);
		}
	}


	//Call papyrus function to write to json file
	auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();

	RE::TESForm* form = RE::TESForm::LookupByEditorID("SkyRomanceInitQuest");
	auto policy = vm->GetObjectHandlePolicy();
	RE::VMHandle handle = policy->GetHandleForObject(form->GetFormType(), form);

	if (handle == policy->EmptyHandle()) {
		return;
	}
	//RE::BSFixedString scriptName = SKYRO::MainQuestScript;
	RE::BSFixedString scriptName = "SkyRomanceInitQuestScript";
	RE::BSFixedString functionName = "WriteAddedItemsToJson";

	RE::BSTSmartPointer<RE::BSScript::Object> object;
	RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> result;

	if (vm->FindBoundObject(handle, scriptName.c_str(), object)) {
		auto args = RE::MakeFunctionArguments(std::move(ItemsKey), std::move(Counts));
		vm->DispatchMethodCall1(object, functionName, args, result);
	}

	return;
}

void TestingPrint(RE::StaticFunctionTag*)
{
	if (bisDebugBuild())
	{
		RE::ConsoleLog::GetSingleton()->Print(".....Debug Test String:.....");
		for (const auto& [item, dataB] : SavedInventoryMap) {
			const char* ItemName = item->GetFormEditorID();
			uint32_t ItemCount = dataB.first;
			if (ItemName != "")
			{
				RE::ConsoleLog::GetSingleton()->Print("%s x %d", ItemName, ItemCount);
			}
		}
	}

	auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();

	RE::TESForm* form = RE::TESForm::LookupByEditorID("SkyRomanceInitQuest");

	auto policy = vm->GetObjectHandlePolicy();
	RE::VMHandle handle = policy->GetHandleForObject(form->GetFormType(), form);

	if (handle == policy->EmptyHandle()) {
		return;
	}

	RE::BSFixedString scriptName = "SkyRomanceInitQuestScript";
	RE::BSFixedString functionName = "WriteAddedItemsToJson";

	RE::BSTSmartPointer<RE::BSScript::Object> object;
	RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> result;

	if (vm->FindBoundObject(handle, scriptName.c_str(), object)) {
		auto args = RE::MakeFunctionArguments();
		vm->DispatchMethodCall1(object, functionName, args, result);
	}

	return;
}

bool PapyrusFunction(RE::BSScript::IVirtualMachine* vm) {
    vm->RegisterFunction("GetFormByEditorID", "SkyRomanceMiscFunction", GetFormByEditorID);
	vm->RegisterFunction("TestingPrint", "SkyRomanceMiscFunction", TestingPrint);
	vm->RegisterFunction("SKSEGetNPCInventory", "SkyRomanceMiscFunction", SKSEGetNPCInventory);
	vm->RegisterFunction("GetAddedItems", "SkyRomanceMiscFunction", GetAddedItems);
	vm->RegisterFunction("SKSEGetBarterNPC", "SkyRomanceMiscFunction", SKSEGetBarterNPC);

	return true;
}

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
	SKSE::Init(skse);
	SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message) {
		if (message->type == SKSE::MessagingInterface::kDataLoaded)
			RE::ConsoleLog::GetSingleton()->Print("SKYROUtil plugins init!");
		});

	SKSE::GetPapyrusInterface()->Register(PapyrusFunction);

	//Reset static variables
	SavedInventoryMap.clear();

	return true;
}