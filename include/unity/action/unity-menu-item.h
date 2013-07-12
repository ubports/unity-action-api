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

#ifndef UNITY_ACTION_MENUITEM
#define UNITY_ACTION_MENUITEM

namespace unity {
namespace action {
    class MenuItem;
    class Action;
}
}

#include <QObject>
#include <QVariant>
#include <QScopedPointer>

class Q_DECL_PUBLIC unity::action::MenuItem : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(MenuItem)

    Q_PROPERTY(unity::action::Action *action
               READ action
               WRITE setAction
               NOTIFY actionChanged)

    Q_PROPERTY(QString text
               READ text
               WRITE setText
               NOTIFY textChanged)

    Q_PROPERTY(QString iconName
               READ iconName
               WRITE setIconName
               NOTIFY iconNameChanged)

    Q_PROPERTY(QVariant target
               READ target
               WRITE setTarget
               NOTIFY targetChanged)

    Q_PROPERTY(bool visible
               READ visible
               WRITE setVisible
               NOTIFY visibleChanged)

    Q_PROPERTY(bool enabled
               READ enabled
               WRITE setEnabled
               NOTIFY enabledChanged)

public:

    explicit MenuItem(QObject *parent =0);
    virtual ~MenuItem();

    Action *action() const;
    void setAction(Action *value);

    QString text() const;
    void setText(const QString &value);

    QString iconName() const;
    void setIconName(const QString &value);

    QVariant target() const;
    void setTarget(const QVariant &value);

    bool visible() const;
    void setVisible(bool value);

    bool enabled() const;
    void setEnabled(bool value);

signals:
    void actionChanged();
    void textChanged(const QString &value);
    void iconNameChanged(const QString &value);
    void targetChanged(const QVariant &value);
    void visibleChanged(bool value);
    void enabledChanged(bool value);

private:
    class Private;
    QScopedPointer<Private> d;
};

#endif
