class EditorMainMenuStats: ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	
	protected Widget m_TimePlayed;
	protected TextWidget m_TimePlayedValue;
	
	protected Widget m_ObjectsPlaced;
	protected TextWidget m_ObjectsPlacedValue;
	
	protected Widget m_ObjectsRemoved;
	protected TextWidget m_ObjectsRemovedValue;
	
	protected Widget m_DistanceTraveled;
	protected TextWidget m_DistanceTraveledValue;
	/*
	protected Widget m_LongRangeShot;
	protected TextWidget m_LongRangeShotValue;*/
	
	void EditorMainMenuStats(Widget root)
	{
		m_Root = root;
		
		m_TimePlayed = m_Root.FindAnyWidget("EditorPlayTime");
		m_TimePlayedValue = TextWidget.Cast(m_Root.FindAnyWidget("EditorPlayTimeValue"));
		
		m_ObjectsPlaced = m_Root.FindAnyWidget("ItemsPlaced");
		m_ObjectsPlacedValue = TextWidget.Cast( m_Root.FindAnyWidget("ItemsPlacedValue"));
		
		m_ObjectsRemoved = m_Root.FindAnyWidget("ItemsRemoved");
		m_ObjectsRemovedValue = TextWidget.Cast(m_Root.FindAnyWidget("ItemsRemovedValue"));
		
		m_DistanceTraveled = m_Root.FindAnyWidget( "DistanceTraveled" );
		m_DistanceTraveledValue = TextWidget.Cast( m_Root.FindAnyWidget( "DistanceTraveledValue" ) );
		/*
		m_LongRangeShot = m_Root.FindAnyWidget( "LongRangeShot" );
		m_LongRangeShotValue = TextWidget.Cast( m_Root.FindAnyWidget( "LongRangeShotValue" ) );*/
	}
	
	void UpdateStats()
	{
		m_TimePlayedValue.SetText(GetTimeString(EditorStatistics.GetInstance().EditorPlayTime));
		m_ObjectsPlacedValue.SetText(EditorStatistics.GetInstance().EditorPlacedObjects.ToString());
		m_ObjectsRemovedValue.SetText(EditorStatistics.GetInstance().EditorRemovedObjects.ToString());
		m_DistanceTraveledValue.SetText(GetDistanceString(EditorStatistics.GetInstance().EditorDistanceFlown));
	}
	
	protected string GetTimeString( float total_time )
	{
		string day_symbol = "d";							//define symbols
		string hour_symbol = "h";
		string minute_symbol = "min";
		
		if ( total_time > 0 )
		{
			string time_string;		
			int time_seconds = total_time; 						//convert total time to int
			
			int days = time_seconds / 3600 / 24;
			int hours = time_seconds / 3600 % 24;
			int minutes = ( time_seconds % 3600 ) / 60;
			
			if ( days > 0 )
			{
				time_string += GetValueString( days ) + day_symbol;		//days
				time_string += " ";										//separator
			}
			
			if ( hours > 0 || days > 0 )
			{
				time_string += GetValueString( hours ) + hour_symbol;	//hours
				time_string += " ";										//separator
			}			

			if ( minutes >= 0 )
			{
				time_string += GetValueString( minutes ) + minute_symbol;	//minutes
			}			

			return time_string;
		}
		
		return "0" + " " + minute_symbol;
	}
	
	protected string GetDistanceString(float total_distance, bool meters_only = false)
	{
		const int DISTANCE_TO_MOON = 384400;
		const int CIRCUMFERENCE_OF_EARTH = 40075;
		
		string meter_symbol = "m";							//define symbols
		string kilometer_symbol = "km";
		
		if (total_distance > DISTANCE_TO_MOON * 0.5) {
			return string.Format("%1 To The Moon", total_distance / DISTANCE_TO_MOON);
		}
		
		if (total_distance > CIRCUMFERENCE_OF_EARTH) {
			return string.Format("%1x Around Earth", total_distance / CIRCUMFERENCE_OF_EARTH);
		}
		
		if ( total_distance > 0 )
		{
			string distance_string;
			
			float kilometers = total_distance;
			kilometers = Math.Round( kilometers );
			if ( kilometers >= 10 && !meters_only )
			{
				distance_string = GetValueString( kilometers, true ) + kilometer_symbol;		//kilometers
			}
			else
			{
				distance_string = GetValueString( total_distance ) + meter_symbol;			//meters
			}
			
			return distance_string;
		}	
	
		return "0" + meter_symbol;
	}
	
	protected string GetValueString( float total_value, bool show_decimals = false )
	{
		if ( total_value > 0 )
		{
			string out_string;
			
			int total_value_int = total_value;
			string number_str = total_value_int.ToString();
			
			//number
			if ( total_value >= 1000 )
			{
				int count;		
				int first_length = number_str.Length() % 3;		//calculate position of the first separator
				if ( first_length > 0 )
				{
					count = 3 - first_length;
				}
				
				for ( int i = 0; i < number_str.Length(); ++i )
				{
					out_string += number_str.Get( i );
					count ++;
					
					if ( count >= 3 )
					{
						out_string += " ";						//separator
						count = 0;
					}
				}
			}
			else
			{
				out_string = number_str;
			}
			
			//decimals
			if ( show_decimals )
			{
				string total_value_str = total_value.ToString();
				int decimal_idx = total_value_str.IndexOf( "." );
			
				if ( decimal_idx > -1 )
				{
					out_string.TrimInPlace();
					out_string += total_value_str.Substring( decimal_idx, total_value_str.Length() - decimal_idx );
				}
			}

			return out_string;
		}
		
		return "0";
	}
}