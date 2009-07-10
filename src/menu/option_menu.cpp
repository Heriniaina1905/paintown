#include "menu/option_menu.h"
#include "menu/menu.h"
#include "menu/tab_menu.h"
#include "util/token.h"
#include "return_exception.h"
#include "util/token_exception.h"
#include "util/funcs.h"

OptionMenu::OptionMenu(Token *token) throw (LoadException): MenuOption(token, Event), _menu(0)
{
    // Check whether we have a menu or tabmenu
    if ( *token == "menu" )
    {
	_menu = new Menu();
    } else if (*token == "tabmenu"){
	_menu = new TabMenu();
    } else {
	throw LoadException("Not a menu");
    }
    // Set this menu as an option
    _menu->setAsOption(true);
    
    // Lets try loading from a file
    std::string temp;
    // Filename
    *token >> temp;
    if(temp=="name")
    {
	    token->resetToken();
	    _menu->load(token);
    }
    else _menu->load(Util::getDataPath() + temp);
    this->setText(_menu->getName());
    
    // Lets check if this menu is going bye bye
    if ( _menu->checkRemoval() ) setForRemoval(true);
}

OptionMenu::~OptionMenu()
{
	// Delete our menu
	if(_menu)delete _menu;
}

void OptionMenu::logic()
{
	// Nothing
}

void OptionMenu::run(bool &endGame)
{
	// Do our new menu
	_menu->run();
}

