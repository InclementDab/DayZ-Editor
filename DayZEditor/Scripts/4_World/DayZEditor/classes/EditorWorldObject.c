class EditorWorldObject
{
	static const ref array<string> VALID_PATHS = { 
		"DZ\\plants", 
		"DZ\\plants_bliss",
		"DZ\\rocks",
		"DZ\\rocks_bliss",
	};
	
	protected Object m_WorldObject;
	Object GetWorldObject() 
	{
		return m_WorldObject;
	}
	
	void ~EditorWorldObject()
	{
		GetGame().ObjectDelete(m_WorldObject);	
	}
}