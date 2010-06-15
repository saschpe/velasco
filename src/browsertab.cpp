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

#include "browsertab.h"
#include "browsertabwidget.h"

BrowserTab::BrowserTab(BrowserTabWidget *parent)
	: QWebView(parent)
{
	m_loadingFinished = false;
	m_index = -1;

	connect(page(), SIGNAL(statusBarTextChanged(const QString&)),
			SLOT(slotStatusBarText(const QString&)));
	connect(this, SIGNAL(loadStarted()), SLOT(slotLoadStarted()));
	connect(this, SIGNAL(loadFinished()), SLOT(slotLoadFinished()));

	connect(this, SIGNAL(titleChanged(const QString&)), SLOT(slotChanged()));
	connect(this, SIGNAL(urlChanged(const QUrl&)), SLOT(slotChanged()));
	connect(this, SIGNAL(statusBarTextChanged(const QString&)), SLOT(slotChanged()));
}

void BrowserTab::setIndex(int index)
{
	m_index = index;
}

void BrowserTab::slotChanged()
{
	if (index >= 0) {
		emit changed(m_index);
	}
}

QString BrowserTab::getStatusBarText() const
{
	return m_statusBarText;
}

bool BrowserTab::isLoadingFinished() const
{
	return m_loadingFinished;
}

void BrowserTab::slotStatusBarText(const QString& text)
{
	m_statusBarText = text;
}

void BrowserTab::slotLoadStarted()
{
	m_loadingFinished = false;
}

void BrowserTab::slotLoadFinished()
{
	m_loadingFinished = true;
}
