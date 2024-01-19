

#include <Windows.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

#include <string>
#include <cctype>

#include <steam_api.h>

struct WorkshopItemInfo
{
	PublishedFileId_t fileID = (PublishedFileId_t)0;
	std::string path;
	std::string name;
	std::string version;
	ERemoteStoragePublishedFileVisibility visibility;
};

class SteamHandler
{
public:
	void CreateItem(WorkshopItemInfo& info);
	void UpdateItem(WorkshopItemInfo& info, std::string changeLog);

	bool finished = false;
	bool success = false;
	AppId_t nConsumerAppID = (AppId_t)221100;

	WorkshopItemInfo* currentItem = nullptr;

private:
	void OnItemCreated(CreateItemResult_t* pCallback, bool bIOFailure);
	void OnItemSubmitted(SubmitItemUpdateResult_t* pCallback, bool bIOFailure);

	CCallResult<SteamHandler, CreateItemResult_t> m_CreateItemResult;
	CCallResult<SteamHandler, SubmitItemUpdateResult_t> m_SubmitItemUpdateResult;
};

void SteamHandler::CreateItem(WorkshopItemInfo& info)
{
	std::cout << "Creating item..." << std::endl;
	finished = false;
	success = false;

	currentItem = &info;

	SteamAPICall_t hSteamAPICall = SteamUGC()->CreateItem(nConsumerAppID, k_EWorkshopFileTypeCommunity);
	m_CreateItemResult.Set(hSteamAPICall, this, &SteamHandler::OnItemCreated);

	while (!finished) {
		SteamAPI_RunCallbacks();
	}
}

void SteamHandler::UpdateItem(WorkshopItemInfo& info, std::string changeLog)
{
	finished = true;

	std::cout << "Updating item..." << std::endl;

	currentItem = &info;

	std::fstream meta_file(currentItem->path + "meta.cpp", std::ios::out | std::ios::trunc);
	if (meta_file.is_open()) {
		meta_file << "protocol = 1;" << std::endl;
		meta_file << "publishedid = " << currentItem->fileID << ";" << std::endl;
		meta_file << "name = \"" << currentItem->name << "\";" << std::endl;

		// https://www.codeproject.com/Tips/694874/Exchange-Date-Time-between-Native-Cplusplus-and-Cs
		const auto currentTime = std::chrono::system_clock::now();
		long long epoch = std::chrono::duration_cast<std::chrono::seconds>(currentTime.time_since_epoch()).count();
		long long timeNow = (epoch * 10000000) + 0x089f7ff5f7b58000;

		meta_file << "timestamp = " << timeNow << ";" << std::endl;

		meta_file.close();
	}
	else {
		std::cerr << "Failure to create and save \"" << currentItem->path << "meta.cpp\" file" << std::endl;
		return;
	}

	std::fstream mod_file(currentItem->path + "mod.cpp", std::ios::out | std::ios::trunc);
	if (mod_file.is_open()) {

		mod_file << "name = \"DayZ Editor\";\n";
		mod_file << "picture = \"Editor/gui/images/dayz_editor_icon_black.edds\";\n";
		mod_file << "logo = \"Editor/gui/images/dayz_editor_icon_black.edds\";\n";
		mod_file << "logoSmall = \"Editor/gui/images/dayz_editor_icon_black.edds\";\n";
		mod_file << "logoOver = \"Editor/gui/images/dayz_editor_icon_black.edds\";\n";
		mod_file << "tooltip = \"DayZ Editor\";\n";
		mod_file << "overview = \"\";\n";
		mod_file << "action = \"\";\n";
		mod_file << "author = \"InclementDab\";\n";
		mod_file << "authorID = \"\";\n";
		mod_file << "version = \"" << currentItem->version << "\";\n";

		mod_file.close();
	}
	else {
		std::cerr << "Failure to create and save \"" << currentItem->path << "mod.cpp\" file" << std::endl;
		return;
	}


	UGCUpdateHandle_t handle = SteamUGC()->StartItemUpdate(nConsumerAppID, currentItem->fileID);

	SteamUGC()->SetItemTitle(handle, currentItem->name.c_str());
	SteamUGC()->SetItemVisibility(handle, currentItem->visibility);

	SteamParamStringArray_t* pTags = new SteamParamStringArray_t();
	if (!SteamUGC()->SetItemContent(handle, currentItem->path.c_str())) {
		std::cerr << "Failed to set the item content folder" << std::endl;
		return;
	}

	finished = false;
	success = false;

	SteamAPICall_t submit_item_call = SteamUGC()->SubmitItemUpdate(handle, changeLog.c_str());
	if (submit_item_call == k_uAPICallInvalid) {
		std::cerr << "Submit item call invalid" << std::endl;
		return;
	}

	m_SubmitItemUpdateResult.Set(submit_item_call, this, &SteamHandler::OnItemSubmitted);

	while (!finished) {
		SteamAPI_RunCallbacks();
	}
}

