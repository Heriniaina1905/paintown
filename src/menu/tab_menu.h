#ifndef _paintown_tab_menu_h
#define _paintown_tab_menu_h

#include "menu.h"
#include "gui/widget.h"
#include "gui/box.h"
#include "gui/popup-box.h"
#include "gui/context-box.h"
#include "input/input-map.h"

class Font;
class Bitmap;
class MenuOption;
class Token;
class MenuAnimation;
class ColorInfo;

// Provides a buffer for going between two colors
/* This is probably just Util::Gradient */
class ColorBuffer{
    public:
	ColorBuffer(int color1, int color2);
	virtual ~ColorBuffer();

    static const int maxColors = 100;
	int colors[maxColors];
	int index;
	bool forward;
	
	int update();
	
	void reset();
};

// Provide menus in a box
class MenuBox : public Gui::Box{
    public:
	MenuBox(int w, int h);
	virtual ~MenuBox();
	bool checkVisible(const Gui::Coordinate &);
	void setColors(const Gui::ColorInfo &, const int fontColor);
	void setColors(const int bodyColor, const int borderColor, const int fontColor);
	Menu menu;
	int fontColor;
        //Gui::Box snapPosition;
        Gui::ContextBox context;
	bool running;
};

class TabMenu : public Menu
{
    public:
	TabMenu();
	virtual ~TabMenu();
	
	void load(const Filesystem::AbsolutePath &filename) throw (LoadException);
	void load(Token *token) throw (LoadException);
	
	/*! do logic, draw whatever */
	void run();
    private:
        Gui::Coordinate tabInfo;
        Gui::ColorInfo tabColors;
        Gui::Coordinate selectedTabInfo;
        Gui::ColorInfo selectedTabColors;
        Gui::Coordinate runningTabInfo;
        Gui::ColorInfo runningTabColors;
	
	int fontColor;
	int selectedFontColor;
	int runningFontColor;
	
	std::string runningInfo;
	
	std::vector<MenuBox *>tabs;
	std::vector<MenuBox *>::iterator currentTab;
	int location;
	// Our actual targetOffset
	double targetOffset;
	// Actual offset
	double totalOffset;
	// double scrollSpeed;
	// Total lines from options
	int totalLines;

        void drawMenus(Bitmap *bmp);
	
	void calculateTabLines();
        
        Gui::PopupBox contentArea;
};
#endif
