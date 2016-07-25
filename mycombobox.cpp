#include "mycombobox.h"

MyComboBox::MyComboBox(QMap<QString, bool> values, QWidget* parent ) :
    QComboBox(parent) {

    displayRectDelta = new QRect(4, 1, -25, 0);
    displayText = "";

    setEditable(false);
    // заменяем стандартный вьювер
    listView = new QListView(this);
    setView(listView);
    // устанавливаем перехват событий
    listView->viewport()->installEventFilter(this);//this);
    listView->installEventFilter(listView);
    // флаг открытия комбобокса
    b_cb_open = false;
    this->setInsertPolicy(QComboBox::InsertAtBottom);
    this->addItem("---Seleccione---");
    this->setItemData(0, Qt::Unchecked, Qt::CheckStateRole);

    setValues(values);

    QStandardItemModel* standartModel = qobject_cast<QStandardItemModel*>(model());
    connect(standartModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
    collectCheckedItems();
}


void MyComboBox::returnValues(QMap<QString, bool> &values)
{
    QStandardItemModel* standartModel = qobject_cast<QStandardItemModel*>(model());
    for (int i = 0; i < count(); ++i) {
        QStandardItem* currentItem = standartModel->item(i);
        //1) name
        QString key = currentItem->text();
        //2) value
        bool value = false;
        Qt::CheckState checkState = static_cast<Qt::CheckState>(currentItem->data(Qt::CheckStateRole).toInt());
        if (checkState == Qt::Checked) {
            value = true;
        }
        values.insert(key,value);
    }
}

void MyComboBox::setValues(QMap<QString, bool> values) {
    int vals_size=values.size();
    QMap<QString, bool>::iterator it=values.begin();
    QStandardItemModel* model = new QStandardItemModel(vals_size, 1);
    for(int i=0; it !=values.end(); ++it,++i) {
        QStandardItem* si;
        si = new QStandardItem(QString("%0").arg((it.key())));//название поля
        si->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled
                     | Qt::ItemIsSelectable);
        if(it.value() == true)//выбрано поле или нет
            si->setData(Qt::Checked, Qt::CheckStateRole);
        else
            si->setData(Qt::Unchecked, Qt::CheckStateRole);
        model->setItem(i, 0, si);
    }
    this->setModel(model);
}

void MyComboBox::updateDisplayText() {
    // определяем границы выводимого текста, mDisplayRectDelta сдвигает текст "вовнутрь" виджета
    // с учётом того, что справа находится кнопка, раскрывающая список
    QRect textRect = rect().adjusted(displayRectDelta->left(), displayRectDelta->top(),
                                     displayRectDelta->right(), displayRectDelta->bottom());

    QFontMetrics fontMetrics(font());

    // разделяем запятыми
    displayText = mCheckedItems.join(", ");

    // если текст выходит за границы
    if (fontMetrics.size(Qt::TextSingleLine, displayText).width() > textRect.width()) {
        // обрезаем его посимвольно, пока не будет в пределах границы
        while (displayText != "" && fontMetrics.size(Qt::TextSingleLine, displayText + "...").width() > textRect.width()) {
            displayText.remove(displayText.length() - 1, 1);
        }

        // дополняем троеточием
        displayText += "...";
    }
}


//MyComboBox::~MyComboBox()
//{

//    if( m_listView)
//        delete  m_listView;
//    if(mDisplayRectDelta)
//        delete mDisplayRectDelta;
//}

QStringList MyComboBox::checkedItems() {
    return mCheckedItems;
}

void MyComboBox::collectCheckedItems() {
    QStandardItemModel* standartModel = qobject_cast<QStandardItemModel*>(model());

    mCheckedItems.clear();

    for (int i = 0; i < count(); ++i) {
        QStandardItem* currentItem = standartModel->item(i);

        Qt::CheckState checkState = static_cast<Qt::CheckState>(currentItem->data(Qt::CheckStateRole).toInt());

        if (checkState == Qt::Checked) {
            mCheckedItems.push_back(currentItem->text());
        }
    }

    updateDisplayText();
    repaint();
}
void MyComboBox::setCheckedItems(QStringList& items) {
    // необходимо приведение
    QStandardItemModel* standartModel = qobject_cast<QStandardItemModel*>(model());

    // отсоединяемся от сигнала, на время установки элементам флажков
    disconnect(standartModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));

    for (int i = 0; i < items.count(); ++i) {
        // ищем индекс элемента
        int index = findText(items.at(i));

        if (index != -1) {
            // устанавливаем элементу флажок
            standartModel->item(index)->setData(Qt::Checked, Qt::CheckStateRole);
        }
    }

    // присоединяемся к сигналу
    connect(standartModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));

    // обновляем список отмеченных элементов
    collectCheckedItems();
}

bool MyComboBox::eventFilter(QObject* watched, QEvent* event) {

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast <QMouseEvent*> (event);

        if (mouseEvent -> button() == Qt::RightButton) {
            QComboBox::hidePopup();
            return QObject::eventFilter(watched, event);
        }
    }


    //проверка тика отловленного события
    if (event->type() == QEvent::MouseButtonRelease) {
        // блокируем смену галочки при открытии
        if (b_cb_open) {
            b_cb_open = false;
            return QObject::eventFilter(watched, event);
        }
        //проверяем тип
        if (watched->parent()->inherits("QListView")) {
            // приводим к нужным типам
            QListView* tmp = (QListView*)(watched->parent());
            QMouseEvent* mEvent = (QMouseEvent*)event;
            QModelIndex ind = tmp->indexAt(mEvent->pos());

            // меняем состояние cheched
            bool checked = tmp->model()->data(ind, Qt::CheckStateRole).toBool();
            if(!checked)
                tmp->model()->setData(ind, Qt::Checked, Qt::CheckStateRole);
            else
                tmp->model()->setData(ind, Qt::Unchecked, Qt::CheckStateRole);
            tmp->updateGeometry();//(ind);
            // блокируем закрытие комбобокса
            return true;
        }
    }


    return QObject::eventFilter(watched, event);
}
////////////////////////////////////////////////////////////////////////////////
// функция раскрытия комбобокса
////////////////////////////////////////////////////////////////////////////////
void MyComboBox::showPopup() {
    // флаг открытия комбобокса
    b_cb_open = true;
    // вызываем функцию класса - предка
    QComboBox::showPopup();
}

void MyComboBox::paintEvent(QPaintEvent* event) {
    (void)event;

    QStylePainter painter(this);

    painter.setPen(palette().color(QPalette::Text));

    QStyleOptionComboBox option;

    initStyleOption(&option);

    // рисуем базовую часть виджета
    painter.drawComplexControl(QStyle::CC_ComboBox, option);

    // определяем границы выводимого текста
    QRect textRect = rect().adjusted(displayRectDelta->left(), displayRectDelta->top(),
                                     displayRectDelta->right(), displayRectDelta->bottom());

    // рисуем текст
    painter.drawText(textRect, Qt::AlignVCenter, displayText);
}

void MyComboBox::resizeEvent(QResizeEvent* event) {
    (void)event;

    updateDisplayText();

}

void MyComboBox::slotModelItemChanged(QStandardItem* item) {
    Q_UNUSED(item);
    collectCheckedItems();
}
