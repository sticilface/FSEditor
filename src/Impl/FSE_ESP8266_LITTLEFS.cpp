#ifdef ESP8266
#include "FSE_ESP8266_LITTLEFS.h"


bool ESP8266_LITTLEFS_Impl::isValidFile(const String & fname, bool cache)
{
	//Serial.printf("isVAlid called: %s\n", fname.c_str()); 
	bool result = false; 
	if (_fs.exists(fname)) {
		_cachedFile = _fs.open(fname, "r");
		if (_cachedFile && !_cachedFile.isDirectory()) {
			result = true;
		}
	}

	if (_cachedFile && !cache) {
		//Serial.println("Closing File"); 
		_cachedFile.close(); 
	}
	//Serial.printf("result = %s\n", (result)? "true": "false"); 
	return result; 
}


ESP8266_LITTLEFS_Impl::FSListType ESP8266_LITTLEFS_Impl::getFileList(const String & rootDir)
{
	FSListType list;
	_fileListIterator(rootDir, [&list](File & f){ 
		list.push_back(std::pair<String, size_t>(f.fullName(), f.size())); 
	});

	return list; 
}

bool ESP8266_LITTLEFS_Impl::remove(const String & name)
{
	return _fs.remove(name); 
	//  maybe ? how to remove empty directory here.... 
}  

void ESP8266_LITTLEFS_Impl::_fileListIterator(const String & dirName, FSCbType Cb)
{
    Dir dir = _fs.openDir(dirName);
    while (dir.next())
    {
        if (dir.isFile())
        {
            File f =  dir.openFile("r");
            if (Cb)
            {
                Cb(f);
            }
        }
        else
        {
            _fileListIterator(dir.fileName().c_str(), Cb);
        }
    }
}

#endif


