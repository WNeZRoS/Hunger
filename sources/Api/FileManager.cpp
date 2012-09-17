#include "FileManager.h"
#include "compatibility.h"
#include <cstring>

FileManager::FileManager() {
	_pwd = "./";
}

FileManager& FileManager::instance() {
	static FileManager manager;
	return manager;
}

void FileManager::changeDirectory(const char *dir) {
	_pwd = dir;
}

std::ifstream* FileManager::read(const char *filename, bool binary) {
	std::string path = _pwd + filename;
	return new std::ifstream(path.c_str(), binary ? std::ios::in | std::ios::binary : std::ios::in);
}

std::ofstream* FileManager::write(const char *filename, bool binary) {
	std::string path = _pwd + filename;
	return new std::ofstream(path.c_str(), binary ? std::ios::out | std::ios::binary : std::ios::out);
}

void FileManager::getPath(const char *filename, char *result) {
	std::string path = _pwd + filename;
	std::strcpy(result, path.c_str());
}
