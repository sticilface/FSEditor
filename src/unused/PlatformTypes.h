#pragma once

namespace FSE {

enum class Platform_enum {
	esp32,
	esp8266
};

struct Platform {
    Platform_enum type;
};

 static constexpr Platform ESP8266_P{Platform_enum::esp8266};
 static constexpr Platform ESP32_P{Platform_enum::esp32};

}; 

