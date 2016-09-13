#include <time.h>
#include <iostream>
#include <string>

using namespace std;
/**
	<summary> The class Time is meant to manage time related objects. It extends the notion of date and time by implementing a lexicographical order. </summary>
*/
class Time
{
	private:
		int year, month, day, hour, minutes, seconds;

	public:
		Time(int year_ =0, int month_ =0, int day_ =0, int hour_ =0, int minute_ =0, int second_ =0);
		~Time(){};

		// ----
		friend bool operator==(const Time& tm1, const Time& tm2);
		friend bool operator<(const Time& tm1, const Time& tm2);
		friend bool operator>(const Time& tm1, const Time& tm2);
		friend bool operator<=(const Time& tm1, const Time& tm2);
		friend bool operator>=(const Time& tm1, const Time& tm2);
		friend istream& operator>>(const istream& in, const Time& tm);
		friend ostream& operator<<(ostream& out, const Time& tm);

		// ----
		void toCurrentTime();		// sets this Time to current time : year, month, day, hour, minutes and seconds.
		void toCurrentYear();		// sets this Time to current year.
		void toCurrentMonth();		// sets this Time to current year and month.
		void toCurrentDay();		// sets this Time to current year, month and day.
		void toCurrentHour();		// sets this Time to current year, month, day and hour.
		void toCurrentMinutes();	// sets this Time to current year, month, day, hour and minutes.

		// ----
		void setYear(int year_){ year = year_; }
		void setMonth(int month_){ month = month_; }
		void setDay(int day_){ day = day_; }
		void setHour(int hour_){ hour = hour_; }
		void setMinutes(int minutes_){ minutes = minutes_; }
		void setSeconds(int seconds_){ seconds = seconds_; }

		int getYear(){ return year; }
		int getMonth(){ return month; }
		int getDay(){ return day; }
		int getHour(){ return hour; }
		int getMinutes(){ return minutes; }
		int getSeconds(){ return seconds; }

		// ---
		string toString();



};

inline Time::Time(int year_, int month_, int day_, int hour_, int minute_, int second_)
{
	year = year_, month = month_, day = day_, hour = hour_, minutes = minute_, seconds = second_;
}

inline void Time::toCurrentYear()
{
	time_t rawtime;
	struct tm timeinfo;

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	setYear((&timeinfo)->tm_year);
}

inline void Time::toCurrentMonth()
{
	time_t rawtime;
	struct tm timeinfo;

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	setYear((&timeinfo)->tm_year);
	setMonth((&timeinfo)->tm_mon);
}

inline void Time::toCurrentDay()
{
	time_t rawtime;
	struct tm timeinfo;

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	setYear((&timeinfo)->tm_year);
	setMonth((&timeinfo)->tm_mon);
	setDay((&timeinfo)->tm_mday);
}

inline void Time::toCurrentHour()
{
	time_t rawtime;
	struct tm timeinfo;

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	setYear((&timeinfo)->tm_year);
	setMonth((&timeinfo)->tm_mon);
	setDay((&timeinfo)->tm_mday);
	setHour((&timeinfo)->tm_hour);
}

inline void Time::toCurrentMinutes()
{
	time_t rawtime;
	struct tm timeinfo;

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	setYear((&timeinfo)->tm_year);
	setMonth((&timeinfo)->tm_mon);
	setDay((&timeinfo)->tm_mday);
	setHour((&timeinfo)->tm_hour);
	setMinutes((&timeinfo)->tm_min);
}

inline string Time::toString()
{
	return to_string(year) + "/" + to_string(month) + "/" + to_string(day) + ", " + to_string(hour) + ":" + to_string(minutes) + ":" + to_string(seconds);
}

inline void Time::toCurrentTime()
{
	time_t rawtime;
	struct tm timeinfo;

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	setYear((&timeinfo)->tm_year +1900);
	setMonth((&timeinfo)->tm_mon +1);
	setDay((&timeinfo)->tm_mday);
	setHour((&timeinfo)->tm_hour);
	setMinutes((&timeinfo)->tm_min);
	setSeconds((&timeinfo)->tm_sec);

}