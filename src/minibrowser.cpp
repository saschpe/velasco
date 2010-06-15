#include "minibrowser.h"

#include <QtGui>

MiniBrowser::MiniBrowser()
{
	m_editUrl = new QLineEdit();
	connect(m_editUrl, SIGNAL(returnPressed()), this, SLOT(updateUrl()));

	//m_webView = new QWebView();
	//connect(m_webView, SIGNAL(urlChanged(const QUrl&)), this, SLOT(updateUrlWidget(const QUrl&)));
	//connect(m_webView, SIGNAL(titleChanged(const QString&)), this, SLOT(updateWindowTitle(const QString&)));

	m_webView= new BrowserTabWidget(this);

	m_actionBackward = new QAction(QIcon(":/images/actions/go-previous.png"), tr("&Backward"), this);
	m_actionBackward->setShortcut(tr("Alt+Right"));
	connect(m_actionBackward, SIGNAL(triggered()), m_webView, SLOT(backward()));
	
	m_actionForward = new QAction(QIcon(":/images/actions/go-next.png"), tr("&Forward"), this);
	m_actionForward->setShortcut(tr("Alt+Left"));
	connect(m_actionForward, SIGNAL(triggered()), m_webView, SLOT(forward()));
	
	m_actionStop = new QAction(QIcon(":/images/actions/process-stop.png"), tr("&Stop"), this);
	m_actionStop->setShortcut(tr("Esc"));
	connect(m_actionStop, SIGNAL(triggered()), m_webView, SLOT(stop()));
	
	m_actionReload = new QAction(QIcon(":/images/actions/view-refresh.png"), tr("&Reload"), this);
	m_actionReload->setShortcut(tr("F5"));
	connect(m_actionReload, SIGNAL(triggered()), m_webView, SLOT(reload()));

	QToolBar *m_navToolBar = new QToolBar(tr("Navigation"), this);
	m_navToolBar->setMovable(true);
	m_navToolBar->setFloatable(true);
	m_navToolBar->addAction(m_actionBackward);
	m_navToolBar->addAction(m_actionForward);
	m_navToolBar->addAction(m_actionStop);
	m_navToolBar->addAction(m_actionReload);
	m_navToolBar->addSeparator();
	m_navToolBar->addWidget(m_editUrl);
	m_navToolBar->addSeparator();

	QAction *find = new QAction(tr("&Find"), this);
	find->setShortcut(tr("Ctrl+F"));
	connect(find, SIGNAL(triggered()), m_webView, SLOT(find()));
	m_navToolBar->addAction(find);

	addToolBar(m_navToolBar);
	setCentralWidget(m_webView);
	setWindowTitle(tr("Minibrowser"));
}

void MiniBrowser::updateUrl()
{
	QUrl url(m_editUrl->text());
	if (url.scheme().isEmpty()) {
		url = QUrl("http://" + m_editUrl->text());
		m_editUrl->setText(url.toString());
	}

	m_webView->load(url);
}

void MiniBrowser::updateUrlWidget(const QUrl& url)
{
	m_editUrl->setText(url.toString());
}

void MiniBrowser::updateWindowTitle(const QString& title)
{
	if (title.isEmpty()) {
		setWindowTitle(tr("Minibrowser"));
	} else {
		setWindowTitle(tr("Minibrowser - %1").arg(title));
	}
}
