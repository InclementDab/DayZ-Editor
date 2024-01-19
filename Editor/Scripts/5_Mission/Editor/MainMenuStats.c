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
	
	static string GetTimeString(int total_time)
	{		
		if (total_time == 0) {
			return "0 min";
		}
			
		int days = total_time / 3600 / 24;
		int hours = total_time / 3600 % 24;
		int minutes = (total_time % 3600) / 60;
		
		string time_string;
		
		if (days > 0) {
			time_string += GetValueString(days) + "d ";
		}
		
		if (hours > 0 || days > 0) {
			time_string += GetValueString(hours) + "h ";							
		}			

		if (minutes >= 0) {
			time_string += GetValueString(minutes) + "min";
		}

		return time_string;
	}
	
	static string GetDistanceString(float total_distance, bool meters_only = false)
	{	// When bodies closest.
		const int CIRCUMFERENCE_OF_EARTH = 40075;
		const int DISTANCE_TO_MOON = 384400;  // 384400 km
		const int DISTANCE_TO_MARS = 54600000; // 54.6 million km
		const int DISTANCE_TO_SUN = 147500000; // 147.5 million km
		const int DISTANCE_TO_JUPITER = 5880000000; // 588 million km
		// very surprised if anyone can reach saturn NO CHEATING
		const int DISTANCE_TO_SATURN = 1200000000; // 1.2 billion km
		
		
		string meter_symbol = "m";							//define symbols
		string kilometer_symbol = "km";
		
		if (total_distance == 0) {
			return "0m";
		}
		
		if (total_distance > DISTANCE_TO_SATURN) {
			return string.Format("%1x To Saturn", total_distance / DISTANCE_TO_SATURN);
		}
		
		if (total_distance > DISTANCE_TO_SUN * 2) {
			return string.Format("%1x To Jupiter", total_distance / DISTANCE_TO_JUPITER);
		}
		
		if (total_distance > DISTANCE_TO_MARS * 2) {
			return string.Format("%1x To The Sun", total_distance / DISTANCE_TO_SUN);
		}
		
		if (total_distance > DISTANCE_TO_MARS * 0.25) {
			return string.Format("%1x To Mars", total_distance / DISTANCE_TO_MARS);
		}
		
		if (total_distance > DISTANCE_TO_MOON * 0.25) {
			return string.Format("%1x To The Moon", total_distance / DISTANCE_TO_MOON);
		}		
		
		if (total_distance > CIRCUMFERENCE_OF_EARTH * 0.25) {
			return string.Format("%1x Around Earth", total_distance / CIRCUMFERENCE_OF_EARTH);
		}
	
		float kilometers = Math.Round(total_distance);
		if (kilometers >= 10 && !meters_only) {
			return GetValueString(kilometers, true) + "km";
		}
		
		return GetValueString(total_distance) + "m";
	}
	
	static string GetValueString(float total_value, bool show_decimals = false)
	{
		if (total_value == 0) {
			return "0";
		}
		
		string out_string;
		int total_value_int = total_value;
		string number_str = total_value_int.ToString();
		out_string = number_str;
		//number
		if (total_value >= 1000) {		
			out_string = "";		
			for (int i = 0; i < number_str.Length(); i++) {
				out_string += number_str[i];
				if (i % 3 == 1 && i != number_str.Length() - 1) {
					out_string += ",";
				}
			}
		}

		//decimals
		if (!show_decimals) {
			return out_string;
		}

		string total_value_str = total_value.ToString();
		int decimal_idx = total_value_str.IndexOf(".");
	
		if (decimal_idx > -1) {
			out_string.TrimInPlace();
			out_string += total_value_str.Substring(decimal_idx, total_value_str.Length() - decimal_idx);
		}
		
		return out_string;
	}
}