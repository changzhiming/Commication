#ifndef EMAIL_GLOBAL_H
#define EMAIL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EMAIL_LIBRARY)
#  define EMAILSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EMAILSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EMAIL_GLOBAL_H
