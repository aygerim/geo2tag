/*!
 * \file MarkPane.h
 * \brief Header of MarkPane
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _MarkPane_H_D511DD50_5236_4D1D_84DB_72BE3E938E3E_INCLUDED_
#define _MarkPane_H_D511DD50_5236_4D1D_84DB_72BE3E938E3E_INCLUDED_

#include <QtGui/QWidget>
#include "MarkTableDelegat.h"

namespace GUI
{
 /*!
   * Class description. May use HTML formatting
   *
   */
  class MarkPane : public QWidget
  {
    ListView *m_list;
    Q_OBJECT;

  public:

    MarkPane(QWidget * parent);

    virtual ~MarkPane()
    {
    }
  
  public slots:

    void refresh()
    {
      qDebug() << "refresh";       
      qobject_cast<ListModel*>((m_list)->model())->layoutUpdate();
    }  
    

  private:  

    MarkPane(const MarkPane& obj);
    MarkPane& operator=(const MarkPane& obj);

  }; // class MarkPane
  
} // namespace GUI

#endif //_MarkPane_H_D511DD50_5236_4D1D_84DB_72BE3E938E3E_INCLUDED_

/* ===[ End of file ]=== */