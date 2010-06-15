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

#include "config.h"
#include "browserwindow.h"

#include <QtGui>

BrowserWindow::BrowserWindow()
{
	m_dockWidget = new QDockWidget(tr("Dock panel"), this);
	m_dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	m_tabWidget = new BrowserTabWidget(this);
	m_editAddress = new QLineEdit();
	//connect(m_editAddress, 
	
	m_listToolBars = new QList<QToolBar*>();

	addDockWidget(Qt::RightDockWidgetArea, m_dockWidget);
	setCentralWidget(m_tabWidget);
	setWindowIcon(QIcon(":/images/apps/internet-web-browser.png"));

	setupActions();
	setupMenu();
	setupDefaultToolbars();

	statusBar()->showMessage(tr("Ready"));
	loadPreferences();
}

BrowserWindow::~BrowserWindow()
{
	savePreferences();
}

void BrowserWindow::setupActions()
{
	qDebug("DEBUG: Setup actions ...");

	m_actionNewWindow = new QAction(QIcon(":/images/actions/window-new.png"), tr("&New Window"), this);
	m_actionNewWindow->setShortcut(tr("Ctrl+N"));
	m_actionNewWindow->setStatusTip(tr("Open a new browser window"));

	m_actionNewTab = new QAction(QIcon(":/images/actions/tab-new.png"), tr("New &tab"), this);
	m_actionNewTab->setShortcut(tr("Ctrl+T"));
	m_actionNewTab->setStatusTip(tr("Open a new tab in current window"));
	connect(m_actionNewTab, SIGNAL(triggered()), this, SLOT(newTab()));

	m_actionOpenFile = new QAction(QIcon(":/images/actions/document-open.png"), tr("&Open ..."), this);
	m_actionOpenFile->setShortcut(tr("Ctrl+O"));
	m_actionOpenFile->setStatusTip(tr("Open a file"));
	connect(m_actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));

	m_actionSaveAs = new QAction(QIcon(":/images/actions/document-save-as.png"), tr("Save &as ..."), this);
	m_actionSaveAs->setShortcut(tr("Ctrl+S"));
	m_actionSaveAs->setStatusTip(tr("Save currently open document"));
	connect(m_actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));

	m_actionPrint = new QAction(QIcon(":/images/actions/document-print.png"), tr("&Print ..."), this);
	m_actionPrint->setShortcut(tr("Ctrl+P"));
	m_actionPrint->setStatusTip(tr("Print current open document"));
	connect(m_actionPrint, SIGNAL(triggered()), this, SLOT(print()));

	m_actionSendLink = new QAction(QIcon(":/images/actions/mail-forward.png"), tr("Send &link to ..."), this);
	m_actionSendLink->setStatusTip(tr("Send current open document link via mail"));
	connect(m_actionSaveAs, SIGNAL(triggered()), this, SLOT(sendLink()));

	m_actionToggleOfflineMode = new QAction(tr("Work &offline"), this);
	m_actionToggleOfflineMode->setStatusTip(tr("Switch to offline working mode"));
	m_actionToggleOfflineMode->setCheckable(true);
	connect(m_actionToggleOfflineMode, SIGNAL(toggled(bool)), this, SLOT(toggleOfflineMode(bool)));

	m_actionQuit = new QAction(QIcon(":/images/actions/system-log-out.png"), tr("&Quit"), this);
	m_actionQuit->setShortcut(tr("Ctrl+Q"));
	m_actionQuit->setStatusTip(tr("Quit the application"));
	connect(m_actionQuit, SIGNAL(triggered()), this, SLOT(close()));

	m_actionUndo = new QAction(QIcon(":/images/actions/edit-undo.png"), tr("&Undo"), this);
	m_actionUndo->setShortcut(tr("Ctrl+Z"));
	m_actionUndo->setStatusTip(tr("Undo last action"));
	connect(m_actionUndo, SIGNAL(triggered()), this, SLOT(undo()));

	m_actionRedo = new QAction(QIcon(":/images/actions/edit-redo.png"), tr("&Redo"), this);
	m_actionRedo->setShortcut(tr("Ctrl+Shift+Z"));
	m_actionRedo->setStatusTip(tr("Redo last action"));
	connect(m_actionRedo, SIGNAL(triggered()), this, SLOT(redo()));

	m_actionCut = new QAction(QIcon(":/images/actions/edit-cut.png"), tr("Cu&t"), this);
	m_actionCut->setShortcut(tr("Ctrl+X"));
	m_actionCut->setStatusTip(tr("Cut the current selection's contents into cliboard"));
	connect(m_actionCut, SIGNAL(triggered()), this, SLOT(cut()));

	m_actionCopy = new QAction(QIcon(":/images/actions/edit-copy.png"), tr("&Copy"), this);
	m_actionCopy->setShortcut(tr("Ctrl+C"));
	m_actionCopy->setStatusTip(tr("Copy the current selection's contents into clipboard"));
	connect(m_actionCopy, SIGNAL(triggered()), this, SLOT(copy()));

	m_actionPaste = new QAction(QIcon(":/images/actions/edit-paste.png"), tr("&Paste"), this);
	m_actionPaste->setShortcut(tr("Ctrl+V"));
	m_actionPaste->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
	connect(m_actionPaste, SIGNAL(triggered()), this, SLOT(paste()));

	m_actionSelectAll = new QAction(QIcon(":/images/actions/edit-select-all.png"), tr("Select &all"), this);
	m_actionSelectAll->setShortcut(tr("Ctrl+A"));
	m_actionSelectAll->setStatusTip(tr("Select the whole page content"));
	connect(m_actionSelectAll, SIGNAL(triggered()), this, SLOT(selectAll()));

	m_actionFind = new QAction(QIcon(":/images/actions/edit-find.png"), tr("&Find ..."), this);
	m_actionFind->setShortcut(tr("Ctrl+F"));
	m_actionFind->setStatusTip(tr("Search a word or string in the current page"));
	connect(m_actionFind, SIGNAL(triggered()), this, SLOT(find()));

	m_actionFindNext = new QAction(tr("Find &next"), this);
	m_actionFindNext->setShortcut(tr("Ctrl+G"));
	m_actionFindNext->setStatusTip(tr("Search next position on which the string occurs"));
	connect(m_actionFindNext, SIGNAL(triggered()), this, SLOT(findNext()));

	m_actionFindLast = new QAction(tr("Find &last"), this);
	m_actionFindNext->setShortcut(tr("Shift+Ctrl+G"));
	m_actionFindNext->setStatusTip(tr("Search last position on which the string occurs"));
	connect(m_actionFindLast, SIGNAL(triggered()), this, SLOT(findLast()));

	m_actionPreferences = new QAction(QIcon(":/images/categories/preferences-system.png"), tr("Preferences"), this);
	m_actionPreferences->setStatusTip(tr("Configure the web browser"));
	connect(m_actionPreferences, SIGNAL(triggered()), this, SLOT(preferences()));

	m_actionToggleToolBars = new QAction(tr("&Visible"), this);
	m_actionToggleToolBars->setStatusTip(tr("Show or hide toolbars"));
	m_actionToggleToolBars->setCheckable(true);
	m_actionToggleToolBars->setChecked(true);
	// TODO FIXME: use standard slot
	connect(m_actionToggleToolBars, SIGNAL(toggled(bool)), this, SLOT(toggleToolBars(bool)));

	m_actionCustomizeToolBars = new QAction(tr("&Customize ..."), this);
	m_actionCustomizeToolBars->setStatusTip(tr("Customize toolbars"));
	connect(m_actionCustomizeToolBars, SIGNAL(triggered()), this, SLOT(customizeToolBars()));
	
	m_actionToggleStatusBar = new QAction(tr("&Statusbar"), this);
	m_actionToggleStatusBar->setStatusTip(tr("Show or hide statusbar"));
	m_actionToggleStatusBar->setCheckable(true);
	m_actionToggleStatusBar->setChecked(true);
	connect(m_actionToggleStatusBar, SIGNAL(toggled(bool)), statusBar(), SLOT(setVisible(bool)));

	m_actionToggleDock = new QAction(tr("&Dockpanel"), this);
	m_actionToggleDock->setShortcut(tr("F9"));
	m_actionToggleDock->setStatusTip(tr("Show or hide dock panel"));
	m_actionToggleDock->setCheckable(true);
	m_actionToggleDock->setChecked(true);
	connect(m_actionToggleDock, SIGNAL(toggled(bool)), this, SLOT(toggleDock(bool)));
	connect(m_actionToggleDock, SIGNAL(toggled(bool)), m_dockWidget, SLOT(setVisible(bool)));

	m_actionToggleFullScreen = new QAction(QIcon(":/images/actions/view-fullscreen.png"), tr("&Fullscreen"), this);
	m_actionToggleFullScreen->setShortcut(tr("F11"));
	m_actionToggleFullScreen->setStatusTip(tr("Toggle fullscreen view"));
	m_actionToggleFullScreen->setCheckable(true);
	connect(m_actionToggleFullScreen, SIGNAL(toggled(bool)), this, SLOT(toggleFullScreen(bool)));

	m_actionFontSizeBigger = new QAction(tr("Fontsize &bigger"), this);
	m_actionFontSizeBigger->setShortcut(tr("Ctrl++"));
	m_actionFontSizeBigger->setStatusTip(tr("Increase font size"));
	connect(m_actionFontSizeBigger, SIGNAL(triggered()), this, SLOT(fontSizeBigger()));

	m_actionFontSizeSmaller = new QAction(tr("Fontsize &smaller"), this);
	m_actionFontSizeSmaller->setShortcut(tr("Ctrl+-"));
	m_actionFontSizeSmaller->setStatusTip(tr("Decrease font size"));
	connect(m_actionFontSizeSmaller, SIGNAL(triggered()), this, SLOT(fontSizeSmaller()));
	
	m_actionFontSizeNormal = new QAction(tr("Normal &fontsize"), this);
	m_actionFontSizeSmaller->setShortcut(tr("Ctrl+0"));
	m_actionFontSizeSmaller->setStatusTip(tr("Reset normal font size"));
	connect(m_actionFontSizeNormal, SIGNAL(triggered()), this, SLOT(fontSizeNormal()));

	m_actionPageSource = new QAction(tr("&Page source"), this);
	m_actionPageSource->setShortcut(tr("Ctrl+U"));
	m_actionPageSource->setStatusTip(tr("Show source code of the current page"));
	connect(m_actionPageSource, SIGNAL(triggered()), this, SLOT(pageSource()));

	m_actionPageInfo = new QAction(QIcon(":/images/actions/document-properties.png"), tr("Page &info"), this);
	m_actionPageInfo->setShortcut(tr("Ctrl+I"));
	m_actionPageInfo->setStatusTip(tr("Show information about the current page"));
	connect(m_actionPageInfo, SIGNAL(triggered()), this, SLOT(pageInfo()));

	m_actionStop = new QAction(QIcon(":/images/actions/process-stop.png"), tr("&Stop"), this);
	m_actionStop->setShortcut(tr("Esc"));
	m_actionStop->setStatusTip(tr("Stop loading page"));
	connect(m_actionStop, SIGNAL(triggered()), this, SLOT(stop()));

	m_actionReload = new QAction(QIcon(":/images/actions/view-refresh.png"), tr("&Reload"), this);
	m_actionReload->setShortcut(tr("F5"));
	m_actionReload->setStatusTip(tr("Reload current page"));
	connect(m_actionReload, SIGNAL(triggered()), this, SLOT(reload()));

	m_actionToggleAutoReload = new QAction(tr("&Auto reload"), this);
	m_actionToggleAutoReload->setStatusTip(tr("Update the page at regular intervals"));
	m_actionToggleAutoReload->setCheckable(true);
	connect(m_actionToggleAutoReload, SIGNAL(toggled(bool)), this, SLOT(toggleAutoReload(bool)));

	m_actionBack = new QAction(QIcon(":/images/actions/go-previous.png"), tr("&Back"), this);
	m_actionBack->setShortcut(tr("Alt+Left"));
	m_actionBack->setStatusTip(tr("Go back to previous page"));
	connect(m_actionBack, SIGNAL(triggered()), this, SLOT(back()));

	m_actionForward = new QAction(QIcon(":/images/actions/go-next.png"), tr("&Forward"), this);
	m_actionForward->setShortcut(tr("Alt+Right"));
	m_actionForward->setStatusTip(tr("Go to following page"));
	connect(m_actionForward, SIGNAL(triggered()), this, SLOT(forward()));

	m_actionHomepage = new QAction(QIcon(":/images/actions/go-home.png"), tr("&Home page"), this);
	m_actionHomepage->setShortcut(tr("Alt+Home"));
	m_actionHomepage->setStatusTip(tr("Go the home page"));
	connect(m_actionHomepage, SIGNAL(triggered()), this, SLOT(homePage()));

	m_actionGoto = new QAction(tr("Goto ..."), this);
	// TODO FIXME: implement url search
	//connect(m_actionGoto, SIGNAL(triggered()), this, SLOT(G

	m_actionHelpContents = new QAction(QIcon(":/images/apps/help-browser.png"), tr("&Contents"), this);
	m_actionHelpContents->setShortcut(tr("F1"));
	m_actionHelpContents->setStatusTip(tr("Show help for %1").arg(APP_NAME));
	// TODO FIXME: show help contents

	m_actionHelpInfo = new QAction(QIcon(":/images/status/dialog-information.png"),tr("&Info"), this);
	m_actionHelpInfo->setStatusTip(tr("Show info about %1").arg(APP_NAME));
	// TODO FIXME: show info
	
}