void SteamHandler::OnItemCreated(CreateItemResult_t* pCallback, bool bIOFailure)
{
	finished = true;

	std::cout << "name=" << currentItem->name << " result=" << pCallback->m_eResult << " IOfailure=" << bIOFailure << std::endl;

	if (pCallback->m_eResult == k_EResultOK && !bIOFailure && !pCallback->m_bUserNeedsToAcceptWorkshopLegalAgreement) {
		currentItem->fileID = pCallback->m_nPublishedFileId;

		success = true;
	}
	else {
		std::cout << "Result:" << pCallback->m_eResult << std::endl;
		success = false;
	}

	currentItem = nullptr;
}

void SteamHandler::OnItemSubmitted(SubmitItemUpdateResult_t* pCallback, bool bIOFailure)
{
	finished = true;

	std::cout << "name=" << currentItem->name << " result=" << pCallback->m_eResult << " IOfailure=" << bIOFailure << std::endl;

	if (pCallback->m_eResult == k_EResultOK && !bIOFailure && !pCallback->m_bUserNeedsToAcceptWorkshopLegalAgreement) {
		std::cout << "Item update submitted!" << std::endl;

		success = true;
	}
	else {
		success = false;
	}

	currentItem = nullptr;
}

PublishedFileId_t ReadLinePublishedFileId(std::ifstream& stream)
{
	std::string line;
	std::getline(stream, line);

	std::istringstream iss(line);
	PublishedFileId_t number;
	if (!(iss >> number))
		return -1;

	return number;
}

int ReadLineInt(std::ifstream& stream)
{
	std::string line;
	std::getline(stream, line);

	std::istringstream iss(line);
	int number;
	if (!(iss >> number))
		return -1;

	return number;
}

int main(int argc, char** argv)
{
	if (!SteamAPI_Init()) {
		std::cerr << "Failure to initialize steam" << std::endl;
		return EXIT_FAILURE;
	}

	if (argc > 1) {
		std::string type(argv[1]);
		std::string mod_path = "P:\\Mods\\";

		WorkshopItemInfo mod1;

		mod1.name = "DayZ-Editor";
		mod1.path = mod_path + "@" + mod1.name + "\\";
	
		std::ifstream version_file("P:\\Editor\\Workbench\\version");

		if (version_file.is_open()) {
			std::string version;
			std::getline(version_file, version);

			mod1.version = version;
		}

		if (type.size() > 0 && std::strcmp(type.c_str(), "release") != 0) {
			std::string u_type(type);
			u_type[0] = std::toupper(u_type[0]);
			mod1.name += "-" + u_type;
		}

		SteamHandler handler;

		std::ifstream workshop_file(type);

		if (!workshop_file.is_open()) {
			handler.CreateItem(mod1);
			if (!handler.success) {
				std::cerr << "Failure to create workshop item for " << mod1.name << std::endl;
				return EXIT_FAILURE;
			}

			mod1.visibility = ERemoteStoragePublishedFileVisibility::k_ERemoteStoragePublishedFileVisibilityPrivate;

			std::ofstream workshop_file_save(type);
			if (workshop_file_save.is_open()) {
				workshop_file_save << std::to_string(mod1.fileID) << std::endl;
				workshop_file_save << std::to_string(mod1.visibility) << std::endl;

				workshop_file_save.close();
			}
			else {
				std::cerr << "Failure to save workshop file" << std::endl;
				return EXIT_FAILURE;
			}
		}
		else {
			mod1.fileID = ReadLinePublishedFileId(workshop_file);
			mod1.visibility = (ERemoteStoragePublishedFileVisibility)ReadLineInt(workshop_file);

			workshop_file.close();
		}

		std::string changelog;
		std::ifstream changelog_file("changelog.txt");

		if (changelog_file.is_open()) {
			std::string line;
			while (std::getline(changelog_file, line)) {
				changelog += line + '\n';
			}
		}

		handler.UpdateItem(mod1, changelog);
		if (!handler.success) {
			std::cerr << "Failure to update workshop item for " << mod1.name << std::endl;
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}