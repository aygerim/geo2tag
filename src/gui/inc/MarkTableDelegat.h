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
/*  */
/*!
 * \file MarkTableDelegat.h
 * \brief Header of MarkTableDelegat
 * \todo add comment here
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */


#ifndef _MarkTableDelegat_H_C595B6B3_EF48_4F54_879C_E662D3876A5C_INCLUDED_
#define _MarkTableDelegat_H_C595B6B3_EF48_4F54_879C_E662D3876A5C_INCLUDED_

#include <sstream>
#include <QAbstractListModel>
#include <QStandardItemModel>
#include <QWidget>
#include <QTableView>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>
#include <QTimer>
#include "DataMarks.h"
#include <QItemDelegate>
#include <QTableWidget>
#include <QLineEdit>
#include <QKeyEvent>
#include "GpsInfo.h"
#include "Handle.h"
#include "DbSession.h"
#include "User.h"
#include "GpsInfo.h"
#include "ListModel.h"
#include "ListView.h"

namespace GUI
{

 /*!
   * Class description. May use HTML formatting
   *
   */
  class MarkTableDelegat : public QItemDelegate
  {
     Q_OBJECT;
  public:
    MarkTableDelegat(QWidget *parent =NULL);

    virtual ~MarkTableDelegat();

    int columnCount ( const QModelIndex & /*parent = QModelIndex()*/ ) const;
  }; // class MarkTableDelegat
   
} // namespace GUI

#endif //_MarkTableDelegat_H_C595B6B3_EF48_4F54_879C_E662D3876A5C_INCLUDED_

/* ===[ End of file  ]=== */