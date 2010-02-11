#ifndef mugen_fadetool_h
#define mugen_fadetool_h

/*! FADE UTILITY */

class Bitmap;

namespace Mugen{

class FadeTool{
public:
	FadeTool();
	virtual ~FadeTool();

        enum State{
            FadeIn,
            FadeOut,
            NoFade,
            RunFade,
        };
	
	// Fade state
	void setState(const State &);
	
	// update
	void act();
	
	// Fade to whatever state it is at
	virtual void draw(const Bitmap &);
	
	inline const State & getState() const { 
            return currentState; 
        }
	inline void setFadeInTime(const int time){ 
            fadeInTime = time; 
        }
	inline void setFadeOutTime(const int time){ 
            fadeOutTime = time; 
        }
	inline int getFadeInTime() const { 
            return fadeInTime; 
        }
	inline int getFadeOutTime() const { 
            return fadeOutTime; 
        }
	inline void setFadeInColor( int c ){ 
            fadeInColor = c; 
        }
	inline int getFadeInColor() const { 
            return fadeInColor; 
        }
	inline void setFadeOutColor( int c ){ 
            fadeInColor = c; 
        }
	inline int getFadeOutColor() const { 
            return fadeInColor; 
        }
private:
	
	State currentState;
	State lastState;
	int fader;
	int fadeInTime;
	int fadeOutTime;
	int fadeInColor;
	int fadeOutColor;
	
};

}

#endif
