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

#ifndef __VELASCO_BROWSER_WINDOW_H__
#define __VELASCO_BROWSER_WINDOW_H__

#include "config.h"
#include "browsertabwidget.h"

#include <QtGui/QMainWindow>

class QLineEdit;
class QTabWidget;

/**
 * @short The MainWindow of the Browser Application.
 *
 * This is keeping track of the menu, window title, creating and
 * closing tabs.
 */
class BrowserWindow : public QMainWindow
{
	Q_OBJECT

public:
	BrowserWindow();
	~BrowserWindow();

	void loadAddress(const QString& address);

private slots:
	void newTab();
	void closeTab(int);
	void openFile();
	void saveAs();
	void print();
	void sendLink();
	void toggleOfflineMode(bool);

	void undo();
	void redo();
	void cut();
	void copy();
	void paste();
	void selectAll();
	void find();
	void findNext();
	void findLast();
	void preferences();

	void toggleToolBars(bool);
	void customizeToolBars();
	void toggleDock(bool);
	void toggleFullScreen(bool);
	void fontSizeNormal();
	void fontSizeBigger();
	void fontSizeSmaller();
	void toggleTextDirection();

	void pageSource();
	void pageInfo();
	void stop();
	void reload();
	void toggleAutoReload(bool);

	void back();
	void forward();
	void homePage();

	void helpContents();
	void helpAbout();

	void updateWindowTitle(const QString&);
	void updateStatusBar(const QString&);
	//void browserTabTitleChanged(const QString&);
	//void browserTabIconChanged();

private:
	void setupActions();
	void setupMenu();
	void setupDefaultToolbars();

	void savePreferences();
	void loadPreferences();

private:
	BrowserTabWidget *m_tabWidget;
	QDockWidget *m_dockWidget;
	QLineEdit *m_editAddress;
	QList<QToolBar*> *m_listToolBars;

	QMenu *m_menuFile;
	QMenu *m_menuEdit;
	QMenu *m_menuView;
	QMenu *m_menuHistory;
	QMenu *m_menuBookmarks;
	QMenu *m_menuTools;
	QMenu *m_menuHelp;

	QAction *m_actionNewWindow;
	QAction *m_actionNewTab;
	QAction *m_actionOpenFile;
	QAction *m_actionSaveAs;
	QAction *m_actionPrint;
	QAction *m_actionSendLink;
	QAction *m_actionToggleOfflineMode;
	QAction *m_actionQuit;

	QAction *m_actionUndo;
	QAction *m_actionRedo;
	QAction *m_actionCut;
	QAction *m_actionCopy;
	QAction *m_actionPaste;
	QAction *m_actionSelectAll;
	QAction *m_actionFind;
	QAction *m_actionFindNext;
	QAction *m_actionFindLast;
	QAction *m_actionPreferences;

	QAction *m_actionToggleToolBars;
	QAction *m_actionCustomizeToolBars;
	QAction *m_actionToggleStatusBar;
	QAction *m_actionToggleDock;
	QAction *m_actionToggleFullScreen;
	QAction *m_actionFontSizeNormal;
	QAction *m_actionFontSizeBigger;
	QAction *m_actionFontSizeSmaller;
	QAction *m_actionToggleTextDirection;
	QAction *m_actionPageSource;
	QAction *m_actionPageInfo;
	QAction *m_actionStop;
	QAction *m_actionReload;
	QAction *m_actionToggleAutoReload;

	QAction *m_actionBack;
	QAction *m_actionForward;
	QAction *m_actionHomepage;
	QAction *m_actionGoto;						// uses LineEdit

	QAction *m_actionHelpContents;
	QAction *m_actionHelpInfo;
};

#endif
