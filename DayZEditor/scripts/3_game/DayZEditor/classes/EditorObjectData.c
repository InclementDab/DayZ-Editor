
enum EditorObjectFlags {
	NONE = 0,
	BBOX = 2,
	MAPMARKER = 4,
	OBJECTMARKER = 8,
	LISTITEM = 16,
	ALL = 30 // 2 | 4 | 8 | 16
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
	vector Transform[4];

	EditorObjectFlags Flags;
	
	[NonSerialized()]
	ModStructure ObjectMod;
	
	void EditorObjectData() {}
		
	static EditorObjectData Create(string type, vector position, vector orientation, EditorObjectFlags flags = EditorObjectFlags.ALL)
	{
		vector transform[4];
		
		Math3D.YawPitchRollMatrix(orientation, transform);
		transform[3] = position;
		return Create(type, transform, flags);
	}
	
	static EditorObjectData Create(string type, vector transform[4], EditorObjectFlags flags = EditorObjectFlags.ALL)
	{
		EditorLog.Trace("EditorObjectData::Create");
		
		if (GetGame().GetModelName(type) == "UNKNOWN_P3D_FILE") {
			EditorLog.Warning(string.Format("EditorObjectData::Create %1 is not a valid Object Type!", type));
			return null;
		}
		
		EditorObjectData data = new EditorObjectData();
		data.Type = type; 
		data.Transform = transform; 
		data.Flags = flags;
		data.DisplayName = data.Type;
		//data.ObjectMod = GetModFromObject(data.Type); todo refactor.
		

		if (lowest_id == 0) lowest_id = 200000;
		lowest_id--;
		data.m_Id = lowest_id;	
		
		EditorLog.Debug(string.Format("EditorObjectData::Create ID: %1", data.m_Id));
				
		return data;
	}
	
	
	static EditorObjectData Create(notnull Object target, EditorObjectFlags flags = EditorObjectFlags.ALL)
	{
		EditorObjectData data = new EditorObjectData();
		data.Type = target.GetType(); 
		target.GetTransform(data.Transform); 
		data.Flags = flags;
		data.DisplayName = data.Type;
		//data.ObjectMod = GetModFromObject(data.Type);
		data.m_Id = target.GetID();	
		
		EditorLog.Debug(string.Format("EditorObjectData::Create ID: %1", data.m_Id));
		
		return data;
	}
	
	vector GetPosition() {
		return Transform[3];
	}
	
	vector GetOrientation() {
		return Math3D.MatrixToAngles(Transform);
	}
	
	float GetScale() {
		return 1; // not supported yet
	}
	
	void OnSend(Serializer serializer)
	{
		serializer.Write(Type);
		serializer.Write(Transform);
		serializer.Write(Flags);
	}
	
	void OnRead(Serializer serializer)
	{
		serializer.Read(Type);
		serializer.Read(Transform);
		serializer.Read(Flags);
	}
}
