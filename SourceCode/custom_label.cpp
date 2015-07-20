#include "custom_label.h"

custom_label::custom_label(QWidget *parent)
    :QLabel(parent)
{

}

void custom_label::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    emit clicked();
}
