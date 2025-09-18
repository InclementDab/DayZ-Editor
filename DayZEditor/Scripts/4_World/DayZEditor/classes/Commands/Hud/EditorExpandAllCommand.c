[RegisterEditorCommand(EditorExpandAllCommand)]
class EditorExpandAllCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		Param1<EditorListNode> data = Param1<EditorListNode>.Cast(GetData());
		if (!data) {
			return false;
		}
		
		ExpandAll(data.param1);
		
		return true;
	}
	
	protected void ExpandAll(notnull EditorListNode list_node)
	{
		list_node.ExpandAll();
	}
	
	override string GetName() 
	{
		return "Expand All";
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.SQUARE_PLUS;
	}
}

[RegisterEditorCommand(EditorCollapseAllCommand)]
class EditorCollapseAllCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		Param1<EditorListNode> data = Param1<EditorListNode>.Cast(GetData());
		if (!data) {
			return false;
		}
		
		CollapseAll(data.param1);
		
		return true;
	}
	
	protected void CollapseAll(notnull EditorListNode list_node)
	{		
		list_node.CollapseAll();
	}
	
	override string GetName() 
	{
		return "Collapse All";
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.SQUARE_MINUS;
	}
}