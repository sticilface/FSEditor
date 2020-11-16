#pragma once

namespace FSE {

enum class FS_enum {
	spiffs,
	littlefs
};

struct FSType {
    FS_enum type;
};

 static constexpr FSType SPIFFS_T{FS_enum::spiffs};
 static constexpr FSType LITTLEFS_T{FS_enum::littlefs};

}; 