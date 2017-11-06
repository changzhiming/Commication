#ifndef WTGLOBAL
#define WTGLOBAL
#include <QString>
#include <QDebug>

#if defined(WT_BUILD)
#define WT_EXPORT Q_DECL_EXPORT
#else
#define WT_EXPORT Q_DECL_IMPORT
#endif

#endif // WTGLOBAL

