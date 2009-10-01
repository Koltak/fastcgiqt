#pragma once

#include "PrefixMapper.h"

#include <QByteArray>
#include <QHash>

namespace FastCgiQt
{
	class PrefixMapper::Private
	{
		public:
			struct Receiver
			{
				Receiver(SpawnerBase* _spawner, const char* _slot)
				: spawner(_spawner)
				, slot(_slot)
				{
				}

				Receiver()
				: spawner(0)
				, slot(0)
				{
				}

				SpawnerBase* spawner;
				const char* slot;

				operator bool() const { return slot && spawner; }
			};
			typedef QHash<QByteArray, Receiver> ServiceMap;
			ServiceMap services;
			Responder* responder;
	};
};
