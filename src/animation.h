#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include <QMainWindow>
#include <QGraphicsRectItem>
#include <vector>

#include "ui_anim.h"
#include "PetriNetBuilder.h"
#include "EnterAnimItem.h"
#include "ReceptionAnimItem.h"
#include "WindowsGroupAnimItem.h"
#include "ArrowAnimItem.h"

using namespace std;

class AnimationWindow : public QMainWindow {
Q_OBJECT
public:
    AnimationWindow(StatSource src, QWidget *parent = nullptr);

private:
    Ui::Anim *ui;
    QGraphicsScene *scene;
    StatSource src;

    void draw_MFC();
};

#endif
