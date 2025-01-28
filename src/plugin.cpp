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

RE::TESForm* GetNPCByEditorID(RE::StaticFunctionTag*) {
    return RE::TESForm::LookupByEditorID("Abelone");
}

std::string TestingPrint(RE::StaticFunctionTag*)
{
	return "testing script";
}

bool PapyrusFunction(RE::BSScript::IVirtualMachine* vm) {
    vm->RegisterFunction("GetNPCByEditorID", "SkyRomanceMiscFunction", GetNPCByEditorID);
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