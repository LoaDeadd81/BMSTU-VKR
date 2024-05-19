#include "animation.h"

#include <utility>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QGraphicsItemAnimation>

#define M_TO_MS(a) (a * 60000.0)
#define S_TO_MS(a) (a * 1000.0)
#define MS_TO_M(a) (a / 60000.0)

AnimationWindow::AnimationWindow(StatSource src, vector<vector<PetriStatEvent>> logs, int mtime, QWidget *parent)
        : QMainWindow{parent}, ui(new Ui::Anim), src(std::move(src)), logs(std::move(logs)), mtime(mtime) {
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
//    scene->setSceneRect(0, 0, ui->animGV->width(), ui->animGV->height());
    ui->animGV->setScene(scene);
    ui->animGV->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->animGV->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->animGV->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto gl = new QOpenGLWidget();
    QSurfaceFormat format;
    format.setSamples(4);
    gl->setFormat(format);
    ui->animGV->setViewport(gl);

    draw_MFC();
    create_anims();
    init_time();
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

//    int max_h = padding * 2 + padding * 2 * (group_num - 1) + padding * (windows_num - group_num + 1);
//    scene->setSceneRect(0, 0, ui->animGV->width(), max_h);

    int v_center = ui->animGV->height() / 2;

    int x = padding;
    enter = new EnterAnimItem(padding, v_center, size);
    scene->addItem(enter);

    x += size * 3;
    reception = ReceptionAnimItem(scene, x, v_center, size);
    draw_arrow(scene, enter->get_out_place(), reception.queue->get_in_place(), size);

    x += size * 6;
    int y = padding;
    groups = vector<WindowsGroupAnimItem>();
    for (int i = 0; i < group_num; ++i) {
        auto group = WindowsGroupAnimItem(scene, x, y, size, src.window_src[i].out_t.size(), padding);
        draw_arrow(scene, reception.reception->get_out_place(), group.queue->get_in_place(), size);

        groups.push_back(group);
        y += group.get_size() + padding * 3;
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
}

TransactAnim AnimationWindow::anim_from_logs(vector<PetriStatEvent> log) {
    int trans_time = S_TO_MS(10) / anim_speed, inv_time = 1;
    int index = 0;
    auto group = new QSequentialAnimationGroup(this);
    auto transact = new TransactAnimItem(log[index].transact.type, enter->get_out_place().x(),
                                         enter->get_out_place().y(), size);
    connect(group, &QPropertyAnimation::finished, transact, &TransactAnimItem::on_animFinish);
    TransactAnim res = {int(M_TO_MS(log[index].sys_time) / anim_speed), transact, group};


    index++;
    if (log.size() <= index) return res;

    auto anim_to_q = new QPropertyAnimation(transact, "pos");
    anim_to_q->setDuration(trans_time);
    anim_to_q->setStartValue(QPointF(0, 0));
    anim_to_q->setEndValue(reception.queue->get_in_place() - enter->get_out_place());
    group->addAnimation(anim_to_q);

    if (log[index].t == src.reception_q_src.leave_t) {
        auto anim_to_leave = new QPropertyAnimation(transact, "pos");
        anim_to_leave->setDuration(trans_time);
        anim_to_leave->setStartValue(QPointF(0, 0));
        anim_to_leave->setEndValue(reception.queue->get_exit_place() - reception.queue->get_leave_place());
        group->addAnimation(anim_to_leave);

        return res;
    }

    index++;
    if (log.size() <= index) return res;

    auto to_invis_anim = new QPropertyAnimation(transact, "visible");
    to_invis_anim->setDuration(inv_time);
    to_invis_anim->setStartValue(true);
    to_invis_anim->setEndValue(false);
    group->addAnimation(to_invis_anim);

    int q_wait_time = M_TO_MS(log[index].gen_time) / anim_speed - trans_time - inv_time;
    group->addPause(q_wait_time);

    auto from_invis_anim = new QPropertyAnimation(transact, "visible");
    from_invis_anim->setDuration(inv_time);
    from_invis_anim->setStartValue(false);
    from_invis_anim->setEndValue(true);
    group->addAnimation(from_invis_anim);

    return res;
}

void AnimationWindow::on_timeTick(int frame) {
    ui->label->setText(QString("Время: %1 %2").arg(int(MS_TO_M(frame) * anim_speed)).arg(frame));

    while (!anim_list.empty() && anim_list.front().time < frame) {
        auto tmp = anim_list.front();
        anim_list.pop_front();
        scene->addItem(tmp.transact);
        tmp.anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

