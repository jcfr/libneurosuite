#ifndef KLUSTERSSHARED_EXPORT_H
#define KLUSTERSSHARED_EXPORT_H

#include <QtCore/qglobal.h>

#ifndef KLUSTERSSHARED_EXPORT
# if defined(KLUSTERSSHARED_MAKEDLL)
   /* We are building this library */
#  define KLUSTERSSHARED_EXPORT Q_DECL_EXPORT
# else
   /* We are using this library */
#  define KLUSTERSSHARED_EXPORT Q_DECL_IMPORT
# endif
#endif

#endif // KLUSTERSSHARED_EXPORT_H
