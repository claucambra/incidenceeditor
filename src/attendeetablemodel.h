/*
 *  Copyright (C) 2014 Sandro Knauß <knauss@kolabsys.com>
 *
 *  This library is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Library General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This library is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to the
 *  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 *  02110-1301, USA.
 */

#ifndef INCIDENCEEDITOR_ATTENDEETABLEMODEL_H
#define INCIDENCEEDITOR_ATTENDEETABLEMODEL_H

#include <KCalCore/Attendee>

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QModelIndex>

namespace IncidenceEditorNG {
class AttendeeTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Roles {
        AttendeeRole = Qt::UserRole
    };

    enum Columns {
        CuType,
        Role,
        FullName,
        Name,
        Email,
        Available,
        Status,
        Response
    };

    enum AvailableStatus {
        Unknown,
        Free,
        Accepted,
        Busy,
        Tentative
    };

    explicit AttendeeTableModel(const KCalCore::Attendee::List &resources,
                                QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;

    bool insertAttendee(int position, const KCalCore::Attendee::Ptr &attendee);

    void setAttendees(const KCalCore::Attendee::List &resources);
    KCalCore::Attendee::List attendees() const;

    void setKeepEmpty(bool keepEmpty);
    bool keepEmpty() const;

    void setRemoveEmptyLines(bool removeEmptyLines);
    bool removeEmptyLines() const;
private:
    void addEmptyAttendee();

    KCalCore::Attendee::List mAttendeeList;
    QMap<KCalCore::Attendee::Ptr, AvailableStatus> mAttendeeAvailable;
    bool mKeepEmpty;
    bool mRemoveEmptyLines;
};

class ResourceFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ResourceFilterProxyModel(QObject *parent = nullptr);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

class AttendeeFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit AttendeeFilterProxyModel(QObject *parent = nullptr);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};
}

#endif
