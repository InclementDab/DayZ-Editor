
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
	private int m_Id;
	int GetID() { return m_Id; }
	
	string Type;
	string DisplayName;
	vector Transform[4];

	EditorObjectFlags Flags;
	
	[NonSerialized()]
	ModStructure ObjectMod;
	
	[NonSerialized()]
	Object WorldObject;
	
	void EditorObjectData() 
	{
		// Item limit is 2000000
		if (lowest_id == 0) lowest_id = 2000000;
		lowest_id--;
		m_Id = lowest_id;
	}
	
	void Update()
	{
		// THis is because calling GetTransform(m_Data.Transform) from EditorObject
		// Is broken. pointer offsets? i dont fucking know but i hate it here
		WorldObject.GetTransform(Transform);
	}
		
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
		
		EditorLog.Debug(string.Format("EditorObjectData::Create ID: %1", data.m_Id));
				
		return data;
	}
	
	
	static EditorObjectData Create(notnull Object target, EditorObjectFlags flags = EditorObjectFlags.ALL)
	{
		EditorObjectData data = new EditorObjectData();
		data.Type = target.GetType();
		data.WorldObject = target;
		data.WorldObject.GetTransform(data.Transform); 
		data.Flags = flags;
		data.DisplayName = data.Type;
		
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
		//serializer.Write(Transform);
		serializer.Write(Flags);
	}
	
	void OnRead(Serializer serializer)
	{
		serializer.Read(Type);
		//serializer.Read(Transform);
		serializer.Read(Flags);
	}
}

/*
class EditorObjectDataMap
{
	private ref map<int, ref EditorObjectData> _data = new map<int, ref EditorObjectData>();

	void ~EditorObjectDataMap()
	{
		delete _data;
	}
	
	bool Insert(EditorObjectData data)
	{		
		return _data.Insert(data.GetID(), data);
	}
	/*
	void Insert(EditorObjectDataMap data_set)
	{
		

		for (int i = 0; i < data_set.Count(); i++) {
			Insert(data_set.Get(i));
		}
	}
	
	void Remove(EditorObjectData data)
	{
		_data.Remove(data.GetID());
	}
	
	EditorObjectData Get(int index)
	{
		return _data[index];
	}
}
*/
