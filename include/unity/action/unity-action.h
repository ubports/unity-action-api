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

class Q_DECL_EXPORT unity::action::Action : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Action)
    Q_ENUMS(unity::action::Action::Type)
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
