
ref array<ref EditorListItem> m_ListItemCache = new array<ref EditorListItem>();

EditorListItem EditorListItemFromWidget(Widget w)
{
	RecursiveGetParent(w, "EditorListItem");
	foreach (ref EditorListItem list_item: m_ListItemCache) {
		if (list_item.GetRoot() == w)
			return list_item;
	}
	
	EditorPrint("EditorListItem Not Found!", LogSeverity.ERROR);
	return null;
}


void RecursiveGetParent(out ref Widget w, string name)
{
	Print(w.GetName());
	if (w.GetName() == name) 
		return;
	w = w.GetParent();
	RecursiveGetParent(w, name);
}


// maybe use widgets instead of ScriptedWidgetEventHandler
class EditorListItem: ScriptedWidgetEventHandler
{
	private int m_NestIndex;
	
	protected Widget m_Root;
	Widget GetRoot() { return m_Root; }
	
	protected Widget m_ListItemFrame;
	protected WrapSpacerWidget m_ListItemContent;
	protected WrapSpacerWidget m_ListItemChildren;
	
	protected ButtonWidget m_ListItemButton;
	protected ButtonWidget m_ListItemVisible;
	protected ButtonWidget m_ListItemCollapse;
	
	protected TextWidget m_ListItemLabel;
	protected ImageWidget m_ListItemIcon;
	
	
	protected static int COLOR_ON_SELECTED = ARGB(140,41,128,185);
	protected static int COLOR_ON_DESELECTED = ARGB(140,35,35,35);
	
	void EditorListItem()
	{
		EditorPrint("EditorListItem");
		m_Root = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/items/EditorListItem.layout", null);
		
		m_ListItemFrame					= m_Root.FindAnyWidget("EditorListItemFrame");
		m_ListItemContent				= WrapSpacerWidget.Cast(m_Root.FindAnyWidget("EditorListItemContent"));
		m_ListItemChildren				= WrapSpacerWidget.Cast(m_Root.FindAnyWidget("EditorListItemChildren"));
		
		m_ListItemButton				= ButtonWidget.Cast(m_Root.FindAnyWidget("EditorListItemButton"));
		m_ListItemVisible				= ButtonWidget.Cast(m_Root.FindAnyWidget("EditorListItemVisible"));
		m_ListItemCollapse				= ButtonWidget.Cast(m_Root.FindAnyWidget("EditorListItemCollapse"));
		
		m_ListItemLabel 				= TextWidget.Cast(m_Root.FindAnyWidget("EditorListItemLabel"));
		m_ListItemIcon 					= ImageWidget.Cast(m_Root.FindAnyWidget("EditorListItemIcon"));
		
		
		m_Root.SetHandler(this);
		
		// temp
		m_ListItemCache.Insert(this);
	}
	
	void ~EditorListItem()
	{
		EditorPrint("~EditorListItem");
		m_Root.Unlink();
	}
	

	
	void SetText(string text) 
	{
		m_ListItemLabel.SetText(text);
	}
	
	void SetIcon(string icon)
	{
		m_ListItemIcon.LoadImageFile(0, icon);
		m_ListItemIcon.SetImage(0);
		m_ListItemIcon.Update();
	}
	
	void SetColor(int color)
	{
		m_ListItemContent.SetColor(color);
		m_ListItemContent.Update();		
	}
	
	void SetNestIndex(int index)
	{
		Print("EditorListItem::SetNestIndex " + index);
		m_NestIndex = index;
		float x, y;
		m_ListItemFrame.GetSize(x, y);
		m_ListItemFrame.SetSize(290 - 15 * m_NestIndex, y);
		m_ListItemFrame.Update();
	}
	
	int GetNestIndex()
	{
		return m_NestIndex;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return true;
	}
	
	override bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		return OnListItemDropReceived(EditorListItemFromWidget(w));
	}
	
	bool OnListItemDropReceived(EditorListItem target)
	{
		return false;
	}
	
	/*
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		EditorPrint("EditorListItem::OnDrop");
		EditorListItem target = EditorListItemFromWidget(reciever);

		if (target == null) {
			EditorPrint("EditorListItem::OnDrop: Drop Failed", LogSeverity.WARNING);
			return false;
		}
		
		EditorCollapsibleListItem collapsible;
		if (CastTo(collapsible, target)) {
			collapsible.InsertListItem(this);
		}
		return super.OnDrop(w, x, y, reciever);
	}*/
	
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{	
		//Print(w);
		return true;
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		Print("EditorPlaceableListItem::OnFocus");
		SetColor(COLOR_ON_SELECTED);
		return true;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		Print("EditorPlaceableListItem::OnFocusLost");
		SetColor(COLOR_ON_DESELECTED);
		return true;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("EditorPlaceableListItem::OnClick");
		if (button == 0) {
			SetFocus(w);
		}
		
		return super.OnClick(w, x, y, button);
	}
	
}
