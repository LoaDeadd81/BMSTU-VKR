#include "animation.h"

#include <utility>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QGraphicsItemAnimation>

#define M_TO_MS(a) ((a) * 60000.0)
#define S_TO_MS(a) ((a) * 1000.0)
#define MS_TO_M(a) ((a) / 60000.0)

AnimationWindow::AnimationWindow(StatSource src, vector<vector<PetriStatEvent>> logs, int mtime, int anim_speed,
                                 unordered_map<int, vector<QLog>> q_logs, QWidget *parent)
        : QMainWindow{parent}, ui(new Ui::Anim), src(std::move(src)), logs(std::move(logs)), mtime(mtime),
          anim_speed(anim_speed), q_logs(std::move(q_logs)) {
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
//    scene->setSceneRect(0, 0, 1, 1);
    ui->animGV->setScene(scene);
    ui->animGV->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->animGV->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->animGV->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto gl = new QOpenGLWidget();
    QSurfaceFormat format;
    format.setSamples(4);
    gl->setFormat(format);
    ui->animGV->setViewport(gl);

    set_desc();
    draw_MFC();
    sort(begin(q_anim), end(q_anim), [](QAnim a, QAnim b) {
        return a.time < b.time;
    });

    create_anims();
    init_time();
}

void AnimationWindow::set_desc() {
    for (int i = 0; i < src.gen_num; ++i) {
        auto clr = new QFrame(this);
        clr->setStyleSheet("background-color:" + color_vec[i].name());

        auto name = new QLabel(QString("ГЗ %1").arg(i + 1));

        ui->colorGl->addWidget(clr, i, 0);
        ui->colorGl->addWidget(name, i, 1);
    }

    auto entScene = new QGraphicsScene(this);
    ui->entGW->setScene(entScene);
    entScene->addItem(new EnterAnimItem(0, 0, 40));

    auto qScene = new QGraphicsScene(this);
    ui->qGW->setScene(qScene);
    qScene->addItem(new QueueAnimItem(0, 0, 40));

    auto oaScene = new QGraphicsScene(this);
    ui->oaGW->setScene(oaScene);
    oaScene->addItem(new WorkerAnimItem(0, 0, 40));
}

void AnimationWindow::draw_MFC() {
    int padding = 20;
    int group_num = src.window_src.size(), windows_num = 0;
    int available_space = ui->animGV->height() - padding * 2 - padding * 2 * (src.window_src.size() - 1);
    for (const auto &it: src.window_src) {
        windows_num += it.out_t.size();
        available_space -= padding * (it.out_t.size() - 1);
    }
    int min_size = 20, max_size = 50;
    size = available_space / windows_num;
    if (size < min_size) size = min_size;
    if (size > max_size) size = max_size;

    int max_h =
            padding + padding * 2 * (group_num - 1) + padding * (windows_num - group_num) + size * windows_num +
            size / 4;

    int v_center = max_h / 2;

    int x = padding;
    enter = new EnterAnimItem(padding, v_center, size);
    scene->addItem(enter);

    x += size * 3;
    reception = ReceptionAnimItem(scene, x, v_center, size);
    draw_arrow(scene, enter->get_out_place(), reception.queue->get_in_place(), size);
    auto r_q_size = new QSizeAnimItem(reception.queue->get_center().x(), reception.queue->get_center().y(), size);
    scene->addItem(r_q_size);
    create_q_anims(r_q_size, q_logs[src.reception_q_src.queue_p]);

    x += size * 6;
    int y = padding;
    groups = vector<WindowsGroupAnimItem>();
    for (int i = 0; i < group_num; ++i) {
        auto group = WindowsGroupAnimItem(scene, x, y, size, src.window_src[i].out_t.size(), padding);
        draw_arrow(scene, reception.reception->get_out_place(), group.queue->get_in_place(), size);
        auto w_q_size = new QSizeAnimItem(group.queue->get_center().x(), group.queue->get_center().y(), size);
        scene->addItem(w_q_size);
        create_q_anims(w_q_size, q_logs[src.window_q_src[i].queue_p]);

        groups.push_back(group);
        y += group.get_size() + padding * 3;
    }
}

void AnimationWindow::create_q_anims(QSizeAnimItem *item, const vector<QLog> &q_log) {

    for (auto &log: q_log) {
        auto anim = new QPropertyAnimation(item, "txt", this);
        anim->setDuration(1);
        anim->setStartValue("");
        anim->setEndValue(log.size);
        q_anim.emplace_back(M_TO_MS(log.time) / anim_speed, anim);
    }
}

void AnimationWindow::create_anims() {
    for (auto &it: logs) {
        anim_list.push_back(anim_from_logs(it));
    }
    sort(begin(anim_list), end(anim_list), [](TransactAnim a, TransactAnim b) {
        return a.time < b.time;
    });
}

void AnimationWindow::init_time() {
    int ms_time = M_TO_MS(mtime) / anim_speed;
    tl = new QTimeLine(ms_time, this);
    connect(tl, &QTimeLine::frameChanged, this, &AnimationWindow::on_timeTick);
    tl->setFrameRange(0, ms_time);
    tl->setEasingCurve(QEasingCurve::Linear);
    tl->start();

//    q_anims->start(QAbstractAnimation::DeleteWhenStopped);
}

void
move_trans(int trans_time, QPointF start, QPointF end, TransactAnimItem *transact, QSequentialAnimationGroup *group) {
    auto anim = new QPropertyAnimation(transact, "pos");
    anim->setDuration(trans_time);
    anim->setStartValue(transact->mapFromScene(start));
    anim->setEndValue(transact->mapFromScene(end));
    group->addAnimation(anim);
}

void
change_visibility(int inv_time, bool to_visible, TransactAnimItem *transact, QSequentialAnimationGroup *group) {
    qreal from = 1, to = 0;
    if (to_visible) swap(from, to);

    auto anim = new QPropertyAnimation(transact, "opacity");
    anim->setDuration(inv_time);
    anim->setStartValue(from);
    anim->setEndValue(to);
    group->addAnimation(anim);
}

TransactAnim AnimationWindow::anim_from_logs(vector<PetriStatEvent> log) {
    int trans_time = S_TO_MS(10) / anim_speed, inv_time = 1;
    int index = 0;
    auto group = new QSequentialAnimationGroup(this);
    auto transact = new TransactAnimItem(log[index].transact.type, enter->get_out_place().x(),
                                         enter->get_out_place().y(), size);
    connect(group, &QPropertyAnimation::finished, transact, &TransactAnimItem::on_animFinish);
    TransactAnim res = {int(M_TO_MS(log[index].sys_time) / anim_speed), transact, group};
    double gen_time = log[index].sys_time;


    index++; // 1
    if (log.size() <= index) return res;
    move_trans(trans_time, enter->get_out_place(), reception.queue->get_in_place(), transact, group);
    if (log[index].t == src.reception_q_src.leave_t) {
        move_trans(trans_time, reception.queue->get_leave_place(), reception.queue->get_exit_place(), transact, group);

        return res;
    }


    index++; // 2
    if (log.size() <= index) return res;
    int q_wait_time = M_TO_MS(log[index].sys_time - log[index].gen_time - gen_time) / anim_speed;

    if (q_wait_time > 0) {
        change_visibility(inv_time, false, transact, group);
        group->addPause(q_wait_time);
        change_visibility(inv_time, true, transact, group);
    }

    move_trans(trans_time, reception.queue->get_out_place(), reception.reception->get_center(), transact, group);

    int rec_wait_time = M_TO_MS(log[index].gen_time) / anim_speed - trans_time * 2;
    group->addPause(rec_wait_time);


    index++; // 3
    if (log.size() <= index) return res;
    index++; // 4
    if (log.size() <= index) return res;

    int group_index = src.type_w_index[log[index].transact.type - 1];
    auto gw_item = groups[group_index];
    auto gw_q = src.window_q_src[group_index];

    move_trans(trans_time, reception.reception->get_center(), gw_item.queue->get_in_place(), transact, group);
    if (log[index].t == gw_q.leave_t) {
        move_trans(trans_time, gw_item.queue->get_leave_place(), gw_item.queue->get_exit_place(), transact, group);

        return res;
    }


    index++; // 5
    if (log.size() <= index) return res;
    auto gw = src.window_src[group_index];
    int window_index = 0;
    while (gw.in_t[window_index] != log[index].t) window_index++;
    auto worker = gw_item.workers[window_index];

    q_wait_time = M_TO_MS(log[index].sys_time - log[index - 1].sys_time) / anim_speed;
    if (q_wait_time > 0) {
        change_visibility(inv_time, false, transact, group);
        group->addPause(q_wait_time);
        change_visibility(inv_time, true, transact, group);
    }
    move_trans(trans_time, gw_item.queue->get_out_place(), worker->get_center(), transact, group);

    index++; // 6
    if (log.size() <= index) return res;
    int win_wait_time = M_TO_MS(log[index].gen_time) / anim_speed - trans_time * 2;
    group->addPause(win_wait_time);
    move_trans(trans_time, worker->get_center(), worker->get_exit_place(), transact, group);

    return res;
}

void AnimationWindow::on_timeTick(int frame) {
    ui->label->setText(QString("Время: %1").arg(int(MS_TO_M(frame) * anim_speed)));

//    if (q_anim_not_start) {
//        q_anims->start(QAbstractAnimation::DeleteWhenStopped);
//        q_anim_not_start = false;
//    }

    while (!q_anim.empty() && q_anim.front().time < frame) {
        auto tmp = q_anim.front();
        q_anim.pop_front();
        tmp.anim->start(QAbstractAnimation::DeleteWhenStopped);
    }

    while (!anim_list.empty() && anim_list.front().time < frame) {
        auto tmp = anim_list.front();
        anim_list.pop_front();
        scene->addItem(tmp.transact);
        tmp.anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

