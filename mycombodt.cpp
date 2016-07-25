#include "mycombodt.h"

#define DATEFORMAT "dd.MM.yy hh:mm"

MyComboDT::MyComboDT(QWidget* parent) :
  QComboBox(parent) {
  this->setFixedWidth(300);
  QObject::connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(onIndexChanged(QString)));

}

void MyComboDT::setNewData(QList<ComboDTData> data, int currentID) {
  QObject::disconnect(this, SIGNAL(currentIndexChanged(QString)),
                      this, SLOT(onIndexChanged(QString)));

  //1) clear data & combobox
  this->clear();
  dtData.clear();

  //2) fill new data
  dtData.append(data);

  qSort(dtData.begin(), dtData.end());
  //3) fill combobox with new data
  int i = 0;
  foreach (ComboDTData dt, dtData) { //data) {
    QString st = dt.start.toString(DATEFORMAT) + " - " + dt.end.toString(DATEFORMAT);
    this->insertItem(i, st);
    this->setItemData(i, QVariant::fromValue<QColor>(dt.color),  Qt::DecorationRole);
    i++;
  }
  //4) set current index
  QObject::connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(onIndexChanged(QString)));
  setSelectedId(currentID);
}

void MyComboDT::clearAll() {
  QObject::disconnect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(onIndexChanged(QString)));
  //1) clear data & combobox
  this->clear();
  dtData.clear();
  QObject::connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(onIndexChanged(QString)));
}


void MyComboDT::onIndexChanged(QString st) {
  foreach (ComboDTData dt, dtData) {
    if(dt.text == st) {
      QObject::disconnect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(onIndexChanged(QString)));
      QObject::connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(onIndexChanged(QString)));
      break;
    }
  }
}

void MyComboDT::setSelectedId(int id) {
  int i = 0;
  foreach (ComboDTData dt, dtData) {
    if(dt.id == id) {
      QObject::disconnect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(onIndexChanged(QString)));
      this->setCurrentIndex(i);
      QObject::connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(onIndexChanged(QString)));
      break;
    }
    i++;
  }

}
