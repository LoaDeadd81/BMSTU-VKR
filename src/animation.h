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
#include <QParallelAnimationGroup>

#include <iostream>

#include "ui_anim.h"
#include "PetriNetBuilder.h"
#include "EnterAnimItem.h"
#include "ReceptionAnimItem.h"
#include "WindowsGroupAnimItem.h"
#include "ArrowAnimItem.h"
#include "PetriNet.h"
#include "TransactAnimItem.h"
#include "QSizeAnimItem.h"

using namespace std;

struct TransactAnim {
    int time;
    TransactAnimItem *transact;
    QSequentialAnimationGroup *anim;
};

struct QAnim {
    int time;
    QPropertyAnimation *anim;
};

class AnimationWindow : public QMainWindow {
Q_OBJECT
public:
    AnimationWindow(StatSource src, vector<vector<PetriStatEvent>> logs, int mtime, int anim_speed,
                    unordered_map<int, vector<QLog>> q_logs, QWidget *parent);

private:
    Ui::Anim *ui;
    QGraphicsScene *scene;
    StatSource src;
    vector<vector<PetriStatEvent>> logs;
    unordered_map<int, vector<QLog>> q_logs;

    int size;
    EnterAnimItem *enter{};
    ReceptionAnimItem reception{};
    vector<WindowsGroupAnimItem> groups;

    int mtime;
    QTimeLine *tl;
    deque<TransactAnim> anim_list;
    deque<QAnim> q_anim;
    int anim_speed = 50;

    void set_desc();

    void draw_MFC();

    void create_q_anims(QSizeAnimItem *item, const vector<QLog> &q_log);

    void create_anims();

    void init_time();

    TransactAnim anim_from_logs(vector<PetriStatEvent> logs);

public slots:

    void on_timeTick(int frame);
};

#endif
