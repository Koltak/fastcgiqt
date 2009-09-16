#include "XsltService.h"
#include "XsltServicePrivate.h"

#include <QBuffer>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QXmlFormatter>
#include <QXmlQuery>

namespace FastCgiQt
{
	XsltService::Private::Private()
		:
			source(NoXslt),
			prettyPrint(false)
	{
	}
	
	XsltService::XsltService(const FastCgiQt::Request& request, QObject* parent)
		:
			Service(request, parent),
			d(new Private()),
			xmlOut(&d->xml)
	{
		xmlOut.setAutoFormatting(true);
	}
	
	XsltService::~XsltService()
	{
		delete d;
	}
	
	void XsltService::setVariable(const QString& name, const QVariant& value)
	{
		d->variables[name] = value;
	}
	
	void XsltService::setXsltUrl(const QString& path)
	{
		if(path.isEmpty())
		{
			setXsltUrl(QUrl());
		}
		else
		{
			setXsltUrl(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/" + path));
		}
	}
	
	void XsltService::addXsltFile(const QString& fileName)
	{
		d->source = Private::StringXslt;
		d->xslt += readFile(fileName);
	}
	
	void XsltService::setXsltUrl(const QUrl& url)
	{
		d->xsltUrl = url;
		if(url.isValid())
		{
			d->source = Private::FileXslt;
		}
		else
		{
			d->source = Private::NoXslt;
		}
	}
	
	void XsltService::setPrettyPrintingEnabled(bool value)
	{
		d->prettyPrint = value;
	}
	
	bool XsltService::isPrettyPrintingEnabled() const
	{
		return d->prettyPrint;
	}
	
	void XsltService::finished()
	{
		if(d->source != Private::NoXslt)
		{
			xmlOut.writeEndDocument();
	
			QXmlQuery query(QXmlQuery::XSLT20);
	
			QByteArray data(d->xml.toUtf8());
			QBuffer buffer(&data);
			buffer.open(QIODevice::ReadOnly);
	
			bool haveSetFocus = query.setFocus(&buffer);
			Q_ASSERT(haveSetFocus);
			if(!haveSetFocus)
			{
				Service::finished();
				return;
			}
	
			query.bindVariable("root", QVariant(request.baseUri()));
			for(
				QVariantMap::ConstIterator it = d->variables.constBegin();
				it != d->variables.constEnd();
				++it
			)
			{
				query.bindVariable(it.key(), it.value());
			}
	
			if(header("content-type").startsWith("text/html"))
			{
				out
					<< "<!DOCTYPE html "
					<< "PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' "
					<< "'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>"
					<< endl;
			}
	
			QBuffer xsltBuffer(&d->xslt);
			if(Private::FileXslt == d->source)
			{
				query.setQuery(d->xsltUrl);
			}
			else
			{
				xsltBuffer.open(QIODevice::ReadOnly);
				query.setQuery(&xsltBuffer);
			}
	
			if(isPrettyPrintingEnabled())
			{
				QXmlFormatter formatter(query, out.device());
				query.evaluateTo(&formatter);
			}
			else
			{
				query.evaluateTo(out.device());
			}
	
			out << endl;
		}
		else if(!d->xml.isEmpty())
		{
			setHeader("content-type", "application/xml");
			xmlOut.writeEndDocument();
	
			out << d->xml << flush;
		}
		Service::finished();
	}
}
