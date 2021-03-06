#ifndef USERSWIDGET_H
#define USERSWIDGET_H

#include <Wt/WContainerWidget>
#include <Wt/WSelectionBox>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WStackedWidget>
#include <Wt/WTable>

using namespace Wt;

class UsersWidget : public WContainerWidget
{
  WSelectionBox *usersBox;
  WPushButton *addButton;
  WPushButton *removeButton;
  WStackedWidget *actionWidget;
  /* contains widget for concrete action
                                       with usersBox, for example widget
                                       for input inforamtion about new
                                       user */

  /* User information widget */
  WTable *addUserWidget;
  WLineEdit *login;
  WLineEdit *password;
  WLineEdit *password2;
  WPushButton *addUserButton;
  WPushButton *cancelUserButton;
  WText *message;

  std::string m_token;
  public:
    UsersWidget(const std::string &token,WContainerWidget *parent = 0);

    void updateUsersBox();

    /* Slots */
    void addClicked();
    void removeClicked();
    void addUserClicked();
    void cancelUserClicked();

};
// USERSWIDGET_H
#endif
