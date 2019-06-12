#ifndef SPERO_BASE_LOGFILE_H
#define SPERO_BASE_LOGFILE_H

#include <string>
#include <memory>
#include "NonCopyable.hpp"
#include "Mutex.hpp"
#include "FileUtil.h"

namespace spero
{

using FileUtil::AppendFile;

class LogFile : public NonCopyable
{
public:
    LogFile(const std::string& basename,
            off_t rollSize,
            bool ThreadSafe = true,
            int flushInterval = 3,
            int checkEveryN = 1024);
    
    void append(const char* logline, int len);
    void flush();
    bool rollFile();
    // this func creates new file while:
    //      1. Old file is too big
    //      2. A new day has arrived

private:
    void append_unlocked(const char* logline, int len);
    static std::string getLogFileName(const std::string& basename, time_t* now);

private:
    const std::string basename_;
    const off_t       rollSize_;
    const int         flushInterval_;
    const int         checkEveryN_;

    int    count_;
    time_t startOfPeriod_;
    time_t lastRoll_;
    time_t lastFlush_;
    std::unique_ptr<MutexLock>  mutex_;
    std::unique_ptr<AppendFile> file_;

    const static int kRollPerSeconds_ = 60*60*24;
};

} // namespace spero
#endif // SPERO_BASE_LOGFILE_H