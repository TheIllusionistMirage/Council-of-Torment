#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

/*	A dialogue box is a graphical element
	that contains an NPC/s line or a player
	given response. It can be resized to fit
	other purposes too.
*/
class DialogueBox
{
	public:
		DialogueBox() {};
		DialogueBox(sf::String t);
		~DialogueBox();

		void setSidePadding(int p)			{ sidePadding = p;		}	
		void setTopPadding(int p)			{ topPadding = p;		}
		void setFontSize(int x)				{ fontSize = x;			}
		void setRectColor(sf::Color c)		{ rectColor = c;		}	
		void setTextColor(sf::Color c)		{ textColor = c;		}
		void setText(std::string t)			{ text.setString(t); updateDimensions(); }
		
		int getSidePadding()				{ return sidePadding;	}
		int getTopPadding()					{ return topPadding;	}
		int getFontSize()					{ return fontSize;		}
		sf::Vector2f& getPosition()			{ return pos;			}	
		sf::Color& getRectColor()			{ return rectColor;		}
		sf::Color& getTextColor()			{ return textColor;		}
		sf::RectangleShape& getRect()		{ return rect;			}	
		sf::Text& getText()					{ return text;			}	

		void setPosition(sf::Vector2f p);
		void updateDimensions();

	private:
		int fontSize;
		int sidePadding, topPadding;

		sf::Vector2f pos;
		sf::Color rectColor, textColor;
		sf::RectangleShape rect;
		sf::Text text;
		sf::Font font;
};

