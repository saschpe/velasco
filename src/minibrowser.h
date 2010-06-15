#ifndef __VELASCO_MINI_BROWSER_H__
#define __VELASCO_MINI_BROWSER_H__

#include "config.h"
#include "browsertabwidget.h"

#include <QtGui/QMainWindow>

class QLineEdit;
class QToolBar;
class QAction;
class QUrl;

class MiniBrowser : public QMainWindow
{
	Q_OBJECT

public:
	MiniBrowser();

private Q_SLOTS:
	void updateUrl();
	void updateUrlWidget(const QUrl&);
	void updateWindowTitle(const QString&);

private:
	BrowserTabWidget *m_webView;
	QLineEdit *m_editUrl;
	QToolBar *m_navToolBar;

	QAction *m_actionBackward;
	QAction *m_actionForward;
	QAction *m_actionReload;
	QAction *m_actionStop;
};

#endif
