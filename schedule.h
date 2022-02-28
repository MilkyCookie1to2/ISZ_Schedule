#ifndef SCHEDULE_H
#define SCHEDULE_H


#include <QStringList>
#include <QVariant>

class Schedule : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList variantList READ variantList NOTIFY variantListChanged)
    Q_PROPERTY(int for_widtho READ for_widtho NOTIFY for_widthoChanged)

public:
    explicit Schedule(QObject *parent = nullptr) : QObject(parent) { }

    QStringList variantList() const { return list_day; }
    int for_widtho() const { return for_width; }

    friend class MainWindow;

signals:
    void variantListChanged();
    void for_widthoChanged();

public slots:

private:
    QStringList list_day;
    int for_width;
};


#endif // SCHEDULE_H
