#include "mainwindow.h"

#include <QApplication>

#include <iostream>
#include <ctime>
#include <fstream>

using namespace std;

void measure_levels();

int main(int argc, char *argv[]) {
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();

    measure_levels();

//    vector<TypeInfo> type_info = {
//            {1, make_shared<UniformDistribution>(4, 6), make_shared<ConstDistribution>(1)},
//            {2, make_shared<UniformDistribution>(4, 6), make_shared<ConstDistribution>(1)},
//            {3, make_shared<UniformDistribution>(4, 6), make_shared<ConstDistribution>(1)},
//    };
//    ReceptionInfo rec_info = {make_shared<ConstDistribution>(1), 10};
//    vector<WinGroupInfo> win_info = {
//            {2, {1}, 10},
//            {2, {2}, 10},
//            {2, {3}, 10},
//    };
//
//    auto builder = PetriNetBuilder(type_info, rec_info, win_info);
//    auto p_net = builder.build();
//    auto src = builder.get_stat_src();
//
//    auto drawer = PetriNetDrawer(p_net->get_import_data());
//    drawer.draw("net.svg");
//
//    int mtime = 720;
//    DelftParam param(1, 5, mtime);
//    auto delft = Delft(param, p_net);
//    delft.run();
//
//    auto stats = p_net->get_stats();
//
//    auto agregator = StatsAggregator(mtime, src, stats.first, stats.second);
//    auto smoStats = agregator.get_stats();
//
//    cout << "";
}

vector<double> measure_ro(vector<double> lmbd_vec, double mu, double delta, DelftParam param) {
    double step = 0.1;
    int repeat_num = 20;

    ReceptionInfo rec_info = {make_shared<UniformDistribution>(mu / 3 - delta, mu / 3 + delta),
                              10};
    vector<WinGroupInfo> win_info = {
            {1, {1}, 10},
            {1, {2}, 10},
            {1, {3}, 10},
    };

    vector<double> res(lmbd_vec.size());
    int i = 0;
    for (auto &lmbd: lmbd_vec) {
        vector<TypeInfo> type_info = {
                {1, make_shared<UniformDistribution>(lmbd - delta, lmbd + delta), make_shared<UniformDistribution>(
                        mu - delta, mu + delta)},
                {2, make_shared<UniformDistribution>(lmbd - delta, lmbd + delta), make_shared<UniformDistribution>(
                        mu - delta, mu + delta)},
                {3, make_shared<UniformDistribution>(lmbd - delta, lmbd + delta), make_shared<UniformDistribution>(
                        mu - delta, mu + delta)},
        };
        auto p_net = PetriNetBuilder(type_info, rec_info, win_info).build();

        clock_t time = 0, start = 0, end = 0;
        for (int j = 0; j < repeat_num; ++j) {
            start = clock();

            auto delft = Delft(param, p_net);
            delft.run();

            end = clock();
            time += end - start;
        }
        time /= repeat_num;


        res[i++] = 1000.0 * (time) / CLOCKS_PER_SEC;

        lmbd -= step;
    }

    return res;
}

void measure_levels() {
    double mu = 5;
    double lmbd = mu * 10;
    double step = 0.1;

    double delta = 0.01;
    int mtime = 720;
    int delft_step = 1;
    int delft_mult = 1;

    vector<double> ro;
    vector<double> lmbd_vec;
    while (lmbd > mu) {
        lmbd_vec.push_back(lmbd);
        ro.push_back(mu / lmbd);
        lmbd -= step;
    }

    vector<vector<double>> output;
    output.push_back(ro);

    for (int i = 2; i < 11; i++) {
        auto param = DelftParam(delft_step, i, mtime);
        output.push_back(measure_ro(lmbd_vec, mu, delta, param));
        cout << i << endl;
    }

    ofstream file("res.csv", ios::out);

    for (int j = 0; j < output[0].size(); ++j) {
        for (int i = 0; i < output.size(); ++i) {
            file << format("{};", output[i][j]);
        }
        file << endl;
    }

    file.close();
}