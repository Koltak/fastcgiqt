/* LICENSE NOTICE
	Copyright (c) 2009, Frederick Emmott <mail@fredemmott.co.uk>

	Permission to use, copy, modify, and/or distribute this software for any
	purpose with or without fee is hereby granted, provided that the above
	copyright notice and this permission notice appear in all copies.

	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
	WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
	MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
	ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
	WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
	ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
	OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
#ifndef _FASTCGIQT_HTTPINTERFACE
#define _FASTCGIQT_HTTPINTERFACE

#include "CommunicationInterface.h"

class QTcpServer;

namespace FastCgiQt
{
	class HttpRequest;
	class HttpInterface : public CommunicationInterface
	{
		Q_OBJECT
		public:
			/// Create a HttpInterface object.
			HttpInterface(QObject* parent = NULL);
			~HttpInterface();
			bool isFinished() const;
			QStringList backends() const;
			virtual bool requiresConfiguration(const QString& backend) const;
			void configureHttpd(const QString& backend);
		protected:
			bool startBackend(const QString& backend);
		private slots:
			void startResponse();
			void announceRequest(HttpRequest* request);
		private:
			QTcpServer* m_server;
			QString m_staticDirectory;
	};
};
#endif // _FASTCGIQT_HTTPINTERFACE
