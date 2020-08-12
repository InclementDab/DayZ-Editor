

class EditorContextMenu: ScriptedWidgetEventHandler
{
	
	protected ref WrapSpacerWidget m_Root;
	WrapSpacerWidget GetRoot() { return m_Root; }
	
	void EditorContextMenu()
	{
		EditorPrint("EditorContextMenu");
		
		m_Root = WrapSpacerWidget.Cast(GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorContextMenu.layout", null));
		m_Root.Show(true);
		m_Root.SetHandler(this);
		

		
		
	}
	
	void AddButton(string label)
	{
		Widget button = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorDropdownElement.layout", null);
		TextWidget text = TextWidget.Cast(button.FindAnyWidget("ElementText"));
		text.SetText(label);
		m_Root.AddChild(button);
	}
	
	void Show()
	{	
		m_Root.Show(true);
		
		int x, y;
		GetCursorPos(x, y);
		m_Root.SetPos(x, y);
		
		EditorUIManager.ContextSet(this);
	}
	
	void Close()
	{
		m_Root.Show(false);
		EditorUIManager.ContextClose();
	}
	
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("MouseDown");
		return true;
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		return false;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		return false;
	}
	
	
}