void BrowserWindow::setupMenu()
{
	qDebug("DEBUG: Setup menu ...");

	m_menuFile = menuBar()->addMenu(tr("&File"));
	m_menuFile->addAction(m_actionNewWindow);
	m_menuFile->addAction(m_actionNewTab);
	m_menuFile->addAction(m_actionOpenFile);
	m_menuFile->addSeparator();
	m_menuFile->addAction(m_actionSaveAs);
	m_menuFile->addSeparator();
	m_menuFile->addAction(m_actionPrint);
	m_menuFile->addAction(m_actionSendLink);
	m_menuFile->addSeparator();
	m_menuFile->addAction(m_actionToggleOfflineMode);
	m_menuFile->addAction(m_actionQuit);

	m_menuEdit = menuBar()->addMenu(tr("&Edit"));
	m_menuEdit->addAction(m_actionUndo);
	m_menuEdit->addAction(m_actionRedo);
	m_menuEdit->addSeparator();
	m_menuEdit->addAction(m_actionCut);
	m_menuEdit->addAction(m_actionCopy);
	m_menuEdit->addAction(m_actionPaste);
	m_menuEdit->addSeparator();
	m_menuEdit->addAction(m_actionSelectAll);
	m_menuEdit->addSeparator();
	m_menuEdit->addAction(m_actionFind);
	m_menuEdit->addAction(m_actionFindNext);
	m_menuEdit->addAction(m_actionFindLast);
	m_menuEdit->addSeparator();
	m_menuEdit->addAction(m_actionPreferences);

	m_menuView = menuBar()->addMenu(tr("&View"));
	QMenu *menuToolBars = new QMenu(tr("Toolbars"));
	menuToolBars->addAction(m_actionToggleToolBars);
	menuToolBars->addAction(m_actionCustomizeToolBars);
	m_menuView->addMenu(menuToolBars);
	m_menuView->addAction(m_actionToggleStatusBar);
	m_menuView->addAction(m_actionToggleDock);
	m_menuView->addSeparator();
	m_menuView->addAction(m_actionToggleFullScreen);
	m_menuView->addSeparator();
	m_menuView->addAction(m_actionFontSizeBigger);
	m_menuView->addAction(m_actionFontSizeSmaller);
	m_menuView->addAction(m_actionFontSizeNormal);
	m_menuView->addSeparator();
	m_menuView->addAction(m_actionPageSource);
	m_menuView->addAction(m_actionPageInfo);
	m_menuView->addSeparator();
	m_menuView->addAction(m_actionStop);
	m_menuView->addAction(m_actionReload);
	m_menuView->addAction(m_actionToggleAutoReload);

	m_menuHistory = menuBar()->addMenu(tr("&History"));
	m_menuHistory->addAction(m_actionBack);
	m_menuHistory->addAction(m_actionForward);
	m_menuHistory->addAction(m_actionHomepage);
	m_menuHistory->addSeparator();
	m_menuHistory->addAction(m_actionGoto);

	m_menuBookmarks = menuBar()->addMenu(tr("&Bookmarks"));
	// TODO: implement later

	m_menuTools = menuBar()->addMenu(tr("&Tools"));
	// TODO: implement later

	menuBar()->addSeparator();

	m_menuHelp = menuBar()->addMenu(tr("&Help"));
	m_menuHelp->addAction(m_actionHelpContents);
	m_menuHelp->addSeparator();
	m_menuHelp->addAction(m_actionHelpInfo);
}

