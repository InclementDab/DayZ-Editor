

class EditorContextMenuButton: ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	Widget GetRoot() { return m_Root; }
	
	protected ButtonWidget m_Button;
	protected TextWidget m_Label;
	protected ImageWidget m_Icon;
	
	string Label;
	string Action;
	string DisableCondition;
	
	Class Context;
	ref Param Params;
	

	void EditorContextMenuButton(string label, string action = "", Class context = this, Param params = null)
	{
		Label = label; Action = action; Context = context; Params = params;
		m_Root = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorContextMenuElement.layout");
		m_Button = ButtonWidget.Cast(m_Root.FindAnyWidget("ContextMenuButton"));
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
		GetEditor().GetUIManager().ContextClose();
		return super.OnClick(w, x, y, button);
	}
	
	void Show(bool show)
	{
		m_Root.Show(show);
	}
	
	void Disable(bool disable)
	{
		m_Button.Enable(!disable);
		if (disable)
			m_Button.SetAlpha(0.5);
		else m_Button.SetAlpha(1);
	}
	

}

class EditorContextMenuFolder: EditorContextMenuButton
{
	private ref array<ref EditorContextMenuButton> m_ChildrenButtons = new array<ref EditorContextMenuButton>();
	
	protected WrapSpacerWidget m_ContextMenuChildren;
	
	void EditorContextMenuFolder(string label, string action = "", Class context = this, Param params = null)
	{
		EditorPrint("EditorContextMenuFolder");
		
		m_ContextMenuChildren = WrapSpacerWidget.Cast(m_Root.FindAnyWidget("ContextMenuChildren"));
		
				
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		Print("EditorContextMenuFolder::OnMouseEnter");
		foreach (EditorContextMenuButton button: m_ChildrenButtons)
			button.Show(true);
		
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		Print("EditorContextMenuFolder::OnMouseLeave");
		foreach (EditorContextMenuButton button: m_ChildrenButtons)
			button.Show(false);
		
		
		return true;
	}
	
	
	void AddButton(EditorContextMenuButton button)
	{		
		m_ContextMenuChildren.AddChild(button.GetRoot());
		m_ChildrenButtons.Insert(button);
	}
}

class EditorContextMenuDivider: EditorContextMenuButton
{
	void EditorContextMenuDivider(string label, string action = "", Class context = this, Param params = null)
	{
		m_Root.SetSize(250, 10);
		m_Label.Show(false);
		m_Icon.Show(false);
		m_Button.SetFlags(m_Button.GetFlags() | WidgetFlags.IGNOREPOINTER);
	}
	
}


enum EditorContextMenuDirection
{
	LEFT = -1,
	RIGHT = 1,
	AUTO = 0
};

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
	

	void Show(int x = -1, int y = -1, EditorContextMenuDirection direction = EditorContextMenuDirection.AUTO)
	{	
		m_Root.Show(true);
		
		int cursor_x, cursor_y;
		
		GetCursorPos(cursor_x, cursor_y);
		if (x < 0) x = cursor_x;
		if (y < 0) y = cursor_y;
		
		/*
		float height, width;
		int cursor_x, cursor_y, screen_x, screen_y;
		GetCursorPos(cursor_x, cursor_y);
		GetScreenSize(screen_x, screen_y);
		m_Root.GetSize(width, height);
		width *= screen_x;
		height *= m_ContextButtons.Count();
		
		cursor_x = Math.Clamp(cursor_x, 0, screen_x - width);
		cursor_y = Math.Clamp(cursor_y, 0, screen_y - height);
		*/
		m_Root.SetPos(x, y);
		
		GetEditor().GetUIManager().ContextSet(this);
	}
	
	void Close()
	{
		GetEditor().GetUIManager().ContextClose();
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("EditorContextMenu::OnClick");
				
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




