#pragma once
#include <ESPAsyncWebServer.h>
#include <FS.h>



template<typename implT>
class FSEditor: public AsyncWebHandler {
  private:
    implT _impl;
    const String _hostUrl;
    const String _username;
    const String _password; 
    bool _authenticated;
    uint32_t _startTime;
  public:

    FSEditor(const String& hostUrl, typename implT::FSType & fs, const String& username=String(), const String& password=String() );   
    virtual bool canHandle(AsyncWebServerRequest *request) override final;
    virtual void handleRequest(AsyncWebServerRequest *request) override final;
    virtual void handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) override final; 
    virtual bool isRequestHandlerTrivial() override final {return false;}

  private:
};


#include "FSEditor.hpp"
