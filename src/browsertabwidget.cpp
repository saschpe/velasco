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

#include "browsertabwidget.h"
#include "browsertab.h"

#include <QtGui>

BrowserTabWidget::BrowserTabWidget(QMainWindow *parent)
	: QWidget(parent)
{
	qDebug("DEBUG: Load browser tab widget ...");

	setupUi();
	newTab();
	newTab("heise.de");
	newTab("wikipedia.de");
	newTab();
	newTab();
	newTab("http://google.de");
	newTab();
	newTab();
	newTab();
}

void BrowserTabWidget::setupUi()
{
	m_frameFind = new QFrame(this);
	m_frameFind->setVisible(false);

	m_checkCase = new QCheckBox(tr("Case sensitive"), m_frameFind);

	m_editFind = new QLineEdit(m_frameFind);
	connect(m_editFind, SIGNAL(returnPressed()), this, SLOT(findNext()));
	connect(m_editFind, SIGNAL(textEdited(const QString&)), this, SLOT(find(QString)));

	m_toolClose = new QToolButton(m_frameFind);
	m_toolClose->setShortcut(tr("Esc"));
	m_toolClose->setIcon(QIcon(":/images/actions/process-stop.png"));
	m_toolClose->setAutoRaise(true);
	connect(m_toolClose, SIGNAL(clicked()), m_frameFind, SLOT(hide()));

	m_toolUp = new QToolButton(m_frameFind);
	m_toolUp->setIcon(QIcon(":/images/actions/go-up.png"));
	m_toolUp->setText(tr("Up"));
	m_toolUp->setAutoRaise(true);
	m_toolUp->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(m_toolUp, SIGNAL(clicked()), this, SLOT(findPrevious()));

	m_toolDown = new QToolButton(m_frameFind);
	m_toolDown->setIcon(QIcon(":/images/actions/go-down.png"));
	m_toolDown->setText(tr("Down"));
	m_toolDown->setAutoRaise(true);
	m_toolDown->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(m_toolDown, SIGNAL(clicked()), this, SLOT(findNext()));

	QHBoxLayout *layoutFind = new QHBoxLayout();
	layoutFind->setContentsMargins(0, 0, 0, 0);
	layoutFind->addWidget(m_toolClose);
	layoutFind->addWidget(new QLabel(tr("Search:")));
	layoutFind->addWidget(m_editFind);
	layoutFind->addWidget(m_toolUp);
	layoutFind->addWidget(m_toolDown);
	layoutFind->addWidget(m_checkCase);
	m_frameFind->setLayout(layoutFind);

	m_tabWidget = new QTabWidget(this);
	connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(transferFocus()));
	m_tabWidget->setElideMode(Qt::ElideRight);

	QToolButton *newTabButton = new QToolButton();
	m_tabWidget->setCornerWidget(newTabButton, Qt::TopLeftCorner);
	newTabButton->setCursor(Qt::ArrowCursor);
	newTabButton->setAutoRaise(true);
	newTabButton->setIcon(QIcon(":/images/actions/list-add.png"));
	newTabButton->setToolTip(tr("Add tab"));
	connect(newTabButton, SIGNAL(clicked()), this, SLOT(newTab()));

	QToolButton *closeTabButton = new QToolButton();
	m_tabWidget->setCornerWidget(closeTabButton, Qt::TopRightCorner);
	closeTabButton->setCursor(Qt::ArrowCursor);
	closeTabButton->setAutoRaise(true);
	closeTabButton->setIcon(QIcon(":/images/actions/list-remove.png"));
	closeTabButton->setToolTip(tr("Close tab"));
	closeTabButton->setEnabled(false);
	connect(closeTabButton, SIGNAL(clicked()), this, SLOT(closeTab()));

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(m_tabWidget);
	layout->addWidget(m_frameFind);
	layout->setContentsMargins(2, 2, 2, 2);
	setLayout(layout);
}

QMainWindow *BrowserTabWidget::mainWindow() const
{
	return static_cast<QMainWindow *>(parentWidget());
}

BrowserTab *BrowserTabWidget::currentBrowser() const
{
	return static_cast<BrowserTab *>(m_tabWidget->currentWidget());
}

QList<BrowserTab *> BrowserTabWidget::browsers() const
{
	QList<BrowserTab *> list;
	for (int i = 0; i < m_tabWidget->count(); ++i) {
		Q_ASSERT(qobject_cast<BrowserTab *>(m_tabWidget->widget(i)));
		list.append(static_cast<BrowserTab *>(m_tabWidget->widget(i)));
	}
	return list;
}

void BrowserTabWidget::transferFocus()
{
	if (currentBrowser()) {
		currentBrowser()->setFocus();
	}
	mainWindow()->setWindowTitle(tr(APP_NAME " - %1").arg(currentBrowser()->title()));
}

