#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H

#include <QListView>
#include <QMouseEvent>
#include <QComboBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QPair>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QRect>
#include <QMessageBox>
#include <QStylePainter>
#include <QListView>

class MyComboBox : public QComboBox {
  Q_OBJECT
  bool b_cb_open;
  QListView* listView;
  QStringList mCheckedItems;

  void setValues(QMap<QString,bool> values);

  QString displayText;
  QRect* displayRectDelta;

  void updateDisplayText();

public:
  explicit MyComboBox(QMap<QString,bool> values, QWidget* parent = 0);
  QStringList checkedItems();
  void setCheckedItems(QStringList& items);
  void collectCheckedItems();
  void returnValues(QMap<QString, bool> &values);
protected:
  bool eventFilter(QObject* watched, QEvent* event);
  void showPopup();
  void paintEvent(QPaintEvent* event);
  void resizeEvent(QResizeEvent* event);

signals:

private slots:
  void slotModelItemChanged(QStandardItem* item);

};

#endif // MYCOMBOBOX_H
