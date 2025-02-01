#include "../include/NativeEditorIDFixAPI.hpp"

using InventoryItemMap = RE::TESObjectREFR::InventoryItemMap;
static InventoryItemMap SavedInventoryMap;

bool bisDebugBuild()
{
	auto DebugBuildVar = RE::TESForm::LookupByEditorID("SRK_DebugEnabled")->As<RE::TESGlobal>();
	return DebugBuildVar->value == 1;
}

const RE::TESForm* GetFormByEditorID(RE::StaticFunctionTag*, const std::string refEditorID) {
    const RE::TESForm* NPCForm = RE::TESForm::LookupByEditorID<RE::TESForm>(refEditorID);
    return NPCForm;
}

void SKSEGetNPCInventory(RE::StaticFunctionTag*, RE::Actor* TargetNPC) {
	SavedInventoryMap = TargetNPC->GetInventory();
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

	//Print items newly added to console log
	for (const auto& [item, dataB] : addedItems)
	{
		const char* ItemName = item->GetFormEditorID();
		uint32_t ItemCount = dataB.first;
		RE::ConsoleLog::GetSingleton()->Print("Gift are given to %s:", TargetNPC->GetDisplayFullName());
		if (ItemName != "")
		{
			RE::ConsoleLog::GetSingleton()->Print("%s x %d", ItemName, ItemCount);
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
		int x = 42;
		auto args = RE::MakeFunctionArguments(std::move(x));
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