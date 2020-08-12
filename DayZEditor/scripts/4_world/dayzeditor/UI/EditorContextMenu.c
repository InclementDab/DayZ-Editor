
enum EditorContextMenuType 
{
	DEFAULT,
	FOLDER,
	DIVIDER
	
};


class EditorContextMenuButtonData
{
	string Label;
	string Action;
	
	Class Context;
	ref Param Params;
	
	private EditorContextMenuType Type;
	EditorContextMenuType GetType();
	
	private ref map<Widget, ref EditorContextMenuButtonData> m_ChildrenButtons;
	
	private void EditorContextMenuButtonData(string label, string action = "", Class context = this, Param params = null)
	{
		Label = label; Action = action; Context = context; Params = params;
		m_ChildrenButtons = new map<Widget, ref EditorContextMenuButtonData>();
	}
	
	static EditorContextMenuButtonData Create(string label, string action, Class context, Param params = null)
	{
		return new EditorContextMenuButtonData(label, action, context, params);
	}
	
	static EditorContextMenuButtonData CreateFolder(string label) 
	{
		EditorContextMenuButtonData data = new EditorContextMenuButtonData(label, "");
		
		data.Type = EditorContextMenuType.FOLDER;
		
		return data;
	}
	
	static EditorContextMenuButtonData CreateDivider()
	{	
		EditorContextMenuButtonData data = new EditorContextMenuButtonData("", "");
		data.Type = EditorContextMenuType.DIVIDER;
		return data;
	}
	
	void AddToFolder(Widget w, EditorContextMenuButtonData data)
	{
		data.Context = this;
		data.Action = "OnFolderClick";
		data.Params = new Param1<Widget>(w);
		float x, y, sizex, sizey;
		w.GetPos(x, y);
		w.GetSize(sizex, sizey);
		w.SetPos(x + sizex, y);
		w.Show(false);
		m_ChildrenButtons.Insert(w, data);
	}
	
	private void OnFolderClick(Widget w)
	{
		Print("FolderClick");
		w.Show(true);
	}
}



class EditorContextMenu: ScriptedWidgetEventHandler
{
	
	protected ref WrapSpacerWidget m_Root;
	WrapSpacerWidget GetRoot() { return m_Root; }
	
	private ref map<Widget, ref EditorContextMenuButtonData> m_ContextButtons;
	
	void EditorContextMenu()
	{
		EditorPrint("EditorContextMenu");
		
		m_Root = WrapSpacerWidget.Cast(GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorContextMenu.layout", null));
		m_Root.Show(true);
		m_Root.SetHandler(this);
		m_ContextButtons = new map<Widget, ref EditorContextMenuButtonData>();
	}
	
	void AddButton(EditorContextMenuButtonData data)
	{

		Widget button = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorDropdownElement.layout", m_Root);
		
		switch (data.GetType()) {
			
			case EditorContextMenuType.DEFAULT: {
				TextWidget text = TextWidget.Cast(button.FindAnyWidget("ElementText"));
				text.SetText(data.Label);
				break;
			}
			
			case EditorContextMenuType.FOLDER: {
				text = TextWidget.Cast(button.FindAnyWidget("ElementText"));
				text.SetText(data.Label);
				break;
			}
			
			case EditorContextMenuType.DIVIDER: {
				text = TextWidget.Cast(button.FindAnyWidget("ElementDivider"));
				text.Show(true);
				break;
			}
			
			
			
		}
		

		
		button.SetUserData(data);
		m_ContextButtons.Insert(button, data);
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
		

		EditorContextMenuButtonData data = m_ContextButtons.Get(w);
		if (data != null) {
			GetGame().GameScript.Call(data.Context, data.Action, data.Params);
			Close();
			return true;
		}
				
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
