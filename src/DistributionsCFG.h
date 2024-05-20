#ifndef VKR_DISTRIBUTIONSCFG_H
#define VKR_DISTRIBUTIONSCFG_H

#include <string>
#include <memory>
#include <QComboBox>

#include "Distributions.h"

static vector<string> distro_name = {
        "Равномерный",
        "Нормальный"
};

static shared_ptr<BaseDistribution> get_distro_by_id(int id, double a, double b) {
    if (id == 0) return make_shared<UniformDistribution>(a, b);
    if (id == 1) return make_shared<NormalDistribution>(a, b);
    else return make_shared<UniformDistribution>(a, b);
}

static void init_SB(QComboBox *cb) {
    for (int i = 0; i < distro_name.size(); ++i)
        cb->addItem(QString::fromStdString(distro_name[i]), QVariant(i));
}

#endif