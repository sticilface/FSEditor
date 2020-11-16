#ifdef ESP8266
#include "FSE_ESP8266_SPIFFS.h"


bool ESP8266_SPIFFS_Impl::isValidFile(const String & fname, bool cache)
{
	bool result = false; 
	if (_fs.exists(fname)) {
		_cachedFile = _fs.open(fname, "r");
		if (_cachedFile) {
			result = true;
		}
	}

	if (_cachedFile && !cache) {
		_cachedFile.close(); 
	}

	return result; 
}

ESP8266_SPIFFS_Impl::FSListType ESP8266_SPIFFS_Impl::getFileList(const String & rootDir)
{
	FSListType list;
	_fileListIterator(rootDir, [&list](File & f){ 
		list.push_back(std::pair<String, size_t>(f.name(), f.size())); 
	});
	return list; 
}

bool ESP8266_SPIFFS_Impl::remove(const String & name)
{
	return _fs.remove(name); 
}

void ESP8266_SPIFFS_Impl::_fileListIterator(const String & dirName, FSCbType Cb)
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
