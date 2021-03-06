#include <ScriptController.h>
#include <../tinydir/tinydir.h>
#include <fstream>
#include <iostream>
#if __APPLE__
#include <CoreServices/CoreServices.h>
#else
#include <shlobj.h>
#endif

ScriptController* ScriptController::_shared = nullptr;
std::map<std::string, enumKeyCodes> Keybind::customKeys = {};

Keybind Keybind::fromArray(std::vector<std::string> v) {

	if (Keybind::customKeys.size() == 0) {
		Keybind::customKeys["left"] = KEY_Left;
		Keybind::customKeys["right"] = KEY_Right;
		Keybind::customKeys["up"] = KEY_Up;
		Keybind::customKeys["down"] = KEY_Down;
	}


	Keybind ret = {'q', false, false, false, false};

	for (auto& i : v) {
		if (i == "ctrl" || i == "cmd") {
			ret.ctrl = true;
		} else if (i == "alt" || i == "option") {
			ret.alt = true;
		} else if (i == "shift") {
			ret.shift = true;
		} else {
			if (ret.exists) {
				ret.exists = false;
				return ret;
			}
			ret.exists = true;

			if (i.size() == 1) {
				ret.key = i[0];
			} else if (Keybind::customKeys.count(i)) {
				ret.key = Keybind::customKeys[i];
			} else {
				ret.exists = false;
			}
		}
	}

	return ret;
}

Keybind::operator std::string() const {
	std::string out = "";

	if (!exists) return out;
	if (ctrl) out += "ctrl+";
	if (alt) out += "alt+";
	if (shift) out += "shift+";

	for (auto p : Keybind::customKeys) {
		if (p.second == key)
			return out+p.first;
	}
	return out+key;
}

std::string ScriptController::plugFolder() {
	#ifdef _WIN32

	  	LPITEMIDLIST pidl = 0;
	  	char buffer[_MAX_PATH];
	  	SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);      
	  	*buffer = 0;

	    SHGetPathFromIDList(pidl, buffer);
	    CoTaskMemFree(pidl);

		return std::string(buffer) + "\\GeometryDash\\scripts\\";
	#else
		FSRef ref;
		char path[PATH_MAX];
		FSFindFolder( kUserDomain, kApplicationSupportFolderType, kCreateFolder, &ref );
		FSRefMakePath( &ref, (UInt8*)&path, PATH_MAX );

		return std::string(path) + "/GeometryDash/scripts/";

	#endif
}


static auto read_file(std::string path) -> std::string {
	std::string ret = "epic fail";
	std::ifstream h(path, std::ios::ate);

	if (h.is_open()) {
		ret.resize(h.tellg());
		h.seekg(0u, std::ios::beg);
		h.read(ret.data(), ret.size());
	}

	return ret;
}

bool ScriptController::refreshFiles(bool err) {
	std::vector<std::string> files;
	std::string json;
	bool found = false;


	tinydir_dir dir;
	if (tinydir_open(&dir, plugFolder().c_str()) == -1) {

		#if _WIN32
		SHCreateDirectoryEx( NULL, plugFolder().c_str(), NULL );
		#else
		//todo fix this
		mkdir(plugFolder().c_str(), 0777);
		//if (err) FLAlertLayer::create(NULL, "Error parsing config", "Ok", NULL, "Cannot access script folder")->show();
		//return false;
		#endif
	}
	while (dir.has_next) {
		tinydir_file file;
		tinydir_readfile(&dir, &file);
		if (!file.is_dir) {
			if (std::string(file.name) == "config.json") {
				json = read_file(std::string(file.path));
				found = true;
			}
			files.push_back(std::string(file.name));
		}
		tinydir_next(&dir);
	}
	tinydir_close(&dir);

	if (!found) {

		std::ofstream fp(plugFolder()+"/config.json");
		json = "{\n    \"bindings\": {}\n}";
		fp << json;
		fp.close();
	}
	rapidjson::Document document;

	if (document.Parse<rapidjson::kParseDefaultFlags>(json.c_str()).HasParseError()) {
		printf("insane: %s\n", json.c_str());
		#if __APPLE__
		auto st = std::string(document.GetParseError());
		#else
		auto st = std::string(rapidjson::GetParseError_En(document.GetParseError()));
		#endif

		if (err) FLAlertLayer::create(NULL, "Error parsing config", "Ok", NULL, st)->show();
	    return false;
	}

	if (!document.HasMember("bindings")) {
			printf("the fuckig bindings \n");
			if (err) FLAlertLayer::create(NULL, "Error parsing config", "Ok", NULL, "unable to find bindings")->show();
		    return false;
	}

	auto& bindings = document["bindings"];

	std::map<Keybind, std::string> binds;
	static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };

	#if __APPLE__
	for (auto i = bindings.MemberonBegin(); i != bindings.MemberonEnd(); ++i) 
	#else
	for (auto i = bindings.MemberBegin(); i != bindings.MemberEnd(); ++i)
	#endif
	{
		std::vector<std::string> in;
		if (!i->value.IsArray()) {
			printf("expected array found %s\n", kTypeNames[i->value.GetType()]);
			if (err) FLAlertLayer::create(NULL, "Error parsing config", "Ok", NULL, "Expected array")->show();
		    return false;
		}

		#if __APPLE__
		for (auto j = i->value.onBegin(); j != i->value.onEnd(); ++j) 
		#else
		for (auto j = i->value.Begin(); j != i->value.End(); ++j) 
		#endif
		{
			if (!j->IsString()) {
					printf("expected string found %s\n", kTypeNames[j->GetType()]);
					if (err) FLAlertLayer::create(NULL, "Error parsing config", "Ok", NULL, "Expected string")->show();
				    return false;
			}
			in.push_back(j->GetString());
		}

		if (std::find(files.begin(), files.end(), i->name.GetString()) == files.end()) {
				printf("file doesn't exist dumb as s\n");
				if (err) FLAlertLayer::create(NULL, "Error parsing config", "Ok", NULL, std::string("Cannot find file ") + i->name.GetString())->show();
			    return false;
		}

		binds[Keybind::fromArray(in)] = i->name.GetString();
		printf("new item\n");
	}

	m_keybinds = binds;
	return true;
}
