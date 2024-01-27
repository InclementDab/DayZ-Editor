class EditorServer: EditorNode
{
	static const ref array<string> CATEGORIES = { "Unknown", "Plants", "Rocks", "Clutter", "Structures", "Wrecks", "AI", "Water", "Vehicles", "StaticObjects", "DynamicObjects", "ScriptedObjects" };
	static const int DEFAULT_ENTITY_COUNT = 512;
	
	void EditorServer(string uuid, string display_name, Symbols icon)
	{

	}
	
	void ~EditorServer()
	{
		if (GetGame() && GetGame().GetUpdateQueue(CALL_CATEGORY_GUI)) {
			GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		}
	}
	
	void Update(float timeslice)
	{
	}
}