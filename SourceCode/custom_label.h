#ifndef CUSTOM_LABEL_H
#define CUSTOM_LABEL_H

#include <QObject>
#include <QMouseEvent>
#include <QLabel>

class custom_label : public QLabel
{
    Q_OBJECT

public:
    custom_label(QWidget *parent = 0);

protected:
    virtual void mouseReleaseEvent(QMouseEvent * ev);

signals:
    void clicked(void);
};

#endif // CUSTOM_LABEL_H
