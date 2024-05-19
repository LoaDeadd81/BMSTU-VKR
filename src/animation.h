#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include <QMainWindow>
#include <QGraphicsRectItem>
#include <QPropertyAnimation>
#include <vector>
#include <QTimeLine>
#include <unordered_map>
#include <QSequentialAnimationGroup>
#include <QPauseAnimation>

#include <iostream>

#include "ui_anim.h"
#include "PetriNetBuilder.h"
#include "EnterAnimItem.h"
#include "ReceptionAnimItem.h"
#include "WindowsGroupAnimItem.h"
#include "ArrowAnimItem.h"
#include "PetriNet.h"
#include "TransactAnimItem.h"

using namespace std;

struct TransactAnim {
    int time;
    TransactAnimItem *transact;
    QSequentialAnimationGroup *anim;
};

class AnimationWindow : public QMainWindow {
Q_OBJECT
public:
    AnimationWindow(StatSource src, vector<vector<PetriStatEvent>> logs, int mtime, QWidget *parent = nullptr);

private:
    Ui::Anim *ui;
    QGraphicsScene *scene;
    StatSource src;
    vector<vector<PetriStatEvent>> logs;

    int size;
    EnterAnimItem *enter{};
    ReceptionAnimItem reception{};
    vector<WindowsGroupAnimItem> groups;

    int mtime;
    QTimeLine *tl;
    deque<TransactAnim> anim_list;
    int anim_speed = 100;

    void draw_MFC();

    void create_anims();

    void init_time();

    TransactAnim anim_from_logs(vector<PetriStatEvent> logs);

public slots:

    void on_timeTick(int frame);
};

#endif
