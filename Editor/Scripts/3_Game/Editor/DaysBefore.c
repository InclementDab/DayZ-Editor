class ConfigObjectEntry: ConfigObject
{
	[ConfigEntryAttribute("scope")]
	int Scope;
	
	[ConfigEntryAttribute("model")]
	string Model;
}

modded class DaysBefore
{
	static const string EDITORS = "Editors";
	
	void DaysBefore(string path)
	{
		Root.Add(new NamedNode(EDITORS, "Editors", Symbols.PEOPLE_SIMPLE, LinearColor.WHITE));
	}
		
	static Node GetEditors()
	{
		return Root[EDITORS];
	}
		
	static EditorNode GetEditor()
	{
		return EditorNode.Cast(Root[EDITORS][GetGame().GetUserManager().GetTitleInitiator().GetUid()]);
	}
}