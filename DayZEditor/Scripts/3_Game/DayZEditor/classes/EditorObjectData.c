
enum EditorObjectFlags {
	NONE = 0,
	BBOX = 2,
	MAPMARKER = 4,
	OBJECTMARKER = 8,
	LISTITEM = 16,
	ALL = 30 // 2 | 4 | 8 | 16
};

// temp until i can find a better way to find "First" in a map that doesnt blow the software up
static int lowest_id;
class EditorObjectData
{	
	[NonSerialized()]
	int m_Id;
	int GetID() { return m_Id; }
	
	string Type;
	string DisplayName;
	vector Position;
	vector Orientation;
	float Scale = 1;
	
	// Re-enable this once we get versioned serializing working :)
	//bool EditorOnly = false;
	
	[NonSerialized()]
	vector BottomCenter;

	EditorObjectFlags Flags;
	
	[NonSerialized()]
	ModStructure Mod;
	
	[NonSerialized()]
	string Icon;
	
	[NonSerialized()]
	Object WorldObject;
	
	void EditorObjectData() 
	{
		lowest_id++;
		m_Id = lowest_id;
		
		//Mod = LoadModData(Type, Path); todo idk
		//Icon = EditorPlaceableItem.GetIcon(Mod);
	}
	
	static ref EditorObjectData Create(string type, vector transform[4], EditorObjectFlags flags = EditorObjectFlags.ALL)
	{	
		return Create(type, transform[3], Math3D.MatrixToAngles(transform), 1, flags);
	}
	
	static ref EditorObjectData Create(string type, vector position, vector orientation, float scale, EditorObjectFlags flags)
	{
		EditorLog.Trace("EditorObjectData::Create");
				
		if (GetGame().GetModelName(type) == "UNKNOWN_P3D_FILE") {
			EditorLog.Warning(string.Format("EditorObjectData::Create %1 is not a valid Object Type!", type));
			//return null;
		}
		
		ref EditorObjectData data = new EditorObjectData();
		data.Type = type; 
		data.Position = position; 
		data.Orientation = orientation;
		data.Scale = scale;
		data.Flags = flags;
		data.DisplayName = data.Type;
		//data.Mod = GetModFromObject(data.Type); todo refactor.

		EditorLog.Debug(string.Format("EditorObjectData::Create ID: %1", data.m_Id));
				
		return data;
	}
	
	
	static ref EditorObjectData Create(notnull Object target, EditorObjectFlags flags = EditorObjectFlags.ALL)
	{
		// We do this because all 'baked' objects are ID'd to 3. cant store a bunch of 3's can we?
		if (target.GetID() == 3) return null;
		
		ref EditorObjectData data = new EditorObjectData();
		data.Type = target.GetType();
		data.WorldObject = target;
		data.Position = data.WorldObject.GetPosition(); 
		data.Orientation = data.WorldObject.GetOrientation(); 
		data.Scale = data.WorldObject.GetScale();
		data.Flags = flags;
		data.DisplayName = data.Type;
		
		EditorLog.Debug(string.Format("EditorObjectData::Create ID: %1", data.m_Id));
		
		return data;
	}
	
	
	void OnSend(Serializer serializer)
	{
		serializer.Write(Type);
		serializer.Write(Position);
		serializer.Write(Orientation);
		serializer.Write(Flags);
	}
	
	void OnRead(Serializer serializer)
	{
		serializer.Read(Type);
		serializer.Read(Position);
		serializer.Read(Orientation);
		serializer.Read(Flags);
	}
}
