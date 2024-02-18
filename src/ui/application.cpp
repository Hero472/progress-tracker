#include "application.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>

Glib::RefPtr<ui::Application> ui::Application::create() {
    return Glib::RefPtr<ui::Application>(new Application());
}

ui::Application::Application()
    : Gtk::Application{"com.moura.Progress"} {}

void ui::Application::on_startup() {
    Gtk::Application::on_startup();

    auto window_builder = Gtk::Builder::create_from_resource("/ui/app-window.ui");
    main_window = Gtk::Builder::get_widget_derived<ui::ProgressWindow>(
        window_builder, "app-window");
    if (!main_window) {
        exit(1);
    }

    std::string app_dir{std::getenv("HOME")};
    app_dir += +"/.config/progress/boards";

    if (!std::filesystem::exists(app_dir)) {
        if (!std::filesystem::create_directories(app_dir)) {
            std::cerr
                << "\033[;32mIt was not possible to create a directory\033[m"
                << std::endl;
        }
    } else {
        // Load all boards information onto memory
        for (const auto& dir_entry :
             std::filesystem::directory_iterator(app_dir)) {
            std::string board_filename = dir_entry.path();
            if (board_filename.ends_with(".xml")) {
                main_window->get_window_controller().add_board(board_filename);
            }
        }
    }

    add_window(*main_window);
}

void ui::Application::on_activate() {
    Gtk::Application::on_activate();
    main_window->set_visible();
}