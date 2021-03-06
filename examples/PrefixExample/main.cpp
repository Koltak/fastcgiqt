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
#include "PrefixExample.h"
#include <FastCgiQt/Manager.h>
#include <FastCgiQt/PrefixMapper.h>

#include <QCoreApplication>

int main(int argc, char** argv)
{
	QCoreApplication application(argc, argv);
	application.setApplicationName("Dumper");

	FastCgiQt::Manager manager;
	FastCgiQt::PrefixMapper mapper;
	PrefixExample responder;

	mapper.connect(
		&manager,
		SIGNAL(newRequest(FastCgiQt::Request*)),
		SLOT(respond(FastCgiQt::Request*))
	);
	mapper.addMapping("foo", &responder, SLOT(foo(FastCgiQt::Request*)));
	mapper.addMapping("bar", &responder, SLOT(bar(FastCgiQt::Request*)));
	mapper.addMapping("baz", &responder, SLOT(baz(FastCgiQt::Request*)));

	return application.exec();
}
