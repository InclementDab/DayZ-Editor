class EditorPlaceableObjectData: SerializableBase
{	
	Object CreateObject(vector transform[4], int flags = ECE_LOCAL)	
	{
		return null;
	}
	
	EditorObjectDataCategory GetCategory()
	{
		return EditorObjectDataCategory.UNKNOWN;
	}
					
	string GetName()
	{
		return string.Empty;
	}
	
	string GetModel()
	{
		return string.Empty;
	}
	
	string GetIcon()
	{
		return "set:regular icon:tree";
	}
}