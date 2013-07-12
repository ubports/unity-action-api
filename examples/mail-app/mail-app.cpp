/* This file is part of unity-action-api
 * Copyright 2013 Canonical Ltd.
 *
 * unity-action-api is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * unity-action-api is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QMainWindow>
#include <QApplication>

#include <unity/action/Action>
#include <unity/action/ActionManager>

using unity::action::Action;
using unity::action::ActionManager;

class MailApp : public QMainWindow
{
    Q_OBJECT

public:
    MailApp(QWidget *parent = 0);

public slots:
    void newMessage();
    void openAddressBook();

private:
};

MailApp::MailApp(QWidget *parent)
    : QMainWindow(parent)
{

//! [setting the compose name]
    Action *newMessageAction = new Action(this);

    // matches the .desktop file Desktop Action identifier
    newMessageAction->setName("Compose");
//! [setting the compose name]
    newMessageAction->setText(tr("Compose New Message"));
    newMessageAction->setKeywords(tr("Write New Message;Send New Message"));
    connect(newMessageAction, &Action::triggered, this, &MailApp::newMessage);

//! [setting the contacts name]
    Action *openAddressBookAction = new Action(this);

    // matches the .desktop file Desktop Action identifier
    openAddressBookAction->setName("Contacts");
//! [setting the contacts name]
    openAddressBookAction->setText(tr("Open Address Book"));
    connect(openAddressBookAction, &Action::triggered, this, &MailApp::openAddressBook);

//! [add to manager]
    ActionManager *actionManager = new ActionManager(this);
    actionManager->addAction(newMessageAction);
    actionManager->addAction(openAddressBookAction);
//! [add to manager]
}

void
MailApp::newMessage()
{
    // show the compose view
}

void
MailApp::openAddressBook()
{
    // show the address book view
}


int main(int argc, char *argv[])
{
    QApplication *app = new QApplication(argc, argv);
    MailApp mailApp;
    mailApp.show();
    return app->exec();
}

#include "mail-app.moc"