void BrowserWindow::setupDefaultToolbars()
{
	qDebug("DEBUG: Setup default toolbars ...");

	QToolBar *navToolBar = addToolBar(tr("Navigation"));
	
	navToolBar->addAction(m_actionBack);
	navToolBar->addAction(m_actionForward);
	navToolBar->addAction(m_actionStop);
	navToolBar->addAction(m_actionReload);
	navToolBar->addSeparator();
	navToolBar->addWidget(m_editAddress);
	navToolBar->addSeparator();

	m_listToolBars->append(navToolBar);
}

void BrowserWindow::loadPreferences()
{
	qDebug("DEBUG: Load preferences ...");

	QSettings prefs;
	QPoint pos = prefs.value("browser/pos", QPoint(200, 200)).toPoint();
	QSize size = prefs.value("browser/size", QSize(800, 600)).toSize();

	resize(size);
	move(pos);
}

void BrowserWindow::savePreferences()
{
	qDebug("DEBUG: Save preferences ...");

	QSettings prefs;
	prefs.setValue("browser/pos", pos());
	prefs.setValue("browser/size", size());
}

void loadAddress(const QString& address)
{
	QUrl url(address);
	if (url.scheme().isEmpty())
		url = QUrl("http://" + address);

	// TODO FIXME: implement
}

