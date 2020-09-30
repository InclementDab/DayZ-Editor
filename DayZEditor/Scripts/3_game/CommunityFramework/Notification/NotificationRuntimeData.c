modded class NotificationRuntimeData
{
	string m_Icon;
	string m_TitleText;

	int m_Color;

	bool m_HasShown;

	void NotificationRuntimeData( float time, NotificationData data, string detail_text )
	{		
		m_StaticData = data;
		m_Icon = data.m_Icon;
		m_TitleText = data.m_TitleText;

		if ( detail_text != "" )
		{
			m_DetailText	= detail_text;
		} else
		{
			m_DetailText	= m_StaticData.m_DescriptionText;
		}

		m_Color = ARGB( 255, 221, 38, 38 );
		m_NotificationTime = time + NOTIFICATION_FADE_TIME;
	}

	void SetColor( int color )
	{
		m_Color = color;
	}

	int GetColor()
	{
		return m_Color;
	}

	override string GetIcon()
	{
		return m_Icon;
	}
	
	override string GetTitleText()
	{
		return m_TitleText;
	}
}