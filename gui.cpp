#include "gui.hpp"

#include <SDL.h>

#include <sdlgui/screen.h>
#include <sdlgui/window.h>
#include <sdlgui/label.h>
#include <sdlgui/button.h>
#include <sdlgui/entypo.h>

Gui::Gui(std::shared_ptr<SDL_Window> window,
         const size_t width,
         const size_t height,
         const FileCallback loadMaze,
         const FileCallback saveMaze,
         const ButtonClickCallback newMaze,
         const SelectionCallback foreGround,
         const SelectionCallback backGround
         ):
    screen_(std::make_shared<sdlgui::Screen>(window.get(),
                                             sdlgui::Vector2i{static_cast<int>(width), static_cast<int>(height)},
                                             "MazeDraw")),
    loadMaze_(loadMaze),
    saveMaze_(saveMaze_),
    newMaze_(newMaze),
    foreGround_(foreGround),
    backGround_(backGround)
{
    auto& window1 = screen_->window("Sample Window", sdlgui::Vector2i{15,15}).withLayout<sdlgui::GroupLayout>();

    window1.label("File",  "sans-bold");
    window1.button("New",  ENTYPO_ICON_NEW, []{} );
    window1.button("Load", ENTYPO_ICON_UPLOAD, []{}  );
    window1.button("Save", ENTYPO_ICON_SAVE, []{});

    window1.label("Foreground", "sans-bold");
    auto& fg = window1.widget().boxlayout(sdlgui::Orientation::Horizontal, sdlgui::Alignment::Middle, 0, 6);
    fg.button(" ");
    fg.button("+").setPushed(true);
    fg.button("W");
    fg.button("S");
    fg.button("T");

    auto& bg = window1.widget().boxlayout(sdlgui::Orientation::Horizontal, sdlgui::Alignment::Middle, 0, 6);
    bg.button(" ").setPushed(true);
    bg.button("+");
    bg.button("W");
    bg.button("S");
    bg.button("T");

    screen_->performLayout(screen_->sdlRenderer());
}
