//#include "log.h"
//
//
//void OnDataLoaded()
//{
//   
//}
//
//void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
//{
//	switch (a_msg->type) {
//	case SKSE::MessagingInterface::kDataLoaded:
//        
//		break;
//	case SKSE::MessagingInterface::kPostLoad:
//		break;
//	case SKSE::MessagingInterface::kPreLoadGame:
//		break;
//	case SKSE::MessagingInterface::kPostLoadGame:
//        break;
//	case SKSE::MessagingInterface::kNewGame:
//		break;
//	}
//}
//
//SKSEPluginLoad(const SKSE::LoadInterface *skse) {
//    SKSE::Init(skse);
//	SetupLog();
//
//	//SKSE::GetPapyrusInterface()->Register(BindPapyrusFunctions);
//
//    /*auto messaging = SKSE::GetMessagingInterface();
//	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
//		return false;
//	}*/
//
//	
//    return true;
//}
//
//

RE::Actor* GetNPCFormIDByEditorID(RE::StaticFunctionTag*, std::string EditorID) {
    return RE::TESForm::LookupByEditorID<RE::Actor>(EditorID);
}

std::string TestingPrint(RE::StaticFunctionTag*)
{
	return "testing script";
}

bool PapyrusFunction(RE::BSScript::IVirtualMachine* vm) {
    vm->RegisterFunction("GetNPCFormIDByEditorID", "SkyRomanceMiscFunction", GetNPCFormIDByEditorID);
	vm->RegisterFunction("TestingPrint", "SkyRomanceMiscFunction", TestingPrint);
	return true;
}

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
	SKSE::Init(skse);
	SKSE::GetPapyrusInterface()->Register(PapyrusFunction);
	return true;
}