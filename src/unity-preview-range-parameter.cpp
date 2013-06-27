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

#include <unity/action/PreviewRangeParameter>
using namespace unity::action;

//! \private
class Q_DECL_HIDDEN unity::action::PreviewRangeParameter::Private {
public:
    QString text;
    float value;
    float min;
    float max;
};

PreviewRangeParameter::PreviewRangeParameter(QObject *parent)
    : PreviewParameter(parent),
      d(new Private())
{
    d->max = 100.0f;
    d->min = 0.0f;
    d->value = 0.0f;
}

PreviewRangeParameter::~PreviewRangeParameter()
{
}

QString
PreviewRangeParameter::text() const
{
    return d->text;
}

void
PreviewRangeParameter::setText(const QString &value)
{
    if (d->text == value)
        return;
    d->text = value;
    emit textChanged(value);
}

float
PreviewRangeParameter::value() const
{
    return d->value;
}

void
PreviewRangeParameter::setValue(float value)
{
    if (qFuzzyCompare(d->value, value))
        return;

    if (!qFuzzyCompare(d->min, value) && value < d->min) {
        qWarning("%s: trying to set range value (%f) below minimum (%f)",
                 __PRETTY_FUNCTION__,
                 value,
                 d->min);
        return;
    }

    if (!qFuzzyCompare(d->max, value) && value > d->max) {
        qWarning("%s: trying to set range value (%f) above maximum (%f)",
                 __PRETTY_FUNCTION__,
                 value,
                 d->max);
        return;
    }

    d->value = value;
    emit valueChanged(value);
}

float
PreviewRangeParameter::minimumValue() const
{
    return d->min;
}

void
PreviewRangeParameter::setMinimumValue(float value)
{
    if (qFuzzyCompare(d->min, value))
        return;

    // make sure min is not bigger than max
    if (!qFuzzyCompare(value, d->max) && value > d->max) {
        qWarning("%s: trying to set minimum value (%f) above maximum (%f)",
                 __PRETTY_FUNCTION__,
                 value,
                 d->max);
        d->min = d->max;
    } else {
        d->min = value;
    }
    emit minimumValueChanged(d->min);

    // make sure that current value is not smaller than min
    // if so, update value also
    if (!qFuzzyCompare(d->value, d->min) && d->value < d->min) {
        setValue(d->min);
    }
}

float
PreviewRangeParameter::maximumValue() const
{
    return d->max;
}

void
PreviewRangeParameter::setMaximumValue(float value)
{
    if (qFuzzyCompare(d->max, value))
        return;

    // make sure max is not smaller than min
    if (!qFuzzyCompare(value, d->min) && value < d->min) {
        qWarning("%s: trying to set maximum value (%f) below minimum (%f)",
                 __PRETTY_FUNCTION__,
                 value,
                 d->min);
        d->max = d->min;
    } else {
        d->max = value;
    }
    emit maximumValueChanged(d->max);

    // make sure that current value is not bigger than max
    // if so, update value also
    if (!qFuzzyCompare(d->value, d->max) && d->value > d->max) {
        setValue(d->max);
    }
}
