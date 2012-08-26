/**

\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012  Mattia Basaglia

Knotter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Knotter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "help_view.hpp"
#include <QDesktopServices>
#include "resource_loader.hpp"

Help_View::Help_View(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    web_view->load( load::resource_url(DOC_DIR,"user_guide/user_guide.htm") );
    web_view->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
}

void Help_View::on_web_view_linkClicked(const QUrl &arg1)
{
    QDesktopServices::openUrl(arg1);
}
