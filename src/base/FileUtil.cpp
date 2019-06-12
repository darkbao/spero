// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "FileUtil.h"
#include "Logging.h"    // for func@strerror_tl

using namespace spero;
using namespace FileUtil;

AppendFile::AppendFile(StringArg filename)
    : fp_(::fopen(filename.c_str(), "ae")),
      writtenBytes_(0)
{
    assert(fp_);
    ::setbuffer(fp_, buffer_, BUFFER_SIZE);
}
// 'e' for O_CLOEXEC, close all open file when exec after fork

AppendFile::~AppendFile()
{
    ::fclose(fp_);
}

size_t AppendFile::append(const char* logline, const size_t len)
{
    size_t n = ::fwrite_unlocked(logline, 1, len, fp_);
    size_t remain = len - n;
    while (remain > 0) {
        size_t x = ::fwrite_unlocked(logline + n, 1, remain, fp_);
        if (x == 0) {
            int err = ::ferror(fp_);
            if (err) {
                fprintf(stderr, "AppendFile::append() failed %s\n", strerror_tl(err));
            }
            break;
        }
        n += x;
        remain = len - n;
    }
    writtenBytes_ += n;
    return n;
}

void AppendFile::flush()
{
    ::fflush(fp_);
}