#include "Time.h"

bool operator==(const Time& tm1, const Time& tm2)
{
	return ((tm1.year == tm2.year) && (tm1.day == tm2.day)
		&& (tm1.hour == tm2.hour) && (tm1.minutes == tm2.minutes) && (tm1.seconds == tm2.seconds));
}

bool operator<(const Time& tm1, const Time& tm2)
{
	return (
		(tm1.year < tm2.year) 
		|| ((tm1.year == tm2.year) && (tm1.month < tm2.month))
		|| ((tm1.year == tm2.year) && (tm1.month == tm2.month) && (tm1.day < tm2.day))
		|| ((tm1.year == tm2.year) && (tm1.month == tm2.month) && (tm1.day == tm2.day) && (tm1.hour < tm2.hour))
		|| ((tm1.year == tm2.year) && (tm1.month == tm2.month) && (tm1.day == tm2.day) && (tm1.hour == tm2.hour) && (tm1.minutes < tm2.minutes))
		|| ((tm1.year == tm2.year) && (tm1.month == tm2.month) && (tm1.day == tm2.day) && (tm1.hour == tm2.hour) && (tm1.minutes == tm2.minutes)) && (tm1.seconds < tm2.seconds));
}

bool operator>(const Time& tm1, const Time& tm2)
{
	return (
		(tm1.year > tm2.year)
		|| ((tm1.year == tm2.year) && (tm1.month > tm2.month))
		|| ((tm1.year == tm2.year) && (tm1.month == tm2.month) && (tm1.day > tm2.day))
		|| ((tm1.year == tm2.year) && (tm1.month == tm2.month) && (tm1.day == tm2.day) && (tm1.hour > tm2.hour))
		|| ((tm1.year == tm2.year) && (tm1.month == tm2.month) && (tm1.day == tm2.day) && (tm1.hour == tm2.hour) && (tm1.minutes > tm2.minutes))
		|| ((tm1.year == tm2.year) && (tm1.month == tm2.month) && (tm1.day == tm2.day) && (tm1.hour == tm2.hour) && (tm1.minutes == tm2.minutes)) && (tm1.seconds > tm2.seconds));
}


bool operator<=(const Time& tm1, const Time& tm2)
{
	return ((tm1 < tm2) || (tm1 == tm2));
}

bool operator>=(const Time& tm1, const Time& tm2)
{
	return (((tm1 > tm2) || (tm1 == tm2)));
}

istream& operator>>(istream& in, Time& tm)
{
	return in;
}

ostream& operator<<(ostream& out, const Time& tm)
{
	return out << tm.year << "/" << tm.month << "/" << tm.day << ", " << tm.hour << ":" << tm.minutes << ":" << tm.seconds << endl;
}