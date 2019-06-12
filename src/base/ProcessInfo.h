#ifndef SPERO_BASE_PROCESSINFO_H
#define SPERO_BASE_PROCESSINFO_H

#include <sys/types.h>
#include <string>

namespace spero
{

namespace ProcessInfo
{
    pid_t pid();

    std::string hostname();
}

} // namespace spero

#endif // SPERO_BASE_PROCESSINFO_H