/*
  Copyright (c) 2010 Bertjan Broeksema <broeksema@kde.org>
  Copyright (c) 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
*/

#ifndef INCIDENCEEDITOR_INCIDENCEDATETIME_H
#define INCIDENCEEDITOR_INCIDENCEDATETIME_H

#include "incidenceeditor-ng.h"

#include <KCalCore/Event>
#include <KCalCore/Journal>
#include <KCalCore/Todo>

#include <QDate>

namespace Ui {
class EventOrTodoDesktop;
}

namespace IncidenceEditorNG {
class IncidenceDateTime : public IncidenceEditor
{
    Q_OBJECT
public:
    using IncidenceEditorNG::IncidenceEditor::save; // So we don't trigger -Woverloaded-virtual
    using IncidenceEditorNG::IncidenceEditor::load; // So we don't trigger -Woverloaded-virtual
    explicit IncidenceDateTime(Ui::EventOrTodoDesktop *ui);
    ~IncidenceDateTime();

    void load(const KCalCore::Incidence::Ptr &incidence) override;
    void save(const KCalCore::Incidence::Ptr &incidence) override;
    bool isDirty() const override;

    /**
     * Sets the active date for the editing session. This defaults to the current
     * date. It should be set <em>before</em> loading a non-empty (i.e. existing
     * incidence).
     */
    void setActiveDate(const QDate &activeDate);

    QDate startDate() const; /// Returns the current start date.
    QTime startTime() const; /// Returns the current start time.
    QDate endDate() const; /// Returns the current end date.
    QTime endTime() const; /// Returns the current endtime.

    /// Created from the values in the widgets
    QDateTime currentStartDateTime() const;
    QDateTime currentEndDateTime() const;

    void setStartTime(const QTime &newTime);
    void setStartDate(const QDate &newDate);

    bool startDateTimeEnabled() const;
    bool endDateTimeEnabled() const;

    bool isValid() const override;
    void printDebugInfo() const override;

Q_SIGNALS:
    // used to indicate that the widgets were activated
    void startDateFocus(QObject *obj);
    void endDateFocus(QObject *obj);
    void startTimeFocus(QObject *obj);
    void endTimeFocus(QObject *obj);

    // general
    void startDateTimeToggled(bool enabled);
    void startDateChanged(const QDate &newDate);
    void startTimeChanged(const QTime &newTime);
    void endDateTimeToggled(bool enabled);
    void endDateChanged(const QDate &newDate);
    void endTimeChanged(const QTime &newTime);

private Q_SLOTS: /// General
    void setTimeZonesVisibility(bool visible);
    void toggleTimeZoneVisibility();
    void updateStartTime(const QTime &newTime);
    void updateStartDate(const QDate &newDate);
    void updateStartSpec();
    void updateStartToolTips();
    void updateEndToolTips();

private Q_SLOTS: /// Todo specific
    void enableStartEdit(bool enable);
    void enableEndEdit(bool enable);
    void enableTimeEdits();
    bool isDirty(const KCalCore::Todo::Ptr &todo) const;

private Q_SLOTS: /// Event specific
    bool isDirty(const KCalCore::Event::Ptr &event) const;
    bool isDirty(const KCalCore::Journal::Ptr &journal) const;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void load(const KCalCore::Event::Ptr &event, bool isTemplate = false,
              bool templateOverridesTimes = false);
    void load(const KCalCore::Todo::Ptr &todo, bool isTemplate = false,
              bool templateOverridesTimes = false);
    void load(const KCalCore::Journal::Ptr &journal, bool isTemplate = false,
              bool templateOverridesTimes = false);
    void save(const KCalCore::Event::Ptr &event);
    void save(const KCalCore::Todo::Ptr &todo);
    void save(const KCalCore::Journal::Ptr &journal);
    void setDateTimes(const QDateTime &start, const QDateTime &end);
    void setTimes(const QDateTime &start, const QDateTime &end);
    void setTimeZoneLabelEnabled(bool enable);
    bool timeZonesAreLocal(const QDateTime &start, const QDateTime &end);

private:
    Ui::EventOrTodoDesktop *mUi = nullptr;

    QDate mActiveDate;
    /**
     * These might differ from mLoadedIncidence->(dtStart|dtDue) as these take
     * in account recurrence if needed. The values are calculated once on load().
     * and don't change afterwards.
     */
    QDateTime mInitialStartDT;
    QDateTime mInitialEndDT;

    /**
     * We need to store the current start date/time to be able to update the end
     * time appropriate when the start time changes.
     */
    QDateTime mCurrentStartDateTime;

    /// Remembers state when switching between takes whole day and timed event/to-do.
    bool mTimezoneCombosWereVisibile;
};
}

#endif