void BrowserTabWidget::keyPressEvent(QKeyEvent *event)
{
	QString textToFind = m_editFind->text();
	QString text = event->text();

	if (m_frameFind->isVisible()) {
		switch (event->key()) {
			case Qt::Key_Escape:
				m_frameFind->hide();
				return;
			case Qt::Key_Backspace:
				textToFind.chop(1);
				break;
			case Qt::Key_Return:
			case Qt::Key_Enter:
				// Return / Enter key events are not accepted by QLineEdit
				return;
			default:
				if (text.isEmpty()) {
					QWidget::keyPressEvent(event);
					return;
				}
				textToFind += text;
		}
	} else {
		if (text.isEmpty() || text[0].isSpace() || !text[0].isPrint()) {
			QWidget::keyPressEvent(event);
			return;
		}
		if (text.startsWith('/')) {
			m_editFind->clear();
			find();
			return;
		}
		textToFind = text;
		m_frameFind->show();
	}

	m_editFind->setText(textToFind);
	find(textToFind, false, false);
}

bool BrowserTabWidget::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QEvent::KeyPress && m_frameFind->isVisible()) {
		QKeyEvent *ke = static_cast<QKeyEvent *>(event);
		if (ke->key() == Qt::Key_Space) {
			keyPressEvent(ke);
			return true;
		}
	}
	
	return QWidget::eventFilter(object, event);
}

void BrowserTabWidget::load(const QString& address)
{
	if (!address.isEmpty()) {
		QUrl url(address);
		if (url.scheme().isEmpty())
			url = QUrl("http://" + address);
		load(url);
	}
}

void BrowserTabWidget::load(const QUrl& url)
{
	currentBrowser()->load(url);
	//emit browserUrlChanged(currentBrowser()->url().toString());
}

void BrowserTabWidget::forward()
{
	currentBrowser()->forward();
	//emit browserUrlChanged(currentBrowser()->url().toString());
}

void BrowserTabWidget::backward()
{
	currentBrowser()->backward();
	//emit browserUrlChanged(currentBrowser()->url().toString());
}

void BrowserTabWidget::stop()
{
	currentBrowser()->stop();
}

void BrowserTabWidget::reload()
{
	currentBrowser()->reload();
	//emit browserUrlChanged(currentBrowser()->url().toString());
}

void BrowserTabWidget::newTab()
{
	newTab(QString());
}

void BrowserTabWidget::newTab(const QString& address)
{
	BrowserTab *browser = new BrowserTab();
	int tabIndex = -1;

	tabIndex = m_tabWidget->addTab(browser, tr("(Untitled)"));
	browser->setIndex(tabIndex);
	m_tabWidget->setCurrentIndex(tabIndex);
	m_tabWidget->cornerWidget(Qt::TopRightCorner)->setEnabled(m_tabWidget->count() > 1);

	connect(browser, SIGNAL(changed(int)), this, SLOT(updateTab(int)));
	connect(browser, SIGNAL(iconLoaded()), this, SLOT(updateTab(int)));

	emit tabCountChanged(m_tabWidget->count());
}

void BrowserTabWidget::updateTab(int index)
{
	BrowserTab *browser = static_cast<BrowserTab *>(m_tabWidget->widget(index));
	m_tabWidget->setTabText(index, browser->title());
	m_tabWidget->setTabIcon(index, browser->icon());
}

void BrowserTabWidget::nextTab()
{
	if (m_tabWidget->currentIndex() <= m_tabWidget->count() - 1)
		m_tabWidget->setCurrentIndex(m_tabWidget->currentIndex() + 1);
}

void BrowserTabWidget::previousTab()
{
	if (m_tabWidget->currentIndex() > 0)
		m_tabWidget->setCurrentIndex(m_tabWidget->currentIndex() - 1);
}

void BrowserTabWidget::closeTab()
{
	if (m_tabWidget->count() == 1)
		return;

	BrowserTab *browser = currentBrowser();
	m_tabWidget->removeTab(m_tabWidget->indexOf(browser));
	m_tabWidget->cornerWidget(Qt::TopRightCorner)->setEnabled(m_tabWidget->count() > 1);

	emit tabCountChanged(m_tabWidget->count());
}

void BrowserTabWidget::find()
{
	m_frameFind->show();
	m_editFind->setFocus(Qt::ShortcutFocusReason);
	m_editFind->selectAll();
}

void BrowserTabWidget::findNext()
{
	find(m_editFind->text(), true, false);
}

void BrowserTabWidget::findPrevious()
{
	find(m_editFind->text(), false, true);
}

void BrowserTabWidget::find(QString text, bool forward, bool backward)
{
	BrowserTab *browser = currentBrowser();
	//QWebPage *page = browser.page();

	// TODO: implement
}
