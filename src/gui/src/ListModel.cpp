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
#include "ListModel.h"
namespace GUI
{
ListModel::ListModel(QObject* parent) : QStandardItemModel(common::DbSession::getInstance().getMarks()->size(),3,parent), m_data(common::DbSession::getInstance().getMarks())
{
  m_size = 0;
}

int ListModel::rowCount(const QModelIndex &/*parent = QModelIndex()*/) const
{
  return m_size;
}

int ListModel::columnCount ( const QModelIndex & /*parent = QModelIndex()*/ ) const
{
  return 3;
}

CHandlePtr<common::Channel> ListModel::getCurrentChannel() const
{
  return m_currentChannel;
}

void ListModel::setDescription(int row, const std::string& data)
{
  size_t c=0,i=0;
  for(; i<m_data->size(); i++)
  {
    if((*m_data)[i]->getChannel() == m_currentChannel)
      c++;
    if((c-1)==row)
      break;
  }

  (*m_data)[i]->setDescription(data);
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
  if ( Qt::DisplayRole == role )
  {
    QString value="?";
    size_t c=0,i=0;
    for(; i<m_data->size(); i++)
    {
      if((*m_data)[i]->getChannel() == m_currentChannel)
        c++;
      if((c-1)==index.row())
        break;
    }

    switch(index.column())
    {
    case 0:
           value = (*m_data)[i]->getLabel().c_str();
     break;

    case 1:
      value = (*m_data)[i]->getDescription().c_str();
      break;

    case 2:
      value = (*m_data)[i]->getUser()->getLogin().c_str();
      break;

    default:
      break;
    }

          return value;
  }
  return QVariant();
}

void ListModel::layoutUpdate(CHandlePtr<common::Channel> channel)
{
  if(channel!=0)
    m_currentChannel=channel;

  double longitude = common::GpsInfo::getInstance().getLongitude();
  double latitude = common::GpsInfo::getInstance().getLatitude();
  size_t size=0;
  for(size_t i=0; i<m_data->size(); i++)
  {
    CHandlePtr<common::DataMark> mark = (*m_data)[i];
    if(mark->getChannel()              == m_currentChannel &&
       mark->getChannel()->getRadius() >  common::DataMark::getDistance(latitude, longitude,mark->getLatitude(), mark->getLongitude()))
     size++;
  }
  m_size = size;
  setRowCount(size);
  emit layoutChanged();
}
}