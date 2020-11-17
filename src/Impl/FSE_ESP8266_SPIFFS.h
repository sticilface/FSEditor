#pragma once
#include "FSEditor.h"
#include "FSEFSBase.h"
#include <FS.h>
#include <list>
#include <utility>
#include <functional>

class ESP8266_SPIFFS_Impl : public FSEFSBase
{
public:
	friend FSEditor<ESP8266_SPIFFS_Impl>; 
	using FSListType = FSE::FSListType;

	ESP8266_SPIFFS_Impl(ESP8266_SPIFFS_Impl::FSType & fs): FSEFSBase(fs) {}
	bool remove(const String & name);  
	FSListType getFileList(const String & rootDir); 

private:
	// void _fileListIterator(const String & dirName, FSCbType Cb);
}; 



using FSEditor_ESP8266_SPIFFS = FSEditor < ESP8266_SPIFFS_Impl >; 


// #if defined(ESP8266)
//      using FSEditor_ESP8266_SPIFFS = FSEditor < &FSE::ESP8266_SPIFFS_Imple>; 
//      using FSEditor_ESP8266_LITTLEFS = FSEditor < &FSE::ESP8266_LittleFS_Imple>; 
// #endif