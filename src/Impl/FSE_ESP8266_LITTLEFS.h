#pragma once
#include "FSEditor.h"
#include "FSEFSBase.h"
#include <LittleFS.h>
#include <FS.h>



class ESP8266_LITTLEFS_Impl : public FSEFSBase
{
public:
	friend FSEditor<ESP8266_LITTLEFS_Impl>; 
	using FSListType = FSE::FSListType;

	ESP8266_LITTLEFS_Impl(ESP8266_LITTLEFS_Impl::FSType & fs): FSEFSBase(fs) {}
	bool remove(const String & name); 
	FSListType getFileList(const String & rootDir); 
private:
	void _fileListIterator(const String & dirName, FSCbType Cb);

}; 


using FSEditor_ESP8266_LITTLEFS = FSEditor < ESP8266_LITTLEFS_Impl >; 
