#include "Game.h"
#include <Windows.h>
#include <stdexcept>

/* ----------------------------------------------------------------------
 * Author: Julian
 * Date: 27 January 2014
 * Description: Main function: creates the game and runs it
 * ----------------------------------------------------------------------
 */
int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try
	{
		// Create an instance of the game class and run it
		Game game;
		game.run();
	}
	catch(std::exception& exception)
	{
		// Catch an exception if something went wrong
		std::cout<<"Exception: "<<exception.what()<<std::endl;
	}
}