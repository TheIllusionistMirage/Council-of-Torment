#include <iostream>
#include "GlobalTime.h"

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 25 March 2014
 * Description: Constructs the global time
 * ----------------------------------------------------------------------
 */
	GlobalTime::GlobalTime()
	: timeSpeed(200)
	{
		timeOfDay = MORNING;

		hours = 12;
		minutes = 0;
		seconds = 0;
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 25 March 2014
 * Description: Updates the global time
 * ----------------------------------------------------------------------
 */
	void GlobalTime::update(sf::Time elapsedTime)
	{
		// Get the time
		seconds += elapsedTime.asSeconds() * timeSpeed;

		// Change the time
		while(seconds >= 60)
		{
			++minutes;
			seconds -= 60.0f;
		}
		while(minutes >= 60)
		{
			++hours;
			minutes -= 60;
		}
		while(hours >= 24)
			hours -= 24;

		// Depending on the hour we have another time
		if(hours < MORNING)
			timeOfDay = NIGHT;
		else if(hours < DAWN)
			timeOfDay = MORNING;
		else if(hours < NOON)
			timeOfDay = DAWN;
		else if(hours < AFTERNOON)
			timeOfDay = NOON;
		else if(hours < DUSK)
			timeOfDay = AFTERNOON;
		else if(hours < NIGHT)
			timeOfDay = DUSK;
		else
			timeOfDay = NIGHT;
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 25 March 2014
 * Description: Sets the time of the game.
 * Parameters:
 *		h - the hour to set
 *		m - the minute to set
 *		s - the second to set
 * ----------------------------------------------------------------------
 */
	void GlobalTime::setTime(int h, int m, float s)
	{
		hours = h;
		minutes = m;
		seconds = s;

		update(sf::Time::Zero);
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 25 March 2014
 * Description: Sets the time of day
 * Parameters:
 *		id - the time of day for the enum
 * ----------------------------------------------------------------------
 */
	void GlobalTime::setTimeOfDay(int id)
	{
		timeOfDay = id;

		switch(id)
		{
			case MORNING:
				setTime(1, 0, 0);
				break;

			case NOON:
				setTime(12, 0, 0);
				break;

			case AFTERNOON:
				setTime(14, 0, 0);
				break;

			case DUSK:
				setTime(17, 0, 0);
				break;

			case NIGHT:
				setTime(19, 0, 0);
				break;
		}
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 25 March 2014
 * Description: Returns the hour
 * ----------------------------------------------------------------------
 */
	const int& GlobalTime::getHours() const
	{
		return hours;
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 25 March 2014
 * Description: Returns the minutes
 * ----------------------------------------------------------------------
 */
	const int& GlobalTime::getMinutes() const
	{
		return minutes;
	}

/* ----------------------------------------------------------------------
 * Author: Octav
 * Date: 25 March 2014
 * Description: Returns the seconds
 * ----------------------------------------------------------------------
 */
	const float& GlobalTime::getSeconds() const
	{
		return seconds;
	}

/* ----------------------------------------------------------------------
* Author: Octav
* Date: 27 July 2014
* Description: Returns the time of day (enum)
* ----------------------------------------------------------------------
*/
	const int& GlobalTime::getTimeOfDay() const
	{
		return timeOfDay;
	}