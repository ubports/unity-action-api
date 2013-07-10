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

#ifndef UNITY_ACTION_ACTION
#define UNITY_ACTION_ACTION

namespace unity {
namespace action {
    class Action;
}
}

#include <QObject>
#include <QVariant>
#include <QScopedPointer>

/*!
 * \class unity::action::Action
 * \brief
 *
 * The most basic type of action is the Action class. Unity services visualizing this class will usually
 * be represented it as a simple button or menu item, depending upon where it is contributed.
 *
 * The optional name property is available through D-Bus and can be used to activate a specific Action
 * from the Launcher (quicklist actions defined in .desktop file) for example. As the name is available
 * on the bus it can also later be used for IPC between applications.
 *
 * All of the more specific action classes discussed below derive from the base Action class,
 * inheriting its basic behaviour.
 *
 * If the parameterType property is set, the Action is said to be parameterised. This means that when it is
 * bound to a menu or button, the action expects a typed input parameter. When invoked by a user,
 * the action will prompt for input. When invoked programmatically, over DBus for example,
 * the parameter will be provided at that point.
 *
The Action defines an ActionTypes enum in C++, which is then exposed to QML:

enum ActionTypes {
    String,
    Integer,
    Boolean,
    Real,
...
}

To define, for example,  an Action with a string parameter:

Action {
    name: “foo”
    parameterType: ActionTypes.String
}

*/

class Q_DECL_EXPORT unity::action::Action : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Action)
    Q_ENUMS(Type)

    Q_PROPERTY(QString name
               READ name
               WRITE setName
               NOTIFY nameChanged)
    Q_PROPERTY(QString text
               READ text
               WRITE setText
               NOTIFY textChanged)
    Q_PROPERTY(QString iconName
               READ iconName
               WRITE setIconName
               NOTIFY iconNameChanged)
    Q_PROPERTY(QString description
               READ description
               WRITE setDescription
               NOTIFY descriptionChanged)
    Q_PROPERTY(QString keywords
               READ keywords
               WRITE setKeywords
               NOTIFY keywordsChanged)
    Q_PROPERTY(bool enabled
               READ enabled
               WRITE setEnabled
               NOTIFY enabledChanged)
    Q_PROPERTY(unity::action::Action::Type parameterType
               READ parameterType
               WRITE setParameterType
               NOTIFY parameterTypeChanged)
public:
        enum Type {
            None,
            String,
            Integer,
            Bool,
            Real
        };

        explicit Action(QObject *parent = 0);
        virtual ~Action();

        QString name() const;

        /*! potentially an expensive operation */
        void setName(const QString &value);

        QString text() const;
        void setText(const QString &value);

        QString iconName() const;
        void setIconName(const QString &value);

        QString description() const;
        void setDescription(const QString &value);

        QString keywords() const;
        void setKeywords(const QString &value);

        bool enabled() const;
        void setEnabled(bool value);

        Type parameterType() const;

        /*! potentially an expensive operation */
        void setParameterType(Type value);

public slots:
        // checks the value agains parameterType
        void trigger(QVariant value = QVariant());

signals:
        void nameChanged(const QString &value);
        void textChanged(const QString &value);
        void iconNameChanged(const QString &value);
        void descriptionChanged(const QString &value);
        void keywordsChanged(const QString &value);
        void enabledChanged(bool value);
        void parameterTypeChanged(unity::action::Action::Type value);


        void triggered(QVariant value);

private:
        class Private;
        QScopedPointer<Private> d;
};
Q_DECLARE_METATYPE(unity::action::Action::Type)
#endif
