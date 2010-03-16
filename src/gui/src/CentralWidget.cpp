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
/*! ---------------------------------------------------------------
 *  
 *
 * \file CentralWidget.cpp
 * \brief CentralWidget implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include "CentralWidget.h"
#include <QMessageBox>

namespace GUI
{
    CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
    {
      m_palette = new QPalette(Qt::black);
      setPalette(*m_palette);
      m_layout = new QStackedLayout(this);

      m_mapView  = new MapPane(this);
      m_feedView = new MarkPane(this);
      m_channelView = new ChannelPane(this);
      m_editor = new MarkEditor(this);
      
      m_layout->addWidget(m_mapView);
      m_layout->addWidget(m_feedView);
      m_layout->addWidget(m_channelView);
      m_layout->addWidget(m_editor);
      
      switchMap();

      connect(m_channelView, SIGNAL(      clicked(const QModelIndex&)), this, SLOT(switchFeed(const QModelIndex&)));


      setLayout(m_layout);
    }

    void CentralWidget::switchMap()
    {
      m_layout->setCurrentWidget(m_mapView); 
    }

    void CentralWidget::switchFeed(const QModelIndex& index)
    {
//TODO: get real channel info
// qobject_cast<model>->getChannelDescription
      QMessageBox::information(this,"Channel information",(*common::DbSession::getInstance().getChannels())[index.row()]->getDescription().c_str());
      // TODO get active channel from index, switch to m_feedView
      m_feedView->refresh((*common::DbSession::getInstance().getChannels())[index.row()]
);      
      m_layout->setCurrentWidget(m_feedView); 
    }

    void CentralWidget::switchChannel()
    {
      m_layout->setCurrentWidget(m_channelView); 
    }

    void CentralWidget::switchEditor()
    {
      m_layout->setCurrentWidget(m_editor); 
    }
    
    void CentralWidget::setRadius()
    {
      //TODO: ask real raduis
      if(m_layout->currentWidget()!=m_feedView)
      {
        QMessageBox::information(this,"Set channel raduis","You cannot change radius for this channel. You should select channel before.");
      }
      else
      {
        m_feedView->updateCurrentChannelRadius();  
      }
    }



} // namespace GUI

/* ===[ End of file ]=== */