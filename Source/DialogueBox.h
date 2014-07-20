#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "State.h"

/*	A dialogue box is a graphical element
	that contains an NPC/s line or a player
	given response. It can be resized to fit
	other purposes too.
*/
class DialogueBox
{
	public:
		DialogueBox() : context(context) {}
		DialogueBox(State::Context context) : context(context) {}
		DialogueBox(State::Context context, const std::string t);

		void setSidePadding(int p)			{ sidePadding = p;							}	
		void setTopPadding(int p)			{ topPadding = p;							}
		void setFontSize(int x)				{ fontSize = x; text.setCharacterSize(x);	}
		void setRectColor(sf::Color c)		{ rectColor = c; rect.setFillColor(c);		}
		void setTextColor(sf::Color c)		{ textColor = c; text.setColor(c);			}
		void setExecution(std::string s)	{ execute = s;							    }			
		
		int getSidePadding()				{ return sidePadding;	}
		int getTopPadding()					{ return topPadding;	}
		int getFontSize()					{ return fontSize;		}
		sf::Vector2f& getPosition()			{ return pos;			}	
		sf::Color& getRectColor()			{ return rectColor;		}
		sf::Color& getTextColor()			{ return textColor;		}
		sf::RectangleShape& getRect()		{ return rect;			}	
		sf::Text& getText()					{ return text;			}	
		std::string getExecute()			{ return execute;		}	

		void handleEvents(const sf::Event& e);
		void setText(std::string t, int size);
		void setPosition(sf::Vector2f p);
		void setText(std::string t);
		void render();

		int id;

	private:
		State::Context context;

		int fontSize;
		int sidePadding, topPadding;

		sf::Vector2f pos;
		sf::Color rectColor, textColor;
		sf::RectangleShape rect;
		sf::Text text;
		sf::Font font;

		std::string execute;
};

