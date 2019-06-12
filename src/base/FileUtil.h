// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

#ifndef SPERO_BASE_FILEUTIL_H
#define SPERO_BASE_FILEUTIL_H

#include <sys/types.h>
#include "NonCopyable.hpp"
#include "StringPiece.hpp"

namespace spero
{

namespace FileUtil
{

// not thread safe
class AppendFile : public NonCopyable
{
public:
    explicit AppendFile(StringArg filename);
    ~AppendFile();
    
    void flush();
    size_t append(const char* logline, size_t len);
    inline off_t writtenBytes() const { return writtenBytes_; }

private:
    enum { BUFFER_SIZE = 64*1024 };
    
    FILE* fp_;
    char  buffer_[BUFFER_SIZE];
    off_t writtenBytes_;
};

}  // namespace FileUtil

}  // namespace spero

#endif  // SPERO_BASE_FILEUTIL_H

