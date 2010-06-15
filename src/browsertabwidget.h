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

#ifndef __VELASCO_BROWSER_TAB_WIDGET_H__
#define __VELASCO_BROWSER_TAB_WIDGET_H__

#include "config.h"

#include <QtGui/QWidget>

class QTabWidget;
class QCheckBox;
class QLineEdit;
class QToolButton;
class QFrame;
class QMainWindow;
class QUrl;

class BrowserTab;

class BrowserTabWidget : public QWidget
{
	Q_OBJECT

public:
	BrowserTabWidget(QMainWindow *parent);

	QMainWindow *mainWindow() const;
	BrowserTab *currentBrowser() const;
	QList<BrowserTab *> browsers() const;

signals:
	//void currentBrowserUrlChanged(const QUrl&);
	//void browserTitleChanged(const QString&);
	//void browserIconChanged();
	void tabCountChanged(int count);

protected:
	void keyPressEvent(QKeyEvent *event);
	bool eventFilter(QObject *object, QEvent *event);

public slots:
	void load(const QUrl&);
	void load(const QString&);

	void forward();
	void backward();
	void stop();
	void reload();

	void newTab();
	void newTab(const QString&);
	void nextTab();
	void previousTab();
	void closeTab();

	void find();
	void findNext();
	void findPrevious();

	void transferFocus();

private slots:
	void updateTab(int index);
	void find(QString text, bool forward = false, bool backward = false);

private:
	void setupUi();

private:
	QTabWidget *m_tabWidget;

	QFrame *m_frameFind;
	QCheckBox *m_checkCase;
	QLineEdit *m_editFind;
	QToolButton *m_toolClose;
	QToolButton *m_toolUp;
	QToolButton *m_toolDown;
};

#endif
