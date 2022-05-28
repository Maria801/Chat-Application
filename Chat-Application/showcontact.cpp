#include "showcontact.h"
#include "ui_showcontact.h"
#include "ui_contact.h"
#include <QPixmap>
#include "contact.h"
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QtSql>
#include <iostream>
#include <QVector>
#include "Chats.h"
#include "Data.h"
#include <QMap>


using namespace std;

ShowContact::ShowContact(QWidget *parent): QWidget(parent), ui(new Ui::ShowContact) {
    ui->setupUi(this);
    this->setMinimumSize(QSize(700, 500));
    this->setMaximumSize(QSize(700, 500));

    QString myStyleSheet = "background: url(':/images/assets/login_BackGround.png');";
    this->setStyleSheet(myStyleSheet);

}

ShowContact::~ShowContact() {
    delete ui;
}

void ShowContact::on_pushButton_clicked()
{
    emit exitShowContact();

    ui->listWidget->blockSignals(true);
    ui->listWidget->clear();
    ui->listWidget->blockSignals(false);

    ui->listWidget_2->blockSignals(true);
    ui->listWidget_2->clear();
    ui->listWidget_2->blockSignals(false);

}

void ShowContact::openShowContact() {
    selectedMembers.clear();
    if(groupType == 1) {// groupchat
        ui->listWidget_2->show();
        ui->pushButton_creategroupChat->show();
    }
    else {// privatechat
        ui->listWidget_2->hide();
        ui->pushButton_creategroupChat->hide();
    }
    this->myID = MyConstants::getMyId().toInt();

    QString curr = QString::number(this->myID);
    string cid = curr.toStdString();
    string userCol     =  "UserID,FirstName,LastName,ProfilePicture";
    string userTable     =  "USER" ;
    string userCond     =  ";";
    this->data= db.SelectData(userTable,userCol,userCond);
    cout<< "size is "<< data.size();

    string contactCol       =  "UserID, ContactID";
    string contactTable     =  "CONTACTS" ;
    string contactCond      =  "where UserID ='"+cid+"' ;";
    this->cdata= db.SelectData(contactTable,contactCol,contactCond);
    for(auto row1 : this->data) {

        bool found=0;
        if(row1[0]==curr)
            continue;
        for(auto row2 : this->cdata) {
            if(row1[0]==row2[1]){
                found=1;
                break;
            }
        }

        if(found) {
            ContactWidget *mycontact = new ContactWidget();
            QString myphotopath = "D:/Pictures/My Gallery/ACM/FB_IMG_1645480394683.jpg";
            mycontact->setContactData(myphotopath, row1[1] +" "+ row1[2], "ID: "+row1[0]);

            if(!ui->listWidget_2->isHidden()) {
                mycontact->setGeometry(0,0,250,100);
            }
            int w = mycontact->width();
            int h = mycontact->height();
            QListWidgetItem *item = new QListWidgetItem(ui->listWidget);

            item->setSizeHint(QSize(w, h));
            if(!ui->listWidget_2->isHidden()) {
                ContactWidget *mycontact2 = new ContactWidget();
                myphotopath = "D:/Pictures/My Gallery/ACM/FB_IMG_1645480394683.jpg";
                mycontact2->setContactData(myphotopath, row1[1] +" "+ row1[2], "ID: "+row1[0]);

                QListWidgetItem *item2 = new QListWidgetItem(ui->listWidget_2);
                item2->setSizeHint(QSize(w, h));
                ui->listWidget_2->setItemWidget(item2, mycontact2);
                ui->listWidget_2->item(ui->listWidget_2->count() - 1)->setHidden(true);
            }
            ui->listWidget->setItemWidget(item, mycontact);
        }

    }

}

void ShowContact::on_listWidget_itemClicked(QListWidgetItem *item){
    int index = ui->listWidget->currentRow();
    QString selectedID = cdata[index][1];

    if(groupType==1) {
        selectedMembers[index] = selectedID;
        ui->listWidget->item(index)->setHidden(true);
        ui->listWidget_2->item(index)->setHidden(false);
    }
    else{
        vector<QString> userData = db.SelectData("USER","FirstName, LastName","WHERE UserID = " + db.convertToValue(selectedID)).front();
        QString chatName1 = MyConstants::getMyId() + "#" + selectedID;
        QString chatName2 = selectedID + "#" +MyConstants::getMyId();
        vector<vector<QString>> myChatID = db.SelectData("CHATROOM", "RoomID", "WHERE Name in ("+ db.convertToValue(chatName1)+", "+db.convertToValue(chatName2)+")" );

        if(myChatID.empty()) {

            db.InsertData("CHATROOM","( " + db.convertToValue(chatName1) + " )");
            QString chatID = db.SelectData("CHATROOM","RoomID","ORDER BY RoomID DESC").front().front();
            db.InsertData("CHATROOMINFO","( " + db.convertToValue(chatID) +", 'None', '2', '0' )");
            db.InsertData("PARTICIPATE","( " + db.convertToValue(chatID) + ","+ db.convertToValue(MyConstants::getMyId()) +" , datetime('now') )");
            db.InsertData("PARTICIPATE","( " + db.convertToValue(chatID) + ","+ db.convertToValue(selectedID) +" , datetime('now') )");

            MyConstants::setMyChatRoomID(chatID);
        }
        else{
            MyConstants::setMyChatRoomID(myChatID[0][0]);
        }

        MyConstants::setMyChatRoomName(userData[0] + " " + userData[1]);
        emit openContactChat();
        ui->listWidget->blockSignals(true);
        ui->listWidget->clear();
        ui->listWidget->blockSignals(false);
    }

}

void ShowContact::on_listWidget_2_itemClicked(QListWidgetItem *item) {
    int index = ui->listWidget_2->currentRow();
    ui->listWidget_2->item(index)->setHidden(true);
    ui->listWidget->item(index)->setHidden(false);
    selectedMembers.erase(index);
}
// chatroom, chatinfo, participate
void ShowContact::on_pushButton_creategroupChat_clicked() {
    if(selectedMembers.empty()) return;

    QWidget Wndow_enterChatName;
    QLabel Label_enterChatName;
    QLineEdit *LineEdit_myChatName;

    Label_enterChatName.setText("Enter groupchat name: ");
    Wndow_enterChatName.setAcceptDrops(true);

    for(auto user : selectedMembers) {

    }


}

