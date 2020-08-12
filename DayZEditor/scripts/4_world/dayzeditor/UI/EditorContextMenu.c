

class EditorContextMenuButton: ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	Widget GetRoot() { return m_Root; }
	
	protected TextWidget m_Label;
	protected ImageWidget m_Icon;
	
	string Label;
	string Action;
	
	Class Context;
	ref Param Params;


	void EditorContextMenuButton(string label, string action = "", Class context = this, Param params = null)
	{
		Label = label; Action = action; Context = context; Params = params;
		m_Root = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorContextMenuElement.layout");
		m_Label = TextWidget.Cast(m_Root.FindAnyWidget("ContextMenuLabel"));
		m_Icon = ImageWidget.Cast(m_Root.FindAnyWidget("ContextMenuIcon"));
		m_Root.SetHandler(this);
		
		m_Label.SetText(label);
	}
	
		
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("EditorContextMenuButton::OnClick");
		
		if (button != 0) return false;
		
		GetGame().GameScript.Call(Context, Action, Params);	
		
		return true;
	}
	
	void Show(bool show)
	{
		m_Root.Show(show);
	}

}

class EditorContextMenuFolder: EditorContextMenuButton
{
	private ref array<ref EditorContextMenuButton> m_ChildrenButtons = new array<ref EditorContextMenuButton>();
	
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		Print("EditorContextMenuFolder::OnMouseEnter");
		foreach (EditorContextMenuButton button: m_ChildrenButtons) {
			button.Show(true);
		}
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		Print("EditorContextMenuFolder::OnMouseLeave");
		foreach (EditorContextMenuButton button: m_ChildrenButtons) {
			button.Show(false);
		}
		
		return true;
	}
	
	
	void AddButton(EditorContextMenuButton button)
	{
		float x, y;
		button.GetRoot().GetPos(x, y);
		button.GetRoot().SetPos(x + 250, y);
		
		m_Root.AddChild(button.GetRoot());
		m_ChildrenButtons.Insert(button);
		
	}
	


}



class EditorContextMenu: ScriptedWidgetEventHandler
{
	
	protected ref WrapSpacerWidget m_Root;
	WrapSpacerWidget GetRoot() { return m_Root; }
	
	private ref array<ref EditorContextMenuButton> m_ContextButtons;
	
	void EditorContextMenu()
	{
		EditorPrint("EditorContextMenu");
		
		m_Root = WrapSpacerWidget.Cast(GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorContextMenu.layout", null));
		m_Root.Show(true);
		m_Root.SetHandler(this);
		m_ContextButtons = new array<ref EditorContextMenuButton>();
	}
	
	void AddButton(EditorContextMenuButton button)
	{		
		m_Root.AddChild(button.GetRoot());
		m_ContextButtons.Insert(button);
	}
	
	void Show()
	{	
		m_Root.Show(true);
		
		float height, width;
		int cursor_x, cursor_y, screen_x, screen_y;
		GetCursorPos(cursor_x, cursor_y);
		GetScreenSize(screen_x, screen_y);
		m_Root.GetSize(width, height);
		width *= screen_x;
		height *= m_ContextButtons.Count();
		
		cursor_x = Math.Clamp(cursor_x, 0, screen_x - width);
		cursor_y = Math.Clamp(cursor_y, 0, screen_y - height);
		
		m_Root.SetPos(cursor_x, cursor_y);
		
		EditorUIManager.ContextSet(this);
	}
	
	void Close()
	{
		EditorUIManager.ContextClose();
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("EditorContextMenu::OnClick");
		if (button != 0) return true;
		

				
		return false;
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
