#ifndef MYCOMBODT_H
#define MYCOMBODT_H

#include <QComboBox>
#include <QMessageBox>
#include <QDateTime>
struct ComboDTData {
  QDateTime start;
  QDateTime end;
  QString text;
  int id;
  QColor color;
  ComboDTData(QDateTime s,
              QDateTime e,
              QString st, int id, QColor color):
      start(s),
      end(e),
      text(st),
      id(id),
      color(color)
  {

  }
  ComboDTData(){
      start = QDateTime::currentDateTime();
      end = QDateTime::currentDateTime();
      text = "error";
      id = 0;
      color = QColor(Qt::black);
  }
  bool operator <(const ComboDTData& other) const {
      if(this->end < other.end && this->start < other.start) {
          return true;
      }
      return false;
  }
  bool operator >(const ComboDTData& other) const {
      if(this->end > other.end && this->start > other.start) {
          return true;
      }
      return false;
  }
};


class MyComboDT : public QComboBox {
  Q_OBJECT

  QList<ComboDTData> dtData;
public:
  explicit MyComboDT(QWidget* parent = 0);
  void setNewData(QList<ComboDTData>, int currentID);
  void sortData();
  void clearAll();

signals:
public slots:
  void onIndexChanged(QString);
  void setSelectedId(int id);
};
#endif // MYCOMBODT_H
