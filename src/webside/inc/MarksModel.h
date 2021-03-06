#ifndef MARKSMODEL_H
#define MARKSMODEL_H

#include <Wt/WAbstractTableModel>

#include "DataMarks.h"
#include "Channel.h"
#include "MarksModelConnector.h"
#include "RSSFeedQuery.h"
#include <QList>
#include <sstream>

using namespace Wt;

class MarksModel : public WAbstractTableModel
{

  QSharedPointer<User> m_user;
  double m_latitude;
  double m_longitude;
  double m_radius;
  MarksModelConnector * m_connector;
  RSSFeedQuery  m_rss;
  QList<QSharedPointer<DataMark> > m_marks;

  void init();
  public:

    enum Type
    {
      ALL_MARKS,
      THE_LATEST_FROM_EACH_MEMBER
    };

    Type   m_type;
    MarksModel(QSharedPointer<User> user, WObject *parent = 0);

    virtual int columnCount(const WModelIndex & parent = WModelIndex()) const;
    virtual int rowCount(const WModelIndex & parent = WModelIndex()) const;
    virtual boost::any data(const WModelIndex & index,
      int role = DisplayRole) const;
    virtual boost::any headerData(int section,
      Orientation orientation = Horizontal,
      int role = DisplayRole) const;
    WFlags<ItemFlag> flags(const WModelIndex &index) const;

    const QList<QSharedPointer<DataMark> >& getMarks() const;
    void marksRecieved();
    void update();
};
/* OPTIONSMODEL_H */
#endif
