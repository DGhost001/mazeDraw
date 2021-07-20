#include "gui.hpp"

#include <SDL.h>

#include <sdlgui/screen.h>
#include <sdlgui/window.h>
#include <sdlgui/label.h>
#include <sdlgui/button.h>
#include <sdlgui/entypo.h>
#include <sdlgui/popupbutton.h>
#include <sdlgui/popup.h>
#include <sdlgui/messagedialog.h>
#include <sdlgui/common.h>

class NullWidget: public sdlgui::Widget
{
public:
    NullWidget():Widget(nullptr) {}
    virtual ~NullWidget() {}
};

static NullWidget nullWidget;

Gui::Gui(std::shared_ptr<SDL_Window> window,
         const size_t width,
         const size_t height,
         const FileCallback loadMaze,
         const FileCallback saveMaze,
         const ButtonClickCallback newMaze,
         const SelectionCallback foreGround,
         const SelectionCallback backGround,
         const ButtonClickCallback quit
         ):
    screen_(std::make_shared<sdlgui::Screen>(window.get(),
                                             sdlgui::Vector2i{static_cast<int>(width), static_cast<int>(height)},
                                             "MazeDraw")),
    foreGroundWidget_(std::ref(nullWidget)),
    backGroundWidget_(std::ref(nullWidget)),
    loadMaze_(loadMaze),
    saveMaze_(saveMaze),
    newMaze_(newMaze),
    foreGround_(foreGround),
    backGround_(backGround),
    quit_(quit)
{
    auto& window1 = screen_->window("Tool Window", sdlgui::Vector2i{15,15}).withLayout<sdlgui::GroupLayout>();

    screen_->theme()->mButtonFontSize = 16;

    auto& fileMenu = window1.popupbutton("File",ENTYPO_ICON_EXPORT);

    auto& filePopup= fileMenu.popup().withLayout<sdlgui::GroupLayout>();
    filePopup.button("New",  ENTYPO_ICON_NEW, [this]{ onNewClicked(); } );
    filePopup.button("Load", ENTYPO_ICON_UPLOAD, [this]{ onOpenClicked(); } );
    filePopup.button("Save", ENTYPO_ICON_SAVE, [this]{ onSaveClicked(); });

    auto& foreGroundMenu  = window1.popupbutton("Foreground",ENTYPO_ICON_EXPORT);

    auto& foreGroundPopup = foreGroundMenu.popup().withLayout<sdlgui::GroupLayout>();

    foreGroundPopup.button(" ", [this]{ onSelectForeground(0); }).setFlags(sdlgui::Button::Flags::RadioButton);
    foreGroundPopup.button("+", [this]{ onSelectForeground(1); }).setFlags(sdlgui::Button::Flags::RadioButton);
    foreGroundPopup.button("W", [this]{ onSelectForeground(2); }).setFlags(sdlgui::Button::Flags::RadioButton);
    foreGroundPopup.button("S", [this]{ onSelectForeground(3); }).setFlags(sdlgui::Button::Flags::RadioButton);
    foreGroundPopup.button("T", [this]{ onSelectForeground(4); }).setFlags(sdlgui::Button::Flags::RadioButton);

    auto& backGroundMenu  = window1.popupbutton("Background",ENTYPO_ICON_EXPORT);
    auto& backGroundPopup = backGroundMenu.popup().withLayout<sdlgui::GroupLayout>();

    backGroundPopup.button(" ", [this]{ onSelectBackground(0); }).setFlags(sdlgui::Button::Flags::RadioButton);
    backGroundPopup.button("+", [this]{ onSelectBackground(1); }).setFlags(sdlgui::Button::Flags::RadioButton);
    backGroundPopup.button("W", [this]{ onSelectBackground(2); }).setFlags(sdlgui::Button::Flags::RadioButton);
    backGroundPopup.button("S", [this]{ onSelectBackground(3); }).setFlags(sdlgui::Button::Flags::RadioButton);
    backGroundPopup.button("T", [this]{ onSelectBackground(4); }).setFlags(sdlgui::Button::Flags::RadioButton);

    window1.button("Exit", ENTYPO_ICON_LOGOUT, quit_);

    screen_->performLayout(screen_->sdlRenderer());

}

void Gui::onSelectForeground(unsigned select)
{
    foreGround_(select);
}

void Gui::onSelectBackground(unsigned select)
{
    backGround_(select);
}

void Gui::onNewClicked( void )
{
    screen_->msgdialog(sdlgui::MessageDialog::Type::Warning, "Create new Maze", "Lose all changes?",
              "Yes", "No", true, [this](int result) { if(!result) { newMaze_(); }  });
}

void Gui::onOpenClicked( void )
{
    screen_->msgdialog(sdlgui::MessageDialog::Type::Warning, "Open Maze", "Lose all changes?",
              "Yes", "No", true, [this](int result) {
        if(!result) {
            std::string s = sdlgui::file_dialog({ {"maze", "Maze File"}}, false);
            loadMaze_(s);
        }
    });
}

void Gui::onSaveClicked( void )
{
    std::string s = sdlgui::file_dialog({ {"maze", "Maze File"}}, true);

    if(!s.ends_with(".maze")) {
        s += ".maze";
    }

    saveMaze_(s);
}
