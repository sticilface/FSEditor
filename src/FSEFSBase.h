#pragma once
#include <FS.h>


class FSEFSBase {
public:
	using FSType     = fs::FS; 
	using FSFileType = fs::File; 
	using FSCbType   = std::function<void(File & f)>; 	
	FSEFSBase(FSType & fs) : _fs(fs) {}; 
protected:
	virtual bool isValidFile(const String & fname, bool cache = false);  
	virtual void fileListIterator(const String & dirName, FSCbType Cb);
	FSType & _fs; 
	FSFileType _cachedFile; 
private:

};
