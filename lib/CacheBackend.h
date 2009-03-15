#ifndef _FASTCGI_QT_CACHE_BACKEND_H
#define _FASTCGI_QT_CACHE_BACKEND_H

#include "CacheEntry.h"

#include <QtPlugin>

class QReadWriteLock;

namespace FastCgiQt
{
	class CacheBackend
	{
		public:
			virtual void remove(const QString& key) = 0;
			virtual CacheEntry value(const QString& key) const = 0;
			virtual void setValue(const QString& key, const CacheEntry& entry) = 0;

			virtual QReadWriteLock* readWriteLock() const = 0;

			class Factory
			{
				public:
					virtual void loadSettings();
					virtual CacheBackend* getCacheBackend(const QString& cacheName) const = 0;
			};
	};
}

Q_DECLARE_INTERFACE(FastCgiQt::CacheBackend::Factory, "uk.co.fredemmott.FastCgiQt.CacheBackend/1.0");

#endif