void BrowserWindow::newTab()
{
	/*BrowserTab *tab = new BrowserTab(m_tabWidget);

	m_editAddress->setFocus();

	m_tabWidget->addTab(tab, tr("(Untitled)"));
	m_tabWidget->setCurrentWidget(tab);
	*/
}

void BrowserWindow::closeTab(int index)
{
	/*QWidget *widget = m_tabWidget->widget(index);

	//if (qobject_cast<BrowserTab*>(widget)) {

	//}

	m_tabWidget->removeTab(index);

	if (m_tabWidget->count() == 0) {
		close();
		newTab();
		homePage();
	}*/
}

void BrowserWindow::openFile()
{

}

void BrowserWindow::saveAs()
{

}

void BrowserWindow::print()
{

}

void BrowserWindow::sendLink()
{

}

void BrowserWindow::toggleOfflineMode(bool enable)
{

}

void BrowserWindow::undo()
{

}

void BrowserWindow::redo()
{

}

void BrowserWindow::cut()
{

}

void BrowserWindow::copy()
{

}

void BrowserWindow::paste()
{

}

void BrowserWindow::selectAll()
{

}

void BrowserWindow::find()
{

}

void BrowserWindow::findNext()
{

}

void BrowserWindow::findLast()
{

}

void BrowserWindow::preferences()
{

}

