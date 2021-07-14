static int DeletedObjectId;
class EditorDeletedObjectData
{
	[NonSerialized()]
	int ID;
	
	string Type;
	vector Position;
	int Flags;
	
	[NonSerialized()]
	Object WorldObject;
	
	private void EditorDeletedObjectData()
	{
		DeletedObjectId++;
		ID = DeletedObjectId;
	}
	
	static EditorDeletedObjectData Create(string type, vector position)
	{		
		EditorDeletedObjectData data();
		data.Type = type;
		data.Position = position;
		
		// todo handle multiple objects of the same name in a 1m radius
		data.WorldObject = data.FindObject(0.05);
		return data;
	}
	
	static EditorDeletedObjectData Create(Object object)
	{
		if (!object) {
			return null;
		}
		
		EditorDeletedObjectData data();
		data.Type = object.GetType();
		data.Position = object.GetWorldPosition();
		data.WorldObject = object;
		return data;
	}
	
	Object FindObject(float radius = 0.05)
	{
		array<Object> objects = {};
		array<CargoBase> cargos = {};
		GetGame().GetObjectsAtPosition3D(Position, radius, objects, cargos);
		
		foreach (Object object: objects) {
			if (object.GetType() == Type) {
				return object;
			}
		}
		
		return null;
	}
}