class EditorWearableItem
{
	string Type;
	string DisplayName;
	ref TStringArray Slots = {};
	
	void EditorWearableItem(string type, string display_name, TStringArray slots)
	{
		Type = type;
		DisplayName = display_name;
		Slots.Copy(slots);
	}
}