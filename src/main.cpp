#include "mainwindow.h"

#include <QApplication>

using namespace std;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

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
