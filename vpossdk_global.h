#ifndef VPOSSDK_GLOBAL_H
#define VPOSSDK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VPOSSDK_LIBRARY)
#  define VPOSSDKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define VPOSSDKSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // VPOSSDK_GLOBAL_H
