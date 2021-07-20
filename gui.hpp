#ifndef GUI_H
#define GUI_H

#include <memory>
#include <functional>

namespace sdlgui {
   class Screen;
   class Widget;
}

struct SDL_Window;

class Gui
{
public:

    typedef std::function< void (const std::string &) > FileCallback;
    typedef std::function< void ( void )> ButtonClickCallback;
    typedef std::function< void ( unsigned ) > SelectionCallback;

    Gui(std::shared_ptr<SDL_Window> window,
        size_t const width,
        size_t const height,
        FileCallback const loadMaze,
        FileCallback const saveMaze,
        ButtonClickCallback const newMaze,
        SelectionCallback const foreGround,
        SelectionCallback const backGround,
        ButtonClickCallback const quit
        );

    std::shared_ptr<sdlgui::Screen> getScreen( void ) const { return screen_; }
private:

    std::shared_ptr<sdlgui::Screen> screen_;
    std::reference_wrapper<sdlgui::Widget> foreGroundWidget_;
    std::reference_wrapper<sdlgui::Widget> backGroundWidget_;

    FileCallback loadMaze_;
    FileCallback saveMaze_;
    ButtonClickCallback newMaze_;
    SelectionCallback foreGround_;
    SelectionCallback backGround_;
    ButtonClickCallback quit_;



    void onSelectForeground(unsigned);
    void onSelectBackground(unsigned);
    void onNewClicked( void );
    void onOpenClicked( void );
    void onSaveClicked( void );

};

#endif // GUI_H
