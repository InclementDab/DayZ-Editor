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


class EditorXMLSpawnsCallback: XMLCallback
{
	private ref array<ref EditorEventSpawn> m_Events;
	
	void EditorXMLSpawnsCallback(ref array<ref EditorEventSpawn> events)
	{
		m_Events = events;
	}
	
	override void OnStart(ref XMLDocument document)
    {
		Print("EditorXMLSpawnsCallback::Start");	

	}
	
	
	override void OnSuccess(ref XMLDocument document)
	{
		Print("EditorXMLSpawnsCallback::Success");		
		XMLElement content = document.Get(1).GetContent();
		
		
		for (int i = 0; i < content.Count(); i++) {
			XMLTag tag = content.Get(i);
			
			//Print(tag.GetName());
		}

	}
	
	
	override void OnFailure(ref XMLDocument document)
	{
		Print("EditorXMLSpawnsCallback::Failure");	
		
	}
	
}


class EditorEventManager
{
	static ref array<ref EditorEventSpawn> m_Events;
	static ref EditorXMLSpawnsCallback m_XMLCallback;
	
	void EditorEventManager()
	{
		m_Events = new array<ref EditorEventSpawn>();
	}
	
	static void ImportEventPositions()
	{
		m_Events = new array<ref EditorEventSpawn>();
		
		
		string file = "$profile:\\Editor\\cfgeventspawns.xml";
		if (!FileExist(file)) {
			Print("File not found!");
			return;
		}
		if (m_XMLCallback == null) {
			delete m_XMLCallback;
			m_XMLCallback = new EditorXMLSpawnsCallback(m_Events);
			GetXMLApi().Read(file, m_XMLCallback);
		}
		
		// debug
		foreach (EditorEventSpawn event_spawn: m_Events) {
			
			foreach (EventPosition pos: event_spawn.m_EventPositions) {
				vector position = pos.GetPosition();
				if (position[1] == 0)	
					position[1] = GetGame().SurfaceY(position[0], position[2]);
				
				GetGame().CreateObject("BrushBase", position);
			}
			
			
		}
		
		
	}
	
}