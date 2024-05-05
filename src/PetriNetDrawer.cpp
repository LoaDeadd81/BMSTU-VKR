#include "PetriNetDrawer.h"

#include <drag/drag.hpp>
#include <drag/drawing/draw.hpp>
#include <drag/types.hpp>
#include <format>


void PetriNetDrawer::draw(const string &filename) {
    drag::graph graph;
    drag::drawing_options opts;

    for (int i = 0; i < data.t_num; ++i) {
        auto v = graph.add_node();
        opts.labels[v] = format("t{}", v);
        opts.colors[v] = "red";
    }

    for (int i = 0; i < data.p_num; ++i) {
        auto v = graph.add_node();
        opts.labels[v] = format("p{}", v - data.t_num);
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
