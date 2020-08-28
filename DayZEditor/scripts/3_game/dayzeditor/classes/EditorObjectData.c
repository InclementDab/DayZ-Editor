
enum EditorObjectFlags
{
	NONE = 0,
	BBOX = 2,
	MAPMARKER = 4,
	OBJECTMARKER = 8,
	LISTITEM = 16,
	ALL = 256
};


class EditorObjectDataSet: map<int, ref EditorObjectData>
{
	
	bool InsertEditorData(EditorObjectData data)
	{		
		Insert(data.GetID(), data);
		return true;
	}
	
	bool RemoveEditorData(EditorObjectData data)
	{
		Remove(data.GetID());
		return true;
	}
	
}

// temp until i can find a better way to find "First" in a map that doesnt blow the software up
static int lowest_id;
class EditorObjectData
{	
	private int m_Id;
	int GetID() { return m_Id; }
	
	string Type;
	string DisplayName;
	vector Position;
	vector Orientation;
	float Scale;
	EditorObjectFlags Flags;
	ModStructure ObjectMod;
	
	void EditorObjectData() {}
	
	static EditorObjectData Create(string type, vector position, vector orientation = "0 0 0", EditorObjectFlags flags = EditorObjectFlags.ALL)
	{
		EditorLog.Trace("EditorObjectData::Create");
		
		if (GetGame().GetModelName(type) == "UNKNOWN_P3D_FILE") {
			EditorLog.Error(string.Format("EditorObjectData::Create %1 is not a valid Object Type!", type));
			return null;
		}
		
		EditorObjectData data = new EditorObjectData();
		data.Type = type; data.Position = position; data.Orientation = orientation; data.Flags = flags;
		data.DisplayName = data.Type;
		data.ObjectMod = GetModFromObject(data.Type);
		

		if (lowest_id == 0) lowest_id = 200000;
		lowest_id--;
		data.m_Id = lowest_id;	
		
		EditorLog.Debug(string.Format("EditorObjectData::Create ID: %1", data.m_Id));
		
		
		return data;
	}
	
	static EditorObjectData Create(EditorPlaceableObjectData data)
	{
		
	}
	
	void OnSend(Serializer serializer)
	{
		serializer.Write(Type);
		serializer.Write(Position);
		serializer.Write(Orientation);
		serializer.Write(Scale);
		serializer.Write(Flags);
	}
	
	void OnRead(Serializer serializer)
	{
		serializer.Read(Type);
		serializer.Read(Position);
		serializer.Read(Orientation);
		serializer.Read(Scale);
		serializer.Read(Flags);
	}
}
