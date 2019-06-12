#ifndef SPERO_BASE_LOGGING_H
#define SPERO_BASE_LOGGING_H

#include <string.h>

namespace spero
{

    const char* strerror_tl(int savedErrno);

} // namespace spero

#endif // SPERO_BASE_LOGGING_H