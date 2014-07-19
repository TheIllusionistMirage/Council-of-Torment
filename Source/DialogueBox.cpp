#include "DialogueBox.h"
#include "Stdafx.h"

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 18 July 2014
* Description: Constructor for a Dialogue Box
* ----------------------------------------------------------------------
*/
	DialogueBox::DialogueBox(sf::String t) 
	: fontSize(12)
	, sidePadding(20)
	, topPadding(5)
	, rectColor(0,0,0,100)
	, textColor(255,255,255)
	{
		font.loadFromFile("Content/Fonts/calibri.ttf");

		text.setCharacterSize(fontSize);
		text.setColor(textColor);
		text.setFont(font);
		text.setString(t);
	
		centerOrigin(text);
		centerOrigin(rect);

		int width = fontSize * text.getString().getSize() + sidePadding * 2;
		int height = fontSize + topPadding * 2;

		rect.setFillColor(rectColor);
		rect.setSize(sf::Vector2f(width, height));
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 18 July 2014
* Description: Destructor for a Dialogue Box
* ----------------------------------------------------------------------
*/
	DialogueBox::~DialogueBox()
	{
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 18 July 2014
* Description: Sets the position of the whole element
* ----------------------------------------------------------------------
*/
	void DialogueBox::setPosition(sf::Vector2f p)
	{
		pos = p;

		text.setPosition(pos);
		rect.setPosition(pos);
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 18 July 2014
* Description: Updates the size of the element
* ----------------------------------------------------------------------
*/
	void DialogueBox::updateDimensions()
	{
		int width = fontSize * text.getString().getSize() + sidePadding * 2;
		int height = fontSize + topPadding * 2;

		rect.setSize(sf::Vector2f(width, height));
	}