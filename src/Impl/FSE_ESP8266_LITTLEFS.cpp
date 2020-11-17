#ifdef ESP8266
#include "FSE_ESP8266_LITTLEFS.h"


ESP8266_LITTLEFS_Impl::FSListType ESP8266_LITTLEFS_Impl::getFileList(const String & rootDir)
{
	FSListType list;
	fileListIterator(rootDir, [&list](File & f){ 
		list.push_back(std::pair<String, size_t>(f.fullName(), f.size())); 
	});

	return list; 
}

bool ESP8266_LITTLEFS_Impl::remove(const String & name)
{
	return _fs.remove(name); 
	//  maybe ? how to remove empty directory here.... 
}  

#endif


