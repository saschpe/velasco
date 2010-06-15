/*
 * Velasco web browser
 * Copyright (C) 2008 Sascha Peilicke <sasch.pe@gmx.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __VELASCO_BROWSER_TAB_H__
#define __VELASCO_BROWSER_TAB_H__

#include <QtWebKit/QWebView>

class BrowserTabWidget;

/**
 *	Tab widget which shows a web page
 *
 *	@TODO: Zooming (QGraphicsView)
 */
class BrowserTab : public QWebView
{
	Q_OBJECT

public:
	BrowserTab(BrowserTabWidget *parent = 0);

	void loadUrl(const QUrl& url);

	QString getStatusBarText() const;
	bool isLoadingFinished() const;
	void setIndex(int index);

signals:
	void changed(int);

private slots:
	void slotStatusBarText(const QString&);
	void slotLoadStarted();
	void slotLoadFinished();
	void slotChanged();

private:
	QString m_statusBarText;
	bool m_loadingFinished;
	int m_index;
};

#endif
