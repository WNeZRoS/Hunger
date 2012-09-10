#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <fstream>
#include <string>

class FileManager
{
public:
	static FileManager& instance();
	void changeDirectory(const char *dir);

	std::ifstream* read(const char *filename, bool binary = false);
	std::ofstream* write(const char *filename, bool binary = false);
private:
	FileManager();

	std::string _pwd;
};

#endif // FILEMANAGER_H
