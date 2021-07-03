class EditorInventoryEditorController: ViewController
{
	PlayerBase Player;
	
	void EditorInventoryEditorController()
	{
		PlayerBase.Cast(GetGame().GetPlayer());
	}
}

class EditorInventoryEditorHud: ScriptViewTemplate<EditorInventoryEditorController>
{
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/Inventory/Inventory.layout";
	}
}