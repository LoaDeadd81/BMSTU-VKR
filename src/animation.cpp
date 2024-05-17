#include "animation.h"

#include <utility>
#include <QOpenGLWidget>
#include <QSurfaceFormat>

AnimationWindow::AnimationWindow(StatSource src, QWidget *parent)
        : QMainWindow{parent}, ui(new Ui::Anim), src(std::move(src)) {
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, ui->animGV->width(), ui->animGV->height());
    ui->animGV->setScene(scene);
    ui->animGV->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    auto gl = new QOpenGLWidget();
    QSurfaceFormat format;
    format.setSamples(4);
    gl->setFormat(format);
    ui->animGV->setViewport(gl);

    draw_MFC();
}

void AnimationWindow::draw_MFC() {
    int padding = 20;
    int group_num = src.window_src.size(), windows_num = 0;
    int available_space = ui->animGV->height() - padding * 2 - padding * 2 * (src.window_src.size() - 1);
    for (const auto &it: src.window_src) {
        windows_num += it.out_t.size();
        available_space -= padding * (it.out_t.size() - 1);
    }
    int min_size = 10, max_size = 50;
    int size = available_space / windows_num;
    if (size < min_size) size = min_size;
    if (size > max_size) size = max_size;

    int v_center = ui->animGV->height() / 2;

    int x = padding;
    auto enter = new EnterAnimItem(padding, v_center, size);
    scene->addItem(enter);

    x += size * 3;
    auto reception = ReceptionAnimItem(scene, x, v_center, size);

    x += size * 6;
    int y = padding;
    auto groups = vector<WindowsGroupAnimItem>();
    for (int i = 0; i < group_num; ++i) {
        auto group = WindowsGroupAnimItem(scene, x, y, size, src.window_src[i].out_t.size(), padding);
        groups.push_back(group);
        y += group.get_size() + padding * 3;
    }
}
