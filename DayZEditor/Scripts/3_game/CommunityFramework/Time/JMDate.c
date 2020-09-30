class JMDate : Managed
{
    // TODO: Stringtable
    static const string MONTHS_FULL_NAME[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
    static const string MONTHS_SHORT_NAME[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    static const int DAYS_IN_MONTH[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    private bool m_UseUTC;

    private int m_Year;
    private int m_Month;
    private int m_Day;
    private int m_Hour;
    private int m_Minute;
    private int m_Second;

    private void JMDate()
    {
    }

    static JMDate Now( bool useUTC )
    {
        ref JMDate date = new JMDate();
        date.m_UseUTC = useUTC;

        if ( date.m_UseUTC )
        {
            GetYearMonthDayUTC( date.m_Year, date.m_Month, date.m_Day );
            GetHourMinuteSecondUTC( date.m_Hour, date.m_Minute, date.m_Second );
        } else
        {
            GetYearMonthDay( date.m_Year, date.m_Month, date.m_Day );
            GetHourMinuteSecond( date.m_Hour, date.m_Minute, date.m_Second );
        }

        return date;
    }

    /** Utils */
    static bool IsLeapYear( int year )
    {
        if ( year % 400 == 0 )
            return true;

        if ( year % 100 == 0 )
            return false;

        if ( year % 4 == 0 )
            return true;

        return false;
    }

    /**
	* Based on: https://en.wikipedia.org/wiki/Unix_time
	*
	* MAJOR NOTE:
	*	Timestamp start at Year 1970, if any functions / methods linked
	*	to Timestamp, be aware of using any date at least starting at this
	*	year.
	*
	* @param {int} m_Year
	* @param {int} month
	* @param {int} m_Day
	* @param {int} m_Hour
	* @param {int} m_Minute
	* @param {int} m_Second
	*
	* @return Timestamp in m_Second
	*/
	static int Timestamp( int year, int month, int day, int hours, int minutes, int seconds )
	{
		if ( year < 1970 )
            return 0;

		bool isLeapYear = IsLeapYear( year );
        int timestamp = 0;

        for ( int iYear = 1970; iYear < year; ++iYear )
        {
            if ( IsLeapYear(iYear) )
			{
                timestamp += 31622400;
			}
            else
			{
                timestamp += 31536000;
			}
        }

        for ( int iMonth = 1; iMonth < month; iMonth++ )
        {
            if ( isLeapYear && iMonth == 2 )
			{
                timestamp += 86400;
			}
            timestamp += DAYS_IN_MONTH[ iMonth - 1 ] * 86400;
        }

        for ( int iDay = 1; iDay < day; iDay++ )
        {
            timestamp += 86400;
        }

        timestamp += hours * 3600;
        timestamp += minutes * 60;
        timestamp += seconds;
        return timestamp;
	}

    static void TimestampToDate( int timestamp, out int year, out int month, out int day, out int hours, out int minutes, out int seconds )
    {
		if (timestamp < 0)
            return;

        int iTimestamp = 0;
        iTimestamp = TimestampCalculYear( timestamp, year );
        iTimestamp = TimestampCalculMonth( iTimestamp, timestamp, year, month );
        iTimestamp = TimestampCalculDays( iTimestamp, timestamp, day );
        iTimestamp = TimestampCalculHours( iTimestamp, timestamp, hours );
        iTimestamp = TimestampCalculMinutes( iTimestamp, timestamp, minutes );
        iTimestamp = TimestampCalculSeconds( iTimestamp, timestamp, seconds );
    }

    protected static int TimestampCalculYear( int timestamp, out int year )
    {
        int iTimestamp = 0;
        year = 1970;

        while ( iTimestamp < timestamp )
        {
			int yearTimestamp;

            if ( IsLeapYear( year ) )
                yearTimestamp = 31622400;
			else
				yearTimestamp = 31536000;

			if ( iTimestamp + yearTimestamp > timestamp )
                break;

			iTimestamp += yearTimestamp;
            year++;
        }

        return iTimestamp;
    }

	protected static int TimestampCalculMonth( int iTimestamp, int timestamp, int year, out int month )
    {
        bool isLeapYear = IsLeapYear( year );
        month = 0;

        while ( iTimestamp < timestamp )
        {
			int monthTimestamp;
			if ( isLeapYear && month == 1 )
				monthTimestamp = 2505600;
			else
				monthTimestamp = DAYS_IN_MONTH[ month ] * 86400;

			if ( iTimestamp + monthTimestamp > timestamp )
                break;

			iTimestamp += monthTimestamp;
            month++;
        }

		month++;
        return iTimestamp;
    }

    protected static int TimestampCalculDays( int iTimestamp, int timestamp, out int days )
    {
        days = 1;

        while ( iTimestamp < timestamp )
        {
            if ( iTimestamp + 86400 > timestamp )
                break;

            iTimestamp += 86400;
            days++;
        }

        return iTimestamp;
    }

    protected static int TimestampCalculHours( int iTimestamp, int timestamp, out int hours )
    {
        hours = 0;

        while ( iTimestamp < timestamp )
        {
            if ( iTimestamp + 3600 > timestamp )
                break;

            iTimestamp += 3600;
            hours++;
        }

        return iTimestamp;
    }

    protected static int TimestampCalculMinutes( int iTimestamp, int timestamp, out int minutes )
    {
        minutes = 0;

        while ( iTimestamp < timestamp )
        {
            if ( iTimestamp + 60 > timestamp )
                break;
            iTimestamp += 60;
            minutes++;
        }

        return iTimestamp;
    }

    protected static int TimestampCalculSeconds( int iTimestamp, int timestamp, out int seconds )
    {
        seconds = 0;

        while ( iTimestamp < timestamp )
        {
            iTimestamp++;
            seconds++;
        }

        return iTimestamp;
    }

	/**
	*	@return {string} JMDate with format: "month day, year hours:minutes:seconds"
	*
	*	Note:
	*		0 in front of numbers are not includes.
	*/
	string DateToString()
	{
		string dateToString = GetFullMonthString();

		dateToString += " " + m_Day;
		dateToString += ", " + m_Year;
		dateToString += " " + m_Hour + ":" + m_Minute + ":" + m_Second;
		return dateToString;
	}

    // Supply a format such as "YYYY-MM-DD hh:mm:ss"
    string ToString( string format )
    {
        format = FormatYear( format );
        format = FormatMonth( format );
        format = FormatDay( format );
        format = FormatHour( format );
        format = FormatMinute( format );
        format = FormatSecond( format );
        return format;
    }

    private string FormatYear( string format )
    {
        string year = "" + m_Year;
        int i = year.Length();
        if ( format.IndexOf( "YYYY" ) != -1 )
        {
            if ( i > 4 )
            {
                year = year.Substring( i - 4, 4 );
            } else if ( i < 4 )
            {
                for ( i; i < 4; i++ )
                {
                    year = "0" + year;
                }
            }

            format.Replace( "YYYY", year );
        } else if ( format.IndexOf( "YY" ) != -1 )
        {
            if ( i > 2 )
            {
                year = year.Substring( i - 2, 2 );
            } else if ( i < 2 )
            {
                for ( i; i < 2; i++ )
                {
                    year = "0" + year;
                }
            }

            format.Replace( "YY", year );
        }

        return format;
    }

    private string FormatMonth( string format )
    {
        string month = "" + m_Month;
        int i = month.Length();
        if ( format.IndexOf( "MM" ) != -1 )
        {
            if ( i > 2 )
            {
                month = month.Substring( i - 2, 2 );
            } else if ( i < 2 )
            {
                for ( i; i < 2; i++ )
                {
                    month = "0" + month;
                }
            }

            format.Replace( "MM", month );
        }

        return format;
    }

    private string FormatDay( string format )
    {
        string day = "" + m_Day;
        int i = day.Length();
        if ( format.IndexOf( "DD" ) != -1 )
        {
            if ( i > 2 )
            {
                day = day.Substring( i - 2, 2 );
            } else if ( i < 2 )
            {
                for ( i; i < 2; i++ )
                {
                    day = "0" + day;
                }
            }

            format.Replace( "DD", day );
        }

        return format;
    }

    private string FormatHour( string format )
    {
        string hour = "" + m_Hour;
        int i = hour.Length();
        if ( format.IndexOf( "hh" ) != -1 )
        {
            if ( i > 2 )
            {
                hour = hour.Substring( i - 2, 2 );
            } else if ( i < 2 )
            {
                for ( i; i < 2; i++ )
                {
                    hour = "0" + hour;
                }
            }

            format.Replace( "hh", hour );
        }

        return format;
    }

    private string FormatMinute( string format )
    {
        string minute = "" + m_Minute;
        int i = minute.Length();
        if ( format.IndexOf( "mm" ) != -1 )
        {
            if ( i > 2 )
            {
                minute = minute.Substring( i - 2, 2 );
            } else if ( i < 2 )
            {
                for ( i; i < 2; i++ )
                {
                    minute = "0" + minute;
                }
            }

            format.Replace( "mm", minute );
        }

        return format;
    }

    private string FormatSecond( string format )
    {
        string second = "" + m_Second;
        int i = second.Length();
        if ( format.IndexOf( "ss" ) != -1 )
        {
            if ( i > 2 )
            {
                second = second.Substring( i - 2, 2 );
            } else if ( i < 2 )
            {
                for ( i; i < 2; i++ )
                {
                    second = "0" + second;
                }
            }

            format.Replace( "ss", second );
        }

        return format;
    }

    /** Getters */
    bool IsUsingUTC()
    {
        return m_UseUTC;
    }

    int GetYear()
    {
        return m_Year;
    }

    int GetMonth()
    {
        return m_Month;
    }

    string GetFullMonthString()
    {
        if ( 0 < m_Month && m_Month < 13 )
		{
            return MONTHS_FULL_NAME[ m_Month - 1 ];
		}

        return "undefined";
    }

    string GetShortMonthString()
    {
        if ( 0 < m_Month && m_Month < 13 )
		{
            return MONTHS_SHORT_NAME[ m_Month - 1 ];
		}

        return "undefined";
    }

    int GetDay()
    {
        return m_Day;
    }

    int GetHours()
    {
        return m_Hour;
    }

    int GetMinutes()
    {
        return m_Minute;
    }

    int GetSeconds()
    {
        return m_Second;
    }
	
	int GetTimestamp()
	{
		return Timestamp( m_Year, m_Month, m_Day, m_Hour, m_Minute, m_Second );
	}

    /** Setters */
    void SetDate( int year, int month, int day, int hours, int minutes, int seconds )
    {
        SetYear( year );
        SetMonth( month );
        SetDay( day );
        SetHours( hours );
        SetMinutes( minutes );
        SetSeconds( seconds );
    }

    void SetDate( int month, int day, int hours, int minutes, int seconds )
    {
        SetMonth( month );
        SetDay( day );
        SetHours( hours );
        SetMinutes( minutes );
        SetSeconds( seconds );
    }

    void SetDate( int day, int hours, int minutes, int seconds )
    {
        SetDay( day );
        SetHours( hours );
        SetMinutes( minutes );
        SetSeconds( seconds );
    }

    void SetDate( int hours, int minutes, int seconds )
    {
        SetHours( hours );
        SetMinutes( minutes );
        SetSeconds( seconds );
    }

    void SetDate( int minutes, int seconds )
    {
        SetMinutes( minutes );
        SetSeconds( seconds );
    }

    void SetYear( int year )
    {
        m_Year = year;
    }

    void SetMonth( int month )
    {
        if ( 0 < month && month < 13 )
		{
            m_Month = month;
			if ( m_Day > DAYS_IN_MONTH[ m_Month - 1 ] )
			{
				m_Day = DAYS_IN_MONTH[ m_Month - 1 ];
			}
		} else
		{
			Error("Invalid SetMonth: " + month + ". Expected a value between 1 and 12!");
		}
    }

    void SetDay( int day )
    {
        int monthDays = DAYS_IN_MONTH[ m_Month - 1 ] + 1;

        if ( IsLeapYear( m_Year ) && m_Month == 2 )
		{
            monthDays++;
		}

        if ( 0 < day && day < monthDays )
		{
            m_Day = day;
		} else
		{
			Error( "Invalid SetDay: " + day + ". Expected a value between 1 and " + DAYS_IN_MONTH[ m_Month - 1 ] + "!");
		}
    }

    void SetHours( int hours )
    {
        if ( -1 < hours && hours < 24 )
		{
            m_Hour = hours;
		} else
		{
			Error( "Invalid SetHours: " + hours + ". Expected a value between 0 and 23!");
		}
    }

    void SetMinutes( int minutes )
    {
        if ( -1 < minutes && minutes < 60 )
		{
            m_Minute = minutes;
		} else
		{
			Error( "Invalid SetMinutes: " + minutes + ". Expected a value between 0 and 59!");
		}
    }

    void SetSeconds( int seconds )
    {
        if ( -1 < seconds && seconds < 60 )
		{
            m_Second = seconds;
		} else
		{
			Error( "Invalid SetSeconds: " + seconds + ". Expected a value between 0 and 59!");
		}
    }
};