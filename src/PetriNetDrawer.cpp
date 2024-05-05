#include "PetriNetDrawer.h"

#include <drag/drag.hpp>
#include <drag/drawing/draw.hpp>
#include <format>
#include <string>


void PetriNetDrawer::draw(const string &filename) {
    drag::graph graph;
    drag::drawing_options opts;

    for (int i = 0; i < data.t_num; ++i) {
        auto v = graph.add_node();
        string label = format("t{}", v);

        if (data.gen_t.contains(v)) label += format(" (g={})", data.gen_t[v]);
        if (data.selector_t.contains(v)) {
            string add = "(s=";
            for (auto &it: data.selector_t[v]) add += format("{},", it);
            add += ")";
            label += add;
        }
        if (data.win_poc.contains(v)) label += " (p)";

        opts.labels[v] = label;
        opts.colors[v] = "red";
    }

    for (int i = 0; i < data.p_num; ++i) {
        auto v = graph.add_node();
        string label = format("p{}", v - data.t_num);

        if (data.q_info.contains(v - data.t_num)) label += format(" (q={})", data.q_info[v - data.t_num]);
        opts.labels[v] = label;
    }

    for (auto &arc: data.t_to_p_arc) {
        graph.add_edge(arc.first, arc.second + data.t_num);
    }

    for (auto &arc: data.p_to_t_arc) {
        graph.add_edge(arc.arc.first + data.t_num, arc.arc.second);
        if (arc.is_ing) opts.edge_colors[{arc.arc.first + data.t_num, arc.arc.second}] = "blue";
    }

    graph.node_dist = 40;
    opts.margin = 20;
    auto image = drag::draw_svg_image(graph, opts);
    image.save(filename);
}
