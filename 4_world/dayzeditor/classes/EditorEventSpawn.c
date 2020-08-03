class EventPosition
{
	float x, y, z, a;
	
	static EventPosition CreateFromXML(string linePos)
	{
		ref EventPosition exEvPos = new EventPosition();
	
		int xIndex = linePos.IndexOfFrom(0, "x=");
		int yIndex = linePos.IndexOfFrom(0, "y=");
		int zIndex = linePos.IndexOfFrom(0, "z=");
		int aIndex = linePos.IndexOfFrom(0, "a=");

		int startQuotation = -1;
		int endQuotation = -1;

		if ( xIndex >= 0 ) {
			startQuotation = linePos.IndexOfFrom( xIndex, "\"" );
			endQuotation = linePos.IndexOfFrom( startQuotation + 1, "\"" ) - startQuotation;
			exEvPos.x = linePos.Substring( startQuotation + 1, endQuotation - 1 ).ToFloat();
		}

		if ( yIndex >= 0 ) {
			startQuotation = linePos.IndexOfFrom( yIndex, "\"" );
			endQuotation = linePos.IndexOfFrom( startQuotation + 1, "\"" ) - startQuotation;
			exEvPos.y = linePos.Substring( startQuotation + 1, endQuotation - 1 ).ToFloat();
		}				
		if ( zIndex >= 0 ) {
			startQuotation = linePos.IndexOfFrom( zIndex, "\"" );
			endQuotation = linePos.IndexOfFrom( startQuotation + 1, "\"" ) - startQuotation;
			exEvPos.z = linePos.Substring( startQuotation + 1, endQuotation - 1 ).ToFloat();
		}
		
		if ( aIndex >= 0 ) {
			startQuotation = linePos.IndexOfFrom( aIndex, "\"" );
			endQuotation = linePos.IndexOfFrom( startQuotation + 1, "\"" ) - startQuotation;
			exEvPos.a = linePos.Substring( startQuotation + 1, endQuotation - 1 ).ToFloat();
		}
		
		return exEvPos;
	}
	
	vector GetPosition()
	{
		return Vector(x, y, z);
	}
}


class EditorEventSpawn
{
	string EventName
	ref array<ref EventPosition> m_EventPositions;
	
	void EditorEventSpawn(string eventName)
	{
		EventName = eventName;
		m_EventPositions = new array<ref EventPosition>();
	}
}


class EditorEventManager
{
	static ref array<ref EditorEventSpawn> m_Events;

	void EditorEventManager()
	{
		m_Events = new array<ref EditorEventSpawn>();
	}
	
	static void ImportEventPositions()
	{
		m_Events = new array<ref EditorEventSpawn>();
		if (m_Events && m_Events.Count() >= 0) {
			m_Events.Clear();
		}
		
		array<string> m_XMLData = new array<string>;
	
		FileHandle cfgeventspawnsFile = OpenFile("$profile:cfgeventspawns.xml", FileMode.READ);
		string line_content = "";
		if (cfgeventspawnsFile != 0) {
			while (FGets(cfgeventspawnsFile, line_content) >= 0) {
				m_XMLData.Insert( line_content );
			}
			CloseFile( cfgeventspawnsFile );
		}
	
	
		bool isInRightPlace = false;
		EditorEventSpawn current_event;
		for (int i = 0; i < m_XMLData.Count(); ++i) {
			
			string linePos = m_XMLData[i];
			if (linePos.Contains( "<!--" ))
				continue;
	
			if (linePos.Contains( "</event>" ) && isInRightPlace) {
				isInRightPlace = false;
				m_Events.Insert(current_event);
				continue;
			}
	
			if (linePos.Contains( "<event name=" )) {
				isInRightPlace = true;
	
				TStringArray strs_name = new TStringArray;
				linePos.Split( "\"", strs_name );
	
				current_event = new EditorEventSpawn(strs_name[1]);
				continue;
			}
	
			linePos.TrimInPlace();
	
			if (isInRightPlace && linePos != "") {
				EventPosition event_position = EventPosition.CreateFromXML(linePos);
				current_event.m_EventPositions.Insert(event_position);
			}
			
			
		}
	

		
		// debug
		foreach (EditorEventSpawn event_spawn: m_Events) {
			
			foreach (EventPosition pos: event_spawn.m_EventPositions) {
				vector position = pos.GetPosition();
				if (position[1] == 0)	
					position[1] = GetGame().SurfaceY(position[0], position[2]);
				
				GetGame().CreateObject(event_spawn.EventName, position);
			}
			
			
		}
		
		
	}
	
}