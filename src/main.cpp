#include "mainwindow.h"

#include <QApplication>

#include <iostream>
#include <ctime>
#include <fstream>

using namespace std;

void measure_levels();

void compare();

void measure_time();

void compare_time();

int main(int argc, char *argv[]) {
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();

    compare();

//    compare_time();

//    measure_levels();

//    measure_time();

//    vector<TypeInfo> type_info = {
//            {1, make_shared<UniformDistribution>(5, 10), make_shared<UniformDistribution>(5, 15)},
//            {2, make_shared<UniformDistribution>(5, 10), make_shared<UniformDistribution>(5, 15)},
//            {3, make_shared<UniformDistribution>(5, 10), make_shared<UniformDistribution>(5, 15)},
//    };
//    ReceptionInfo rec_info = {make_shared<UniformDistribution>(5, 15), 10};
//    vector<WinGroupInfo> win_info = {
//            {1, {1}, 10},
//            {1, {2}, 10},
//            {1, {3}, 10},
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

void compare() {
    double mu = 15;
    double lmbd = 15.0 * 10 / 9;
    double delta = 0.1;
    int mtime = 20000000;

    int delft_step = 10;
    int delft_mult = 4;

    int repeat_num = 1;

    auto param = DelftParam(delft_step, delft_mult, mtime);
    vector<TypeInfo> type_info = {
            {1, make_shared<UniformDistribution>(lmbd - delta, lmbd + delta), make_shared<UniformDistribution>(
                    mu * 3 - delta, mu * 3 + delta)},
            {2, make_shared<UniformDistribution>(lmbd - delta, lmbd + delta), make_shared<UniformDistribution>(
                    mu * 3 - delta, mu * 3 + delta)},
            {3, make_shared<UniformDistribution>(lmbd - delta, lmbd + delta), make_shared<UniformDistribution>(
                    mu * 3 - delta, mu * 3 + delta)},
    };
    ReceptionInfo rec_info = {make_shared<UniformDistribution>(mu / 3 - delta, mu / 3 + delta),
                              10};
    vector<WinGroupInfo> win_info = {
            {3, {1}, 10},
            {3, {2}, 10},
            {3, {3}, 10},
    };


    clock_t time = 0, start = 0, end = 0;
    for (int j = 0; j < repeat_num; ++j) {
        start = clock();

        auto tmp_builder = PetriNetBuilder(type_info, rec_info, win_info);
        auto petriNets = tmp_builder.build();
        auto src = tmp_builder.get_stat_src();

        auto delft = Delft(param, petriNets);
        delft.run();

        auto stats = petriNets->get_stats();
        auto smoStats = StatsAggregator(mtime, src, stats.first, stats.second).get_stats();

        end = clock();
        time += end - start;
        cout << format("{} {}", j, end - start) << endl;
    }
    time /= repeat_num;

    cout << 1000.0 * time / CLOCKS_PER_SEC << endl;
}

vector<double> measure_ro(vector<double> lmbd_vec, double mu, double delta, DelftParam param) {
    int repeat_num = 20;

    ReceptionInfo rec_info = {make_shared<UniformDistribution>(mu / 3 - delta, mu / 3 + delta),
                              10};
    vector<WinGroupInfo> win_info = {
            {3, {1}, 10},
            {3, {2}, 10},
            {3, {3}, 10},
    };

    vector<double> res(lmbd_vec.size());
    int i = 0;
    for (auto &lmbd: lmbd_vec) {
        vector<TypeInfo> type_info = {
                {1, make_shared<UniformDistribution>(lmbd - delta, lmbd + delta), make_shared<UniformDistribution>(
                        mu * 3 - delta, mu * 3 + delta)},
                {2, make_shared<UniformDistribution>(lmbd - delta, lmbd + delta), make_shared<UniformDistribution>(
                        mu * 3 - delta, mu * 3 + delta)},
                {3, make_shared<UniformDistribution>(lmbd - delta, lmbd + delta), make_shared<UniformDistribution>(
                        mu * 3 - delta, mu * 3 + delta)},
        };

        clock_t time = 0, start = 0, end = 0;
        for (int j = 0; j < repeat_num; ++j) {

            auto p_net = PetriNetBuilder(type_info, rec_info, win_info).build();
            start = clock();

            auto delft = Delft(param, p_net);
            delft.run();

            end = clock();
            time += end - start;
        }
        time /= repeat_num;


        res[i++] = 1000.0 * (time) / CLOCKS_PER_SEC;
    }

    return res;
}

void measure_levels() {
    double mu = 15;
    double lmbd = mu * 10;
    double step = 0.1;

    double delta = 0.1;
    int mtime = 2000;
    int delft_step = 1;
    int delft_mult = 5;

    vector<double> ro;
    vector<double> lmbd_vec;
    while (lmbd > mu - 7.5) {
        lmbd_vec.push_back(lmbd);
        ro.push_back(mu / lmbd);
        lmbd -= step;
    }

    auto param = DelftParam(delft_step, delft_mult, mtime);
    auto res = measure_ro(lmbd_vec, mu, delta, param);

    ofstream file("res_new.csv", ios::out);
    for (int i = 0; i < res.size(); ++i) {
        file << format("{};{}", ro[i], res[i]) << endl;
    }

    file.close();

//    vector<vector<double>> output;
//    output.push_back(ro);
//
//    for (int i = 1; i < 6; i++) {
//        auto param = DelftParam(i, delft_mult, mtime);
//        output.push_back(measure_ro(lmbd_vec, mu, delta, param));
//        cout << i << endl;
//    }
//
//    ofstream file("res_new.csv", ios::out);
//
//    for (int j = 0; j < output[0].size(); ++j) {
//        for (int i = 0; i < output.size(); ++i) {
//            file << format("{};", output[i][j]);
//        }
//        file << endl;
//    }
//
//    file.close();
}

void measure_time() {
    double mu = 15;
    double lmbd = mu / 2;
    double delta = 1;

    int mtime_start = 100;
    int mtime_end = 5000;
    int step = 5;
    int repeat_num = 10;

    int delft_step = 1;
    int delft_mult = 5;


    vector<TypeInfo> type_info = {
            {1, make_shared<UniformDistribution>(lmbd - delta, lmbd + delta), make_shared<UniformDistribution>(
                    mu * 3 - delta, mu * 3 + delta)},
            {2, make_shared<UniformDistribution>(lmbd - delta, lmbd + delta), make_shared<UniformDistribution>(
                    mu * 3 - delta, mu * 3 + delta)},
            {3, make_shared<UniformDistribution>(lmbd - delta, lmbd + delta), make_shared<UniformDistribution>(
                    mu * 3 - delta, mu * 3 + delta)},
    };
    ReceptionInfo rec_info = {make_shared<UniformDistribution>(mu / 3 - delta, mu / 3 + delta),
                              10};
    vector<WinGroupInfo> win_info = {
            {3, {1}, 10},
            {3, {2}, 10},
            {3, {3}, 10},
    };

    ofstream file("time20.csv", ios::out);
    for (int i = mtime_start; i < mtime_end; i += step) {
        auto param = DelftParam(delft_step, delft_mult, i);

        clock_t time = 0, start = 0, end = 0;
        for (int j = 0; j < repeat_num; ++j) {

            auto p_net = PetriNetBuilder(type_info, rec_info, win_info).build();
            start = clock();

            auto delft = Delft(param, p_net);
            delft.run();

            end = clock();
            time += end - start;
        }
        time /= repeat_num;

        file << format("{};{}", i, 1000.0 * (time) / CLOCKS_PER_SEC) << endl;
        cout << i << endl;
    }

    file.close();
}


void compare_time() {
    double mu = 15;
    double lmbd = mu * 2;
    double delta = 0.1;

    int mtime_start = 10000000;
    int mtime_end = 20000000;
    int step = 1000000;
    int repeat_num = 1;

    int delft_step = 5;
    int delft_mult = 10;


    vector<TypeInfo> type_info = {
            {1, make_shared<UniformDistribution>(lmbd - delta, lmbd + delta), make_shared<UniformDistribution>(
                    mu * 3 - delta, mu * 3 + delta)},
            {2, make_shared<UniformDistribution>(lmbd - delta, lmbd + delta), make_shared<UniformDistribution>(
                    mu * 3 - delta, mu * 3 + delta)},
            {3, make_shared<UniformDistribution>(lmbd - delta, lmbd + delta), make_shared<UniformDistribution>(
                    mu * 3 - delta, mu * 3 + delta)},
    };
    ReceptionInfo rec_info = {make_shared<UniformDistribution>(mu / 3 - delta, mu / 3 + delta),
                              10};
    vector<WinGroupInfo> win_info = {
            {3, {1}, 10},
            {3, {2}, 10},
            {3, {3}, 10},
    };

    ofstream file("time_cmp.csv", ios::out);
    for (int i = mtime_start; i < mtime_end; i += step) {
        auto param = DelftParam(delft_step, delft_mult, i);

        clock_t time = 0, start = 0, end = 0;
        for (int j = 0; j < repeat_num; ++j) {
            start = clock();

            auto tmp_builder = PetriNetBuilder(type_info, rec_info, win_info);
            auto p_net = tmp_builder.build();
            auto src = tmp_builder.get_stat_src();

            auto delft = Delft(param, p_net);
            delft.run();

            auto stats = p_net->get_stats();
            auto smoStats = StatsAggregator(i, src, stats.first, stats.second).get_stats();

            end = clock();
            time += end - start;
        }
        time /= repeat_num;

        file << format("{};{}", i, 1000.0 * (time) / CLOCKS_PER_SEC) << endl;
        cout << i << endl;
    }

    file.close();
}