#include "cbwnd.h"
#include "mycombobox.h"
#include "mycombodt.h"

#include <QGridLayout>

CBWnd::CBWnd(QWidget *parent) :
    QMainWindow(parent)
{
    QMap<QString,bool> valuesCbCheck;
    QList<ComboDTData> valuesCbDt;

    //пример данных для отображения
    for(int i=0;i<10;++i){
        //1)
        bool b=((i%2)==0)?true:false;
        valuesCbCheck.insert(QString::number(i),b);
        //2)
        ComboDTData dt;
        QColor color((b)?Qt::red:Qt::green);
        dt.color=color;
        valuesCbDt.append(dt);
    }
    MyComboBox *cbCheck=new MyComboBox(valuesCbCheck,this);

    cbCheck->setGeometry(0,0,cbCheck->geometry().width(),
                         cbCheck->geometry().height());

    MyComboDT *cbDt=new MyComboDT(this);

    cbDt->setNewData(valuesCbDt,0);
    cbDt->setGeometry(0,50,cbDt->geometry().width(),
                      cbDt->geometry().height());

    this->setMinimumSize(cbDt->geometry().width(),50);

}
