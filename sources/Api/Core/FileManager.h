#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <fstream>
#include <string>

class FileManager
{
public:
	static FileManager& instance();
	void changeDirectory(const char *dir);

	/**
	  Open file with name filename from current directory for read.
	  @param filename
	  filename to open
	  @param binary
	  open file with binary flag
	  @return stl input stream pointer or NULL. You need to delete this pointer.
	  */
	std::ifstream* read(const char *filename, bool binary = false);

	/**
	  Open file with name filename from current directory for write.
	  @param filename
	  filename to open
	  @param binary
	  open file with binary flag
	  @return stl output stream pointer or NULL. You need to delete this pointer.
	  */
	std::ofstream* write(const char *filename, bool binary = false);

	/**
	  Get file path for filename
	  @param filename filename
	  @param result
	  result of "Current directory" + filename. You must use not defined char* (char *a = 0) or get memory leaks.
	  */
	void getPath(const char *filename, char*& result);
private:
	FileManager();

	std::string _pwd;
};

#endif // FILEMANAGER_H
