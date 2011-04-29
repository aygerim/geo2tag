/*
 * Copyright 2010  OSLL osll@osll.spb.ru
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */
/*!
 * \file Channel.h
 * \brief Header of Channel
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _Channel_H_480D4E41_537B_41D1_A67C_326A700DDC2D_INCLUDED_
#define _Channel_H_480D4E41_537B_41D1_A67C_326A700DDC2D_INCLUDED_

#include <QString>
#include <QVector>
#include <QSharedPointer>

#include "ConcurrentVector.h"
#include "TimeSlot.h"

class Channel: public QObject
{
    Q_OBJECT
    QString m_name; //!< channel name
    QString m_description; //!< Description for channel
    QString m_url; //!< URL for mark
    double m_activeRadius; //< Radius for visible marks

    bool m_isDisplayed; //!< Displayed on the UI

    QSharedPointer<TimeSlot> m_timeSlot;

protected:
    Channel(const QString &name, const QString &description, const QString& url="");

public:

    virtual qlonglong getId() const = 0;

    const QString& getDescription() const;

    const QString& getName() const;

    const QString& getUrl() const;

    void setDescription(const QString& description);

    void setUrl(const QString& url);

    void setRadius(const double &radius);
    double getRadius() const;

    bool isDisplayed() const;
    void setDisplayed(bool);

    void setTimeSlot(QSharedPointer<TimeSlot> timeSlot);
    QSharedPointer<TimeSlot> getTimeSlot() const;

    virtual ~Channel();
}; // class Channel

typedef ConcurrentVector<Channel> Channels;

#endif //_Channel_H_480D4E41_537B_41D1_A67C_326A700DDC2D_INCLUDED_

/* ===[ End of file ]=== */
