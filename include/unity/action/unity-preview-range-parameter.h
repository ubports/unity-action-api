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

#ifndef UNITY_ACTION_PREVIEW_RANGE_PARAMETER
#define UNITY_ACTION_PREVIEW_RANGE_PARAMETER

#include <unity/action/PreviewParameter>

namespace unity {
namespace action {
    class PreviewRangeParameter;
}
}

class Q_DECL_EXPORT unity::action::PreviewRangeParameter : public unity::action::PreviewParameter
{
    Q_OBJECT
    Q_DISABLE_COPY(PreviewRangeParameter)
    Q_PROPERTY(QString text
               READ text
               WRITE setText
               NOTIFY textChanged)
    Q_PROPERTY(float value
               READ value
               WRITE setValue
               NOTIFY valueChanged)
    Q_PROPERTY(float minimumValue
               READ minimumValue
               WRITE setMinimumValue
               NOTIFY minimumValueChanged)
    Q_PROPERTY(float maximumValue
               READ maximumValue
               WRITE setMaximumValue
               NOTIFY maximumValueChanged)

public:

    explicit PreviewRangeParameter(QObject *parent = 0);
    virtual ~PreviewRangeParameter();

    /*! user visible label of the parameter */
    QString text() const;
    void setText(const QString &value);

    float value() const;
    void setValue(float value);

    float minimumValue() const;
    void setMinimumValue(float value);

    float maximumValue() const;
    void setMaximumValue(float value);

signals:
    void textChanged(const QString &text);
    void valueChanged(float value);
    void minimumValueChanged(float value);
    void maximumValueChanged(float value);

private:
    class Private;
    QScopedPointer<Private> d;
};

#endif
