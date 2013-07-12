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

#include <QApplication>
#include <QMainWindow>

#include <unity/action/ActionManager>
#include <unity/action/PreviewAction>
#include <unity/action/PreviewRangeParameter>

#include <QDebug>

using namespace unity::action;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
//! [create previewaction]
    PreviewAction *previewAction = new PreviewAction(this);
    previewAction->setText("Exposure");
//! [create previewaction]

//! [create rangeparameter]
    PreviewRangeParameter *range = new PreviewRangeParameter(this);
    range->setText("Exposure Amount");
    range->setMinimumValue(-50);
    range->setMaximumValue(50);
    range->setValue(0);
//! [create rangeparameter]


//! [set previewaction parameters]
    previewAction->addParameter(range);
//! [set previewaction parameters]

//! [add to manager]
    ActionManager *manager = new ActionManager(this);
    manager->addAction(previewAction);
//! [add to manager]

//! [connect previewaction signals]
    connect(previewAction, &PreviewAction::triggered, [=]() {
        // action was completed.
        // apply the action with the values of the parameters
        qDebug() << "Preview triggered.";
    });
    connect(previewAction, &PreviewAction::cancelled, [=]() {
        // action was cancelled
        qDebug() << "Preview cancelled.";
    });
    connect(previewAction, &PreviewAction::started, [=]() {
        // action was activated in the HUD UI.
        // do the necessary initialization and start providing preview
        // based on the values of the parameters.
        qDebug() << "Preview started.";
    });
    connect(previewAction, &PreviewAction::resetted, [=]() {
        // reset the parameter values
        qDebug() << "Preview resetted.";
    });
//! [connect previewaction signals]

//! [connect parameter signals]
    connect(range, &PreviewRangeParameter::valueChanged, [=](float value) {
        // gets the values from the HUD UI when user interacts with the slider
        // Application then updates the preview based on this value
        qDebug() << "range value:" << value;
    });
//! [connect parameter signals]
}

MainWindow::~MainWindow()
{}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

#include "preview-action.moc"
