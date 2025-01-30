#include "../include/NativeEditorIDFixAPI.hpp"

const RE::TESForm* GetFormByEditorID(RE::StaticFunctionTag*, const std::string refEditorID) {
    const RE::TESForm* NPCForm = RE::TESForm::LookupByEditorID<RE::TESForm>(refEditorID);
    return NPCForm;
}

bool TestingPrint(RE::StaticFunctionTag*)
{
	RE::TESForm* NPCForm = RE::TESForm::LookupByEditorID<RE::TESForm>("CamillaValerius");
	RE::TESObjectREFR* NPC = NPCForm->As<RE::TESObjectREFR>();
	return NPC != NULL;
}

bool PapyrusFunction(RE::BSScript::IVirtualMachine* vm) {
    vm->RegisterFunction("GetFormByEditorID", "SkyRomanceMiscFunction", GetFormByEditorID);
	vm->RegisterFunction("TestingPrint", "SkyRomanceMiscFunction", TestingPrint);
	return true;
}

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
	SKSE::Init(skse);
	SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message) {
		if (message->type == SKSE::MessagingInterface::kDataLoaded)
			RE::ConsoleLog::GetSingleton()->Print("SKYROUtil plugins init!");
		});

	SKSE::GetPapyrusInterface()->Register(PapyrusFunction);
	return true;
}