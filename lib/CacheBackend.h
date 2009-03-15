#ifndef _FASTCGI_QT_CACHE_BACKEND_H
#define _FASTCGI_QT_CACHE_BACKEND_H

#include "CacheEntry.h"

namespace FastCgiQt
{
	class CacheBackend
	{
		public:
			virtual void remove(const QString& key) = 0;
			virtual CacheEntry value(const QString& key) const = 0;
			virtual void setValue(const QString& key, const CacheEntry& entry) = 0;

			class Factory
			{
				public:
					virtual CacheBackend* getCacheBackend() const = 0;
			};
	};
}

#endif
