#pragma once
#include <SFML/System.hpp>

enum TimeOfDay
{
	START_MORNING = 5, MORNING = 7,
	START_DAWN = 7, DAWN = 9,
	START_NOON = 9, NOON = 11,
	START_AFTERNOON = 17, AFTERNOON = 18,
	START_DUSK = 18, DUSK = 20,
	START_NIGHT = 20, NIGHT = 22
};

/*	This class keeps track of the global
	game time. The global game time is like
	a very large clock that runs in the game's
	background. Imagine a day-night cycle, it
	would require a virtual time, right? Or
	say you want to make something happen at
	an exact time.
*/
class GlobalTime
{
	private:
		int hours, minutes;
		float seconds;
		int timeOfDay;
		float timeSpeed;

	public:
		GlobalTime();

		void update(sf::Time elapsedTime);
		void setTime(int h, int m, float s);
		void setTimeOfDay(int id);

		const int& getHours() const;
		const int& getMinutes() const;
		const float& getSeconds() const;
};

struct TimePoint
{
	TimePoint(GlobalTime& globalTime) : hour(globalTime.getHours()), minute(globalTime.getMinutes()), second(globalTime.getSeconds()) {}
	TimePoint(int hour, int minute, float second) : hour(hour), minute(minute), second(second) {}

	int hour, minute;
	float second;
};