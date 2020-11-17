#include "FSEFSBase.h"

bool FSEFSBase::isValidFile(const String & fname, bool cache)
{
	bool result = false; 
	if (_fs.exists(fname)) {
		_cachedFile = _fs.open(fname, "r");
		if (_cachedFile && !_cachedFile.isDirectory()) {
			result = true;
		}
	}
	if (_cachedFile && !cache) {
		_cachedFile.close(); 
	}
	return result; 
} 


void FSEFSBase::fileListIterator(const String & dirName, FSCbType Cb)
{
    Dir dir = _fs.openDir(dirName);
    while (dir.next())
    {
        if (dir.isFile())
        {
            File f =  dir.openFile("r");
            if (Cb && f)
            {
                Cb(f);
                f.close(); 
            }
        }
        else
        {
            fileListIterator(dir.fileName(), Cb);
        }
    }
}