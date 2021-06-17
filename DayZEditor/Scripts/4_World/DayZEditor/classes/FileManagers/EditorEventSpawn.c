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
	private ref array<string> m_TypeNames;
	
	void EditorEventSpawn(string event_name)
	{
		m_EventName = event_name;
		m_EventPositions = new array<ref EventPosition>();
		m_TypeNames = new array<string>();
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
	
	void InsertTypeName(string name)
	{
		m_TypeNames.Insert(name);
	}
	
	ref array<string> GetTypeNames()
	{
		return m_TypeNames;
	}
	
	string GetRandomTypeName()
	{
		return m_TypeNames.Get(Math.RandomInt(0, m_TypeNames.Count()));
	}
	
	string GetName() { return m_EventName; }
}



class XMLEventsCallback: XMLCallback
{
	private ref array<ref EditorEventSpawn> m_Events;
	
	void XMLEventsCallback(ref array<ref EditorEventSpawn> events)
	{
		m_Events = events;
	}
	
	bool SearchEventSpawns(out ref EditorEventSpawn event_spawn, string name)
	{
		name.ToLower();
		foreach (EditorEventSpawn espawn: m_Events) {
			string name_lower = espawn.GetName();
			name_lower.ToLower();
			if (name_lower == name) {
				event_spawn = espawn;
				return true;
			}
		}
		
		Print("Object Search Failed");
		return false;
	}
	
	override void OnSuccess(ref XMLDocument document)
	{
		Print("XMLEventsCallback::Success");		
		XMLElement xml_events = document.Get(1).GetContent();
		
		for (int i = 0; i < xml_events.Count(); i++) {
			
			XMLTag event_tag = xml_events.Get(i);
			if (event_tag.GetName() != "event") 
				continue;
			
			XMLAttribute name_attribute = event_tag.GetAttribute("name");
			string name = name_attribute.ValueAsString();
			EditorEventSpawn event_spawn;
			
			if (SearchEventSpawns(event_spawn, name)) {
								
				XMLElement event_content = event_tag.GetContent();
				for (int j = 0; j < event_content.Count(); j++) {
					
					XMLTag children_tag = event_content.Get(j);
					if (children_tag.GetName() != "children")
						continue;
					
					XMLElement children_content = children_tag.GetContent();
					for (int k = 0; k < children_content.Count(); k++) {
						
						XMLTag spawnable_object = children_content.Get(k);
						if (spawnable_object.GetName() != "child")
							continue;
						
						XMLAttribute type_attribute = spawnable_object.GetAttribute("type");						
						event_spawn.InsertTypeName(type_attribute.ValueAsString());
					}					
				}
			}
		}
		

		// Debug
		foreach (ref EditorEventSpawn espawn: m_Events) {
			
			ref array<ref EventPosition> event_positions = espawn.GetPositions();
			foreach (ref EventPosition pos: event_positions) {
				vector position = pos.GetPosition();
				if (position[1] == 0)	
					position[1] = GetGame().SurfaceY(position[0], position[2]);
				
				string ename = espawn.GetRandomTypeName();
				if (ename.Length() == 0) continue;
				if (position == vector.Zero) continue;
				if (GetGame().IsKindOf(ename, "DZ_LightAI")) continue;
				
				GetEditor().CreateObject(EditorObjectData.Create(ename, position));
				
				//Object o = GetGame().CreateObjectEx(ename, position, ECE_NONE);
				//o.SetFlags(EntityFlags.STATIC, true);
			}
		}
	}
}


class XMLEventSpawnsCallback: XMLCallback
{
	private ref array<ref EditorEventSpawn> m_Events;
	
	void XMLEventSpawnsCallback(ref array<ref EditorEventSpawn> events)
	{
		m_Events = events;
	}
	
	
	
	override void OnSuccess(ref XMLDocument document)
	{
		Print("XMLEventSpawnsCallback::Success");		
		XMLElement content = document.Get(1).GetContent();
		
		for (int i = 0; i < content.Count(); i++) {
						
			if (content.Get(i).GetName() != "event") 
				continue;
			
			XMLAttribute name_attr = content.Get(i).GetAttribute("name");
			EditorEventSpawn event_spawn = new EditorEventSpawn(name_attr.ValueAsString());
			
			
			XMLElement element = content.Get(i).GetContent();
			for (int j = 0; j < element.Count(); j++) {
				
				if (element.Get(j).GetName() != "pos") 
					continue;
				
				event_spawn.InsertPosition(EventPosition.CreateFromXMLTag(element.Get(j)));
			}
			
			m_Events.Insert(event_spawn);
		}
		
		
		EditorEventManager.ImportEvents();
	}
	
	
	override void OnFailure(ref XMLDocument document)
	{
		Print("XMLEventSpawnsCallback::Failure");	
	}
	
}


class EditorEventManager
{
	static ref array<ref EditorEventSpawn> m_Events;
	static ref XMLEventsCallback m_XMLEventsCallback;
	static ref XMLEventSpawnsCallback m_XMLEventSpawnsCallback;
	
	void EditorEventManager()
	{
	}
	

	
	static void ImportEventPositions()
	{
		m_Events = new array<ref EditorEventSpawn>();
		
		string file = "$profile:\\Editor\\cfgeventspawns.xml";
		if (!FileExist(file)) {
			Print("File not found!"); // todo replace with the new fileopen dialog result type
			return;
		}
	
		m_XMLEventSpawnsCallback = new XMLEventSpawnsCallback(m_Events);
		GetXMLApi().Read(file, m_XMLEventSpawnsCallback);		
		
	}
	
	
	static void ImportEvents()
	{
		string file = "$profile:\\Editor\\db\\events.xml";
		if (!FileExist(file)) {
			Print("File not found!");
			return;
		}
		
		m_XMLEventsCallback = new XMLEventsCallback(m_Events);
		GetXMLApi().Read(file, m_XMLEventsCallback);
	
	}
}