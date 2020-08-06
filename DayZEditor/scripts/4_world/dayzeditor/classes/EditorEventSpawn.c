class EventPosition
{
	float x, y, z, a;
	
	void EventPosition(float _x, float _y, float _z, float _a)
	{
		x = _x; y = _y; z = _z; a = _a;
	}
	
	static EventPosition CreateFromXMLTag(XMLTag tag)
	{
		XMLAttribute attribute_x = tag.GetAttribute("x");
		XMLAttribute attribute_y = tag.GetAttribute("y");
		XMLAttribute attribute_z = tag.GetAttribute("z");
		XMLAttribute attribute_a = tag.GetAttribute("a");
		
		
		
		float x1, y1, z1, a1;
		if (attribute_x != null) {
			x1 = attribute_x.ValueAsFloat();
		}
		
		if (attribute_y != null) {
			y1 = attribute_y.ValueAsFloat();
		}
		
		if (attribute_z != null) {
			z1 = attribute_z.ValueAsFloat();
		}
		
		if (attribute_a != null) {
			a1 = attribute_a.ValueAsFloat();
		}
		
	
		return new EventPosition(x1, y1, z1, a1);
	}
	
	vector GetPosition()
	{
		return Vector(x, y, z);
	}
}


class EditorEventSpawn
{
	private string m_EventName;
	private ref array<ref EventPosition> m_EventPositions;
	
	void EditorEventSpawn(string event_name)
	{
		m_EventName = event_name;
		m_EventPositions = new array<ref EventPosition>();
	}
	
	ref array<ref EventPosition> GetPositions()
	{
		return m_EventPositions;
	}
	
	int InsertPosition(EventPosition event_position)
	{
		m_EventPositions.Insert(event_position);
		return m_EventPositions.Count();
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
			
			if (tag.GetName() != "event") 
				continue;
			
			XMLAttribute name_attr = tag.GetAttribute("name");
			EditorEventSpawn event_spawn = new EditorEventSpawn(name_attr.ValueAsString());
			
			XMLElement element = tag.GetContent();
			for (int j = 0; j < element.Count(); j++) {
				
				XMLTag pos_tag = element.Get(j);
				if (pos_tag.GetName() != "pos") 
					continue;
				
				event_spawn.InsertPosition(EventPosition.CreateFromXMLTag(pos_tag));
			}
			
			m_Events.Insert(event_spawn);
		}
		
		
		// Debug
		foreach (EditorEventSpawn espawn: m_Events) {
			ref array<ref EventPosition> event_positions = espawn.GetPositions();
			foreach (EventPosition pos: event_positions) {
				vector position = pos.GetPosition();
				if (position[1] == 0)	
					position[1] = GetGame().SurfaceY(position[0], position[2]);
				
				GetGame().CreateObject("BrushBase", position);
			}
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
		

		
		
	}
	
}