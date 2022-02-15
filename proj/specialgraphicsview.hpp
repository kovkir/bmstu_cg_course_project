#ifndef SPECIALGRAPHICSVIEW_HPP
#define SPECIALGRAPHICSVIEW_HPP

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>

class SpecialGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SpecialGraphicsView(QWidget *parent = nullptr);
};

#endif // SPECIALGRAPHICSVIEW_HPP
