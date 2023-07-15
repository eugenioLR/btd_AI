#include <pybind11/pybind11.h>
#include "include/Game.hpp"

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}


PYBIND11_MODULE(pybtd_ai, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function that adds two numbers");
    
    py::class_<Bloon>(m, "Bloon")
        .def(py::init());

    // py::class_<Game>(m, "Game")
    //     .def(py::init())
    //     .def("perform_action", &Game::perform_action)
    //     .def("step", &Game::step)
    //     .def("get_gamestate", &Game::get_gamestate);
}