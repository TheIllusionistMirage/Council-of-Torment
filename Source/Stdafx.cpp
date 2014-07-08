#include "Stdafx.h"
#include "Game.h"

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Returns the string of a sf::Keyboard::Key
 * ----------------------------------------------------------------------
 */
std::string toString(sf::Keyboard::Key key)
{
	#define KEY_TO_STRING(KEY) case sf::Keyboard::KEY: return #KEY;

	switch(key)
	{
		KEY_TO_STRING(Unknown)
		KEY_TO_STRING(A)
		KEY_TO_STRING(B)
		KEY_TO_STRING(C)
		KEY_TO_STRING(D)
		KEY_TO_STRING(E)
		KEY_TO_STRING(F)
		KEY_TO_STRING(G)
		KEY_TO_STRING(H)
		KEY_TO_STRING(I)
		KEY_TO_STRING(J)
		KEY_TO_STRING(K)
		KEY_TO_STRING(L)
		KEY_TO_STRING(M)
		KEY_TO_STRING(N)
		KEY_TO_STRING(O)
		KEY_TO_STRING(P)
		KEY_TO_STRING(Q)
		KEY_TO_STRING(R)
		KEY_TO_STRING(S)
		KEY_TO_STRING(T)
		KEY_TO_STRING(U)
		KEY_TO_STRING(V)
		KEY_TO_STRING(W)
		KEY_TO_STRING(X)
		KEY_TO_STRING(Y)
		KEY_TO_STRING(Z)
		KEY_TO_STRING(Num0)
		KEY_TO_STRING(Num1)
		KEY_TO_STRING(Num2)
		KEY_TO_STRING(Num3)
		KEY_TO_STRING(Num4)
		KEY_TO_STRING(Num5)
		KEY_TO_STRING(Num6)
		KEY_TO_STRING(Num7)
		KEY_TO_STRING(Num8)
		KEY_TO_STRING(Num9)
		KEY_TO_STRING(Escape)
		KEY_TO_STRING(LControl)
		KEY_TO_STRING(LShift)
		KEY_TO_STRING(LAlt)
		KEY_TO_STRING(LSystem)
		KEY_TO_STRING(RControl)
		KEY_TO_STRING(RShift)
		KEY_TO_STRING(RAlt)
		KEY_TO_STRING(RSystem)
		KEY_TO_STRING(Menu)
		KEY_TO_STRING(LBracket)
		KEY_TO_STRING(RBracket)
		KEY_TO_STRING(SemiColon)
		KEY_TO_STRING(Comma)
		KEY_TO_STRING(Period)
		KEY_TO_STRING(Quote)
		KEY_TO_STRING(Slash)
		KEY_TO_STRING(BackSlash)
		KEY_TO_STRING(Tilde)
		KEY_TO_STRING(Equal)
		KEY_TO_STRING(Dash)
		KEY_TO_STRING(Space)
		KEY_TO_STRING(Return)
		KEY_TO_STRING(BackSpace)
		KEY_TO_STRING(Tab)
		KEY_TO_STRING(PageUp)
		KEY_TO_STRING(PageDown)
		KEY_TO_STRING(End)
		KEY_TO_STRING(Home)
		KEY_TO_STRING(Insert)
		KEY_TO_STRING(Delete)
		KEY_TO_STRING(Add)
		KEY_TO_STRING(Subtract)
		KEY_TO_STRING(Multiply)
		KEY_TO_STRING(Divide)
		KEY_TO_STRING(Left)
		KEY_TO_STRING(Right)
		KEY_TO_STRING(Up)
		KEY_TO_STRING(Down)
		KEY_TO_STRING(Numpad0)
		KEY_TO_STRING(Numpad1)
		KEY_TO_STRING(Numpad2)
		KEY_TO_STRING(Numpad3)
		KEY_TO_STRING(Numpad4)
		KEY_TO_STRING(Numpad5)
		KEY_TO_STRING(Numpad6)
		KEY_TO_STRING(Numpad7)
		KEY_TO_STRING(Numpad8)
		KEY_TO_STRING(Numpad9)
		KEY_TO_STRING(F1)
		KEY_TO_STRING(F2)
		KEY_TO_STRING(F3)
		KEY_TO_STRING(F4)
		KEY_TO_STRING(F5)
		KEY_TO_STRING(F6)
		KEY_TO_STRING(F7)
		KEY_TO_STRING(F8)
		KEY_TO_STRING(F9)
		KEY_TO_STRING(F10)
		KEY_TO_STRING(F11)
		KEY_TO_STRING(F12)
		KEY_TO_STRING(F13)
		KEY_TO_STRING(F14)
		KEY_TO_STRING(F15)
		KEY_TO_STRING(Pause)
	}

	return "";
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 1 April 2014
 * Description: Returns the category of an item
 * ----------------------------------------------------------------------
 */
int toCategory(std::string string)
{
	if(string == "FOOD") return FOOD;
	else if(string == "EQUIPMENT") return EQUIPMENT;
	else if(string == "BOOKS") return BOOKS;
	else if(string == "MATERIALS") return MATERIALS;
	else return ALL;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 31 January 2014
 * Description: Centers the origin of a circle shape
 * ----------------------------------------------------------------------
 */
void centerOrigin(sf::CircleShape& circleShape)
{
	// Center the origin of a circle shape
	sf::FloatRect bounds = circleShape.getLocalBounds();
	circleShape.setOrigin(float(int(bounds.width / 2.0f)), float(int(bounds.height / 2.0f)));
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Centers the origin of a sprite
 * ----------------------------------------------------------------------
 */
void centerOrigin(sf::Sprite& sprite)
{
	// Center the origin of a sprite
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(float(int(bounds.width / 2.0f)), float(int(bounds.height / 2.0f)));
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 19 January 2014
 * Description: Centers the origin of a text
 * ----------------------------------------------------------------------
 */
void centerOrigin(sf::Text& text)
{
	// Center the origin of a text
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(float(int(bounds.width / 2.0f)), float(int(bounds.height / 2.0f)));
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 1 February
 * Description: Calculates the dot product of two vectors
 * ----------------------------------------------------------------------
 */
float dotProduct(const sf::Vector2f& vector1, const sf::Vector2f& vector2)
{
	// Return the dot product
	return vector1.x * vector2.x + vector1.y * vector2.y;
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 2 February
 * Description: Calculates the sum of the vector
 * ----------------------------------------------------------------------
 */
float magnitude(const sf::Vector2f& vector)
{
	// Return the sum of the vector
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 1 February
 * Description: Normalizes the vector
 * ----------------------------------------------------------------------
 */
sf::Vector2f normalize(const sf::Vector2f& vector)
{
	// Normalize the vector
	float sum = magnitude(vector);
	return sf::Vector2f(vector.x / sum, vector.y / sum);
}

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 28 March
 * Description: Returns tru if the sprite is within the camera
 * ----------------------------------------------------------------------
 */
bool isOnScreen(const sf::Sprite& sprite, const sf::View& camera)
{
	// Calculate the position of the camera
	float camPosX = camera.getCenter().x - camera.getSize().x / 2.0f;
	float camPosY = camera.getCenter().y - camera.getSize().y / 2.0f;

	// Return true if the sprite is on the view of the camera
	return sprite.getPosition().x + sprite.getLocalBounds().width > camPosX && sprite.getPosition().x < camPosX + camera.getSize().x &&
		sprite.getPosition().y + sprite.getLocalBounds().height > camPosY && sprite.getPosition().y < camPosY + camera.getSize().y;
}

float randomFloat(float min, float max)
{
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min + 1.0f)));
}

int randomInteger(int min, int max)
{
	return min + rand() / (RAND_MAX / (max - min + 1));
}

float distance(const sf::Vector2f first, const sf::Vector2f second)
{
	float x = second.x - first.x;
	float y = second.y - first.y;

	return sqrtf(x * x + y * y);
}