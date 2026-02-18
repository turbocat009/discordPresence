#pragma once

namespace debug {
    inline bool drawCollisions = false;


    inline void setDebugOptions(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "-eC") {
                debug::drawCollisions = true;
            }
        }
    }
}
