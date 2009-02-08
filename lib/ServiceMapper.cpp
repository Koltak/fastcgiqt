#include "ServiceMapper.h"

#include "Service.h"

#include <QDebug>
#include <QRegExp>
#include <QStringList>
#include <QThread>

namespace FastCgiQt
{
	class ServiceMapper::Private
	{
		public:
			typedef QMap<QString, Service*> ServiceMap;
			static QMap<QThread*, ServiceMap> services;
	};
	QMap<QThread*, ServiceMapper::Private::ServiceMap> ServiceMapper::Private::services;

	ServiceMapper::ServiceMapper(const Request& request, QIODevice* socket, QIODevice* inputDevice, QObject* parent)
		:
			Responder(request, socket, inputDevice, parent),
			d(new Private)
	{
	}

	void ServiceMapper::addService(const QString& serviceName, Service* service)
	{
		d->services[QThread::currentThread()].insert(serviceName, service);
	}

	void ServiceMapper::respond()
	{
		if(d->services.value(QThread::currentThread()).isEmpty())
		{
			loadServices();
		}
		Private::ServiceMap services = d->services.value(QThread::currentThread());

		QStringList parts = request.serverData("PATH_INFO").split("/").filter(QRegExp("."));;
		const QString serviceName(parts.isEmpty() ? "" : parts.takeFirst());

		for(
			Private::ServiceMap::ConstIterator it = services.constBegin();
			it != services.constEnd();
			++it
		)
		{
			if(it.key() == serviceName || (parts.isEmpty() && it.key().isEmpty() && serviceName.isEmpty()))
			{
				Service* service = it.value();

				// copy over the ClientIOInterface parts
				// Trust me, I'm a friend class.
				*const_cast<Request*>(&service->request) = *const_cast<Request*>(&this->request);
				service->out.setDevice(out.device());
				service->in.setDevice(in.device());

				// do the actual request
				it.value()->dispatchRequest(parts.join("/"));

				// flush the text stream
				service->out << flush;
				// reset devices
				service->in.setDevice(NULL);
				service->out.setDevice(NULL);
				return;
			}
		}
		errorMessage(404);
		return;
	}

	void ServiceMapper::errorMessage(int code)
	{
		setHeader("status", QString("%1 BADNESS"));

		out <<
			"<html>\n"
			"<head>\n"
			"<title>" << tr("HTTP Error %1").arg(code) << "</title>\n"
			"</head>\n"
			"<body>\n"
			"<h1>" << tr("HTTP Error %1").arg(code) << "</h1>\n"
			"<p>" << 
			tr("Error %1 occured while trying to find the page you "
			   "requested.").arg(code) << 
			"</p>\n"
			"</body>\n"
			"</html>\n"
		;
	}

	ServiceMapper::~ServiceMapper()
	{
		delete d;
	}
}