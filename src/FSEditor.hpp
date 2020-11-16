bool isExcluded(fs::FS &_fs, const char *filename);

extern const size_t  edit_htm_gz_len;
extern const uint8_t edit_htm_gz[];

template<typename implT>
FSEditor<implT>::FSEditor(const String& hostUrl, typename implT::FSType & fs, const String& username, const String& password )
	: _impl(fs)
	, _hostUrl(hostUrl)
	, _username(username)
	, _password(password)
	, _authenticated(false)
	, _startTime(0)
{ };



template<typename implT>
bool FSEditor<implT>::canHandle(AsyncWebServerRequest *request)
{
//	Serial.printf("Req:url = %s\n", request->url().c_str());
	// int params = request->params();
 //    for (int i = 0; i < params; i++) {
 //        AsyncWebParameter* p = request->getParam(i);
 //        if (p->isFile()) { //p->isPost() is also true
 //            Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
 //        } else if (p->isPost()) {
 //            Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
 //        } else {
 //            Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
 //        }
 //    }

	if (request->url().equalsIgnoreCase( _hostUrl ) ) {
		if (request->method() == HTTP_GET) {
			if (request->hasParam("list")) {
				return true;
			}
			if (request->hasParam("edit")) {
				return  _impl.isValidFile(request->arg("edit"), true );
			}
			if (request->hasParam("download")) {
				return _impl.isValidFile(request->arg("download"), true);
			}
			request->addInterestingHeader("If-Modified-Since");
			return true;
		} else if (request->method() == HTTP_POST) {
			return true;
		} else if (request->method() == HTTP_DELETE) {
			return true;
		} else if (request->method() == HTTP_PUT) {
			return true;
		}

	}
	//Serial.println("CanHandle = false");
	return false;

}



template<typename implT>
void FSEditor<implT>::handleRequest(AsyncWebServerRequest *request)
{
	if (_username.length() && _password.length() && !request->authenticate(_username.c_str(), _password.c_str())) {
		return request->requestAuthentication();
	}

	if (request->method() == HTTP_GET) {
		if (request->hasParam("list")) {
			String path = request->getParam("list")->value();
			auto fileList = _impl.getFileList(path);
			String output = "[";

			for (auto & fileInfo : fileList) {
				if (!isExcluded(_impl._fs, fileInfo.first.c_str())) {
					if (output != "[") { output += ','; }
					output += "{\"type\":\"";
					output += "file";
					output += "\",\"name\":\"";
					output += fileInfo.first;
					output += "\",\"size\":";
					output += String(fileInfo.second);
					output += "}";
				}
			}
			output += "]";
			//Serial.println(output);
			request->send(200, "application/json", output);
			output = String();
		} else if (request->hasParam("edit") || request->hasParam("download")) {
			//Serial.printf("sending: %s [%u]\n", _impl._cachedFile.name(), _impl._cachedFile.size());			
			request->send( _impl._cachedFile, _impl._cachedFile.name(), String(), request->hasParam("download"));
		} else {
			const char * buildTime = __DATE__ " " __TIME__ " GMT";
			if (request->header("If-Modified-Since").equals(buildTime)) {
				request->send(304);
			} else {
				AsyncWebServerResponse *response{nullptr}; 
				if (_impl.isValidFile("/edit.htm", true)) {
						response = request->beginResponse(_impl._cachedFile, _impl._cachedFile.name(), "text/html" , false);					
				} 
				if (!response) {
					response = request->beginResponse_P(200, "text/html", edit_htm_gz, edit_htm_gz_len);
					if (response) {
						response->addHeader("Content-Encoding", "gzip");
						response->addHeader("Last-Modified", buildTime);
					}
				}
				if (response) {					
					request->send(response);					
				} else {
					//Serial.println("no response Ptr");
					request->send(500); 
				}
			}
		}
	} else if (request->method() == HTTP_DELETE) {
		if (request->hasParam("path", true)) {
			_impl.remove(request->getParam("path", true)->value());
			request->send(200, "", "DELETE: " + request->getParam("path", true)->value());
		} else {
			request->send(404);
		}
	} else if (request->method() == HTTP_POST) {
		if (request->hasParam("data", true, true) && _impl.isValidFile(request->getParam("data", true, true)->value())) {
			request->send(200, "", "UPLOADED: " + request->getParam("data", true, true)->value());
		} else {
			request->send(500);
			//Serial.printf("Failed: hasDataparam = %s\n", (request->hasParam("data", true, true)?"true":"false"));
			//Serial.printf("Failed: isValidFile = %s\n", (_impl.isValidFile(request->getParam("data", true, true)->value())?"true":"false"));
		}

	} else if (request->method() == HTTP_PUT) {
		 if(request->hasParam("path", true)){
		  String filename = request->getParam("path", true)->value();
		  if(_impl.isValidFile(filename)){
		    request->send(200);
		  } else {
		    fs::File f = _impl._fs.open(filename, "w");
		    if(f){
		      f.write((uint8_t)0x00);
		      f.close();
		      request->send(200, "", "CREATE: "+filename);
		    } else {
		      request->send(500);
		    }
		  }
		 } else {
		   request->send(400);
		 }
	}
}



template<typename implT>
void FSEditor<implT>::handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)
{
	if (!index) {
		if (!_username.length() || request->authenticate(_username.c_str(), _password.c_str())) {
			_authenticated = true;
			_impl._cachedFile = _impl._fs.open(filename, "w");
			_startTime = millis();
		}
	}
	if (_authenticated && _impl._cachedFile) {
		if (len) {
			_impl._cachedFile.write(data, len);
		}
		if (final) {
			_impl._cachedFile.close();
		}
	}

}

