#include <allegro.h>
#ifdef ALLEGRO_WINDOWS
#include <winalleg.h>
#endif

#ifndef ALLEGRO_WINDOWS
#include <signal.h>
#include <string.h>
#endif

#include "globals.h"
#include "init.h"
#include <pthread.h>
#include "network/network.h"

#include <ostream>
#include "dumb/include/dumb.h"
#include "dumb/include/aldumb.h"
#include "loadpng/loadpng.h"
#include "util/bitmap.h"
#include "configuration.h"

using namespace std;

volatile int Global::speed_counter = 0;
volatile int Global::second_counter = 0;

/* the original engine was running at 90 ticks per second, but we dont
 * need to render that fast, so TICS_PER_SECOND is really fps and
 * LOGIC_MULTIPLIER will be used to adjust the speed counter to its
 * original value.
 */
static const int TICS_PER_SECOND = 40;
const double Global::LOGIC_MULTIPLIER = (double) 90 / (double) TICS_PER_SECOND;
        

pthread_mutex_t Global::loading_screen_mutex;
bool Global::done_loading = false;

const int Global::WINDOWED = GFX_AUTODETECT_WINDOWED;
const int Global::FULLSCREEN = GFX_AUTODETECT_FULLSCREEN;

void inc_speed_counter() {
	Global::speed_counter += 1;
}
END_OF_FUNCTION( inc_speed_counter );

void inc_second_counter() {
	Global::second_counter += 1;
}
END_OF_FUNCTION( inc_second_counter );

#ifndef ALLEGRO_WINDOWS
static void handleSigPipe( int i, siginfo_t * sig, void * data ){
}

/*
static void handleSigUsr1( int i, siginfo_t * sig, void * data ){
	pthread_exit( NULL );
}
*/
#endif

static void registerSignals(){
#ifndef ALLEGRO_WINDOWS
	struct sigaction action;
	memset( &action, 0, sizeof(struct sigaction) );
	action.sa_sigaction = handleSigPipe;
	sigaction( SIGPIPE, &action, NULL );
	/*
	action.sa_sigaction = handleSigUsr1;
	sigaction( SIGUSR1, &action, NULL );
	*/
#endif
}

bool Global::init( int gfx ){

	ostream & out = Global::debug( 0 );
	out << "-- BEGIN init --" << endl;
	out << "Allegro version: " << ALLEGRO_VERSION_STR << endl;
	out <<"Allegro init: "<<allegro_init()<<endl;
	out <<"Install timer: "<<install_timer()<<endl;
	
	set_volume_per_voice( 0 );
	out<<"Install sound: "<<install_sound( DIGI_AUTODETECT, MIDI_NONE, "" )<<endl;
	
	loadpng_init();
        
        Bitmap::SCALE_X = GFX_X;
        Bitmap::SCALE_Y = GFX_Y;

        const int sx = 640;
        const int sy = 480;

	out<<"Install keyboard: "<<install_keyboard()<<endl;
	out<<"Install mouse: "<<install_mouse()<<endl;
	set_color_depth( 16 );
	out<<"Set gfx mode: " << Bitmap::setGraphicsMode( gfx, sx, sy ) <<endl;

	LOCK_VARIABLE( speed_counter );
	LOCK_VARIABLE( second_counter );
	LOCK_FUNCTION( (void *)inc_speed_counter );
	LOCK_FUNCTION( (void *)inc_second_counter );
	out<<"Install game timer: "<<install_int_ex( inc_speed_counter, BPS_TO_TIMER( TICS_PER_SECOND ) )<<endl;
	out<<"Install second timer: "<<install_int_ex( inc_second_counter, BPS_TO_TIMER( 1 ) )<<endl;
	srand( time( NULL ) );
	set_display_switch_mode( SWITCH_BACKGROUND );
	
	atexit( &dumb_exit );
	atexit( Network::closeAll );
	dumb_register_packfiles();

	registerSignals();
	
	Network::init();

	Configuration::loadConfigurations();
	
	pthread_mutex_init( &Global::loading_screen_mutex, NULL );
	
	out<<"-- END init --"<<endl;

	/*
	Bitmap::Screen = new Bitmap( screen );
	if ( ! Bitmap::Screen ){
		return false;
	}
	*/
	return true;
}
