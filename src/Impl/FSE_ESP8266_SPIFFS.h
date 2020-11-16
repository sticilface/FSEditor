#pragma once
#include "FSEditor.h"
#include <FS.h>
#include <list>
#include <utility>
#include <functional>

class ESP8266_SPIFFS_Impl 
{
public:
	friend FSEditor<ESP8266_SPIFFS_Impl>; 
	using FSType     = fs::FS; 
	using FSFileType = fs::File; 
	using FSListType = std::list<std::pair<const String, const size_t>>;
	using FSCbType   = std::function<void(File & f)>; 
	
	ESP8266_SPIFFS_Impl(ESP8266_SPIFFS_Impl::FSType & fs): _fs(fs) {}
	bool remove(const String & name);  
	bool isValidFile(const String & fname, bool cache = false); 
	FSListType getFileList(const String & rootDir); 

private:
	void _fileListIterator(const String & dirName, FSCbType Cb);
	FSType & _fs; 
	FSListType _list; 
	FSFileType _cachedFile; 
}; 



using FSEditor_ESP8266_SPIFFS = FSEditor < ESP8266_SPIFFS_Impl >; 


// #if defined(ESP8266)
//      using FSEditor_ESP8266_SPIFFS = FSEditor < &FSE::ESP8266_SPIFFS_Imple>; 
//      using FSEditor_ESP8266_LITTLEFS = FSEditor < &FSE::ESP8266_LittleFS_Imple>; 
// #endif