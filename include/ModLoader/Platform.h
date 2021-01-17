//
// Author: Ciel DeVille
// Version 1.0.0
// Stability: experimental
//

#ifndef MODLOADER_PLATFORM_H
#define MODLOADER_PLATFORM_H

#if defined(_WIN32) || defined(_WIN64)
#   define MODLOADER_WINDOWS
#elif defined(__linux__)
#   define MODLOADER_LINUX
#else
#   error "Unsupported platform detected ; please build on either Windows or Linux"
#endif

#endif //MODLOADER_PLATFORM_H
