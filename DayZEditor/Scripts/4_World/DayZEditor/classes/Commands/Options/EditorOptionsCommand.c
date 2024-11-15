[RegisterEditorCommand(EditorOptionsCommand)]
class EditorOptionsCommand: EditorCommand
{
   	override bool Execute(Class sender, CommandArgs args) 
	{
        super.Execute(sender, args);

        UIScriptedMenu menu = GetGame().GetUIManager().EnterScriptedMenu(MENU_OPTIONS, null);
		
		int tab_index = -1;
		g_Script.CallFunction(menu, "FindTabIndex", tab_index, "EDITOR"); // this wouldve originally been localized but its lowercase and ugly
        if (tab_index != -1) {
            g_Script.CallFunction(menu, "SetOpenTab", null, tab_index);
        }

        return true;
    }

    override Symbols GetSymbol()
    {
        return Symbols.GEAR;
    }

    override string GetName()
    {
        return "Options";
    }
}