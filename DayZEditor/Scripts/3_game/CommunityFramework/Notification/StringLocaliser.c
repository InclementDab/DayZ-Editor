/**
 * StringLocaliser.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2019 DayZ Expansion Mod Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		StringLocaliser
 * @brief		This class handle string formatting over network
 **/
class StringLocaliser
{
	private string m_text;
	private bool m_translates;

	private string m_param1;
	private string m_param2;
	private string m_param3;
	private string m_param4;
	private string m_param5;
	private string m_param6;
	private string m_param7;
	private string m_param8;
	private string m_param9;

	void StringLocaliser( string text, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "" )
	{
		m_text = text;

		m_translates = true;

		m_param1 = param1;
		m_param2 = param2;
		m_param3 = param3;
		m_param4 = param4;
		m_param5 = param5;
		m_param6 = param6;
		m_param7 = param7;
		m_param8 = param8;
		m_param9 = param9;
	}

	void SetTranslates( bool translates )
	{
		m_translates = translates;
	}

	string Format()
	{
		string text = m_text;

		string p1 = m_param1;
		string p2 = m_param2;
		string p3 = m_param3;
		string p4 = m_param4;
		string p5 = m_param5;
		string p6 = m_param6;
		string p7 = m_param7;
		string p8 = m_param8;
		string p9 = m_param9;

		if ( m_translates )
		{
			text = Widget.TranslateString( "#" + m_text );
			if ( text == "" || text.Get( 0 ) == " " )
			{
				text = m_text;
			}

			p1 = Widget.TranslateString( "#" + m_param1 );
			if ( p1 == "" || p1.Get( 0 ) == " " )
			{
				p1 = m_param1;
			}

			p2 = Widget.TranslateString( "#" + m_param2 );
			if ( p2 == "" || p1.Get( 0 ) == " " )
			{
				p2 = m_param2;
			}

			p3 = Widget.TranslateString( "#" + m_param3 );
			if ( p3 == "" || p3.Get( 0 ) == " " )
			{
				p3 = m_param3;
			}

			p4 = Widget.TranslateString( "#" + m_param4 );
			if ( p4 == "" || p4.Get( 0 ) == " " )
			{
				p4 = m_param4;
			}

			p5 = Widget.TranslateString( "#" + m_param5 );
			if ( p5 == "" || p5.Get( 0 ) == " " )
			{
				p5 = m_param5;
			}

			p6 = Widget.TranslateString( "#" + m_param6 );
			if ( p6 == "" || p6.Get( 0 ) == " " )
			{
				p6 = m_param6;
			}

			p7 = Widget.TranslateString( "#" + m_param7 );
			if ( p7 == "" || p7.Get( 0 ) == " " )
			{
				p7 = m_param7;
			}

			p8 = Widget.TranslateString( "#" + m_param8 );
			if ( p8 == "" || p8.Get( 0 ) == " " )
			{
				p8 = m_param8;
			}

			p9 = Widget.TranslateString( "#" + m_param9 );
			if ( p9 == "" || p9.Get( 0 ) == " " )
			{
				p9 = m_param9;
			}
		}

		return string.Format( text, p1, p2, p3, p4, p5, p6, p7, p8, p9 );
	}

	string GetText()
	{
		return m_text;
	}

	StringLocaliser SetParam1( string arg )
	{
		m_param1 = arg;
		return this;
	}

	StringLocaliser SetParam2( string arg )
	{
		m_param2 = arg;
		return this;
	}

	StringLocaliser SetParam3( string arg )
	{
		m_param3 = arg;
		return this;
	}

	StringLocaliser SetParam4( string arg )
	{
		m_param4 = arg;
		return this;
	}

	StringLocaliser SetParam5( string arg )
	{
		m_param5 = arg;
		return this;
	}

	StringLocaliser SetParam6( string arg )
	{
		m_param6 = arg;
		return this;
	}

	StringLocaliser SetParam7( string arg )
	{
		m_param7 = arg;
		return this;
	}

	StringLocaliser SetParam8( string arg )
	{
		m_param8 = arg;
		return this;
	}

	StringLocaliser SetParam9( string arg )
	{
		m_param9 = arg;
		return this;
	}
}