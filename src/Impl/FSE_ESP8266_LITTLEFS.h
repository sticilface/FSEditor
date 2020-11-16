#pragma once
#include "FSEditor.h"
#include <LittleFS.h>
#include <FS.h>
#include <list>
#include <utility>


class ESP8266_LITTLEFS_Impl 
{
public:
	friend FSEditor<ESP8266_LITTLEFS_Impl>; 
	using FSType = fs::FS; 
	using FSFileType = fs::File; 
	using FSListType = std::list<std::pair<const String, const size_t>>;
	using FSCbType   = std::function<void(File & f)>; 

	ESP8266_LITTLEFS_Impl(ESP8266_LITTLEFS_Impl::FSType & fs): _fs(fs) {}
	bool remove(const String & name); 
	bool isValidFile(const String & fname, bool cache = false); 
	FSListType getFileList(const String & rootDir); 
private:
	void _fileListIterator(const String & dirName, FSCbType Cb);
	FSType & _fs; 
	FSFileType _cachedFile; 
}; 


using FSEditor_ESP8266_LITTLEFS = FSEditor < ESP8266_LITTLEFS_Impl >; 
