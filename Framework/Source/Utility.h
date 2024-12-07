#pragma once

namespace fw {

    double getHighPrecisionTime();
    double getTimeSinceGameStarted();

    void outputMessage(const char* message, ...);
    char* loadCompleteFile(const char* filename, long* length);

} // namespace fw