void BrowserWindow::toggleToolBars(bool visible)
{
	for (int i = 0; i < m_listToolBars->size(); ++i) {
		m_listToolBars->at(i)->setVisible(visible);
	}
}

void BrowserWindow::customizeToolBars()
{

}

void BrowserWindow::toggleDock(bool visible)
{

}

void BrowserWindow::toggleFullScreen(bool enable)
{

}

void BrowserWindow::fontSizeNormal()
{

}

void BrowserWindow::fontSizeBigger()
{

}

void BrowserWindow::fontSizeSmaller()
{

}

void BrowserWindow::toggleTextDirection()
{

}

void BrowserWindow::pageSource()
{

}

void BrowserWindow::pageInfo()
{

}

void BrowserWindow::stop()
{

}

void BrowserWindow::reload()
{

}

void BrowserWindow::toggleAutoReload(bool enable)
{

}

void BrowserWindow::back()
{

}

void BrowserWindow::forward()
{

}

void BrowserWindow::homePage()
{

}

void BrowserWindow::helpContents()
{

}

void BrowserWindow::helpAbout()
{

}

void BrowserWindow::updateWindowTitle(const QString& title)
{
	if (title.isEmpty()) {
		setWindowTitle(tr(APP_NAME));
	} else {
		setWindowTitle(tr(APP_NAME " - %1", "Browser name and page title").arg(title));
	}
}

void BrowserWindow::updateStatusBar(const QString& text)
{
	statusBar()->showMessage(text);
}
