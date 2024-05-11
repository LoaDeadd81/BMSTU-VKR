#ifndef GROUP_H
#define GROUP_H

#include <QWidget>
#include <vector>

#include "ui_group.h"

using namespace std;

struct Distributions{
    string name;
};

class group : public QWidget
{
    Q_OBJECT
public:
    explicit group(QWidget *parent = nullptr);

private:
    Ui::Group *ui;
    vector<Distributions> distros = {
            {"Равномерный"}
    };

signals:
};

#endif // GROUP_H
