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

#include <unity/action/Action>
#include <QMutex>

#include <QDebug>

using namespace unity::action;

namespace unity {
namespace action {
/*!
 * \class Action
 * \brief The main action class.
 *
 * Unity services visualizing this class will usually
 * be represented it as a simple button or menu item, depending upon where it is contributed.
 *
 * The optional name property is available through D-Bus and can be used to activate a specific Action
 * from external componenets such as the Launcher. See \ref page_platform-integartion and
 * \ref page_offline-actions for more information.
 *
 * If the parameterType property is set, the Action is said to be parameterised. This means that when it is
 * bound to a menu or button, the action expects a typed input parameter. The type affects the allowed value
 * of the QVariant that must be passed to the trigger() and triggered().
 *
 * \note As QVariant does automatic conversions between different normally uncorvertable types the developer
 *       must be careful of the side effects the conversions might have if accidentally passing wrong type
 *       of a parameter to trigger() or when handling the value of triggered(). Please, see the QVariant
 *       documentation for more information on the conversions QVariant does.
 *
 * Action has to be added to the ActionManager or a ActionContext to make it available for external components.
 */

/*!
 * \enum Action::Type
 * \brief Available parameter types.
 *
 * This enum defines the available types that the action trigger() and triggered()
 * support.
 *
 *
 * \var Action::Type Action::None
 *
 * No paramater. Trying to pass anything else than an empty
 * QVariant is an error.
 *
 *
 * \var Action::Type Action::String
 *
 * String parameter.
 *
 *
 * \var Action::Type Action::Integer
 *
 * Integer parameter.
 *
 *\var Action::Type Action::Bool
 *
 * Boolean parameter.
 *
 *
 *\var Action::Type Action::Real
 *
 * Single precision floating point parameter.
 */


// property documentation

/*!
 * \property QString Action::description
 *
 * User visible secondary description for the action.
 *
 * Description is more verbose than the text() and should describe the Action with
 * couple of words
 *
 * \initvalue ""
 *
 * \accessors description(), setDescription()
 *
 * \notify descriptionChanged()
 */

/*!
 * \property bool Action::enabled
 *
 * If set to false the action can not be triggered.
 *
 * Components visualizing the action migth either hide the
 * action or make it insensitive.
 *
 * If set to false the Action is removed from the search
 * results of the HUD.
 *
 * \initvalue true
 *
 * \accessors enabled(), setEnabled()
 *
 * \notify enabledChanged()
 */

/*!
 * \property QString Action::iconName
 *
 * Name of a icon for this action.
 *
 * When the action is exported to external components the iconName
 * must be avaible on system icon theme engine.
 *
 * \initvalue ""
 *
 * \accessors iconName(), setIconName()
 *
 * \notify iconNameChanged()
 */

/*!
 * \property QString Action::keywords
 *
 * Additional user visible keywords for the action.
 *
 * Keywords improve the HUD search results when the user tries to search for an action
 * with a synonym of the text(). For example if we the application has an action "Crop"
 * but the user tries to "Trim" then without the keywords the HUD would not try to offer
 * the "Crop" action.
 *
 * The format of the keywords string is "Keyword 1;Keyword 2;Keyword 3" to allow translators
 * to define different number of keywords per language.
 *
 * The keywords are separated by ; and they may contain spaces.
 *
 * \code
 *     Action *action = new Action(this);
 *     action->setText(tr("Crop"));
 *     action->description(tr("Crop the image"));
 *     action->setKeywords(tr("Trim;Cut"));
 * \endcode
 *
 * \initvalue ""
 *
 * \accessors keywords(), setKeywords()
 *
 * \notify keywordsChanged()
 */

/*!
 * \property QString Action::name
 *
 * The name of the action. By default an action gets it's name
 * generated automatically if not overridden with setName() later.
 * If name is set to "" then the action restores it's autogenerated name
 *
 * The actions is accessible from D-Bus with this name.
 *
 * The name is not user visible.
 *
 *
 *
 * \note potentially an expensive operation
 *
 * \initvalue autogenerated
 *
 * \accessors name(), setName()
 *
 * \notify nameChanged()
 */

/*!
 * \property Action::Type Action::parameterType()
 *
 *
 * \note this is potentially an expensive operation if the action is already added to the manager.
 *
 * \initvalue Action::None
 *
 * \accessors parameterType(), setParameterType()
 *
 * \notify parameterTypeChanged()
 */

/*!
 * \property QString Action::text
 *
 * The user visible primary label of the action.
 *
 * \initvalue ""
 *
 * \accessors text(), setText()
 *
 * \notify textChanged()
 */


// signal documentation

/*!
 * \fn void Action::triggered(QVariant)
 *
 * \param value the value which which was passed to trigger()
 *
 * The value is always compatible with the set parameterType. For example
 * If parameterType is Action::String the value can be converted to QString
 * by using the QVariant conversion functions:
 * \code
 *     QString param = value.toString();
 * \endcode
 */
}
}


//! \private
class Q_DECL_HIDDEN unity::action::Action::Private {
public:
    QString name;
    QString generatedname;
    QString text;
    QString iconName;
    QString description;
    QString keywords;
    bool enabled;
    Action::Type parameterType;

    const char *paramTypeName(Action::Type type) {
        switch (type) {
        case Action::None:    return "None";
        case Action::String:  return "String";
        case Action::Integer: return "Integer";
        case Action::Bool:    return "Bool";
        case Action::Real:    return "Real";
        }
        // should not be reached
        Q_ASSERT(0);
        return "Internal Error";
    }
};

/*!
 * \fn Action::Action(QObject *parent = 0)
 * \param parent parent QObject or 0
 *
 * Constructs a new Action. See the property documentation for
 * default values.
 */
Action::Action(QObject *parent)
    : QObject(parent),
      d(new Private())
{
    qRegisterMetaType<unity::action::Action::Type>();
    d->enabled = true;
    d->parameterType = None;

    // autogenerate a unique name
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    static int id = 0;
    d->name = QString("unity-action-%1").arg(QString::number(id++));
    d->generatedname = d->name;
}

Action::~Action()
{
}

QString
Action::name() const
{
    return d->name;
}

void
Action::setName(const QString &value)
{
    QString oldName = d->name;

    if (d->name == value)
        return;
    if (!value.isNull() && !value.isEmpty())
        d->name = value;
    else
        d->name = d->generatedname;

    if (oldName != d->name)
        emit nameChanged(d->name);
}

QString
Action::text() const
{
    return d->text;
}

void
Action::setText(const QString &value)
{
    if (d->text == value)
        return;
    d->text = value;
    emit textChanged(value);
}

QString
Action::iconName() const
{
    return d->iconName;
}

void
Action::setIconName(const QString &value)
{
    if (d->iconName == value)
        return;
    d->iconName = value;
    emit iconNameChanged(value);
}

QString
Action::description() const
{
    return d->description;
}

void
Action::setDescription(const QString &value)
{
    if (d->description == value)
        return;
    d->description = value;
    emit descriptionChanged(value);
}

QString
Action::keywords() const
{
    return d->keywords;
}

void
Action::setKeywords(const QString &value)
{
    if (d->keywords == value)
        return;
    d->keywords = value;
    emit keywordsChanged(value);
}

bool
Action::enabled() const
{
    return d->enabled;
}

void
Action::setEnabled(bool value)
{
    if (d->enabled == value)
        return;
    d->enabled = value;
    emit enabledChanged(value);
}

Action::Type
Action::parameterType() const
{
    return d->parameterType;
}

void
Action::setParameterType(Type value)
{
    if (d->parameterType == value)
        return;
    d->parameterType = value;
    emit parameterTypeChanged(value);
}

void
Action::trigger(QVariant value)
{
    QMetaType::Type targetType = QMetaType::UnknownType;

    switch (d->parameterType) {
    case None: {
        break;
    }
    case String: {
        targetType = QMetaType::QString;
        break;
    }
    case Integer: {
        targetType = QMetaType::Int;
        break;
    }
    case Bool: {
        targetType = QMetaType::Bool;
        break;
    }
    case Real: {
        targetType = QMetaType::Float;
        break;
    }
    }

    // need to take a copy of the value as we have to try to convert() it.
    QVariant tmp = value;
    if ((targetType == QMetaType::UnknownType && d->parameterType != None) ||
        (!tmp.canConvert(targetType) || !tmp.convert(targetType))) {
        qWarning() << __PRETTY_FUNCTION__ << ":\n"
                   << "\tTrying to trigger action (name: " << d->name << " :: text: " << d->text << ")\n"
                   << "\twhich has parameter type '" << d->paramTypeName(d->parameterType) << "'\n"
                   << "\twith incompatible parameter value (" << value << ")";
        return;
    }

    emit triggered(value);
}
