#include "IndexService.h"

IndexService::IndexService(QObject* parent)
	: Service(parent)
{
}

void IndexService::index()
{
	out << "Index goes here";
}

FastCgiQt::Service::UrlMap IndexService::urlMap() const
{
	FastCgiQt::Service::UrlMap map;
	map.append("", SLOT(index()));
	return map;
}
