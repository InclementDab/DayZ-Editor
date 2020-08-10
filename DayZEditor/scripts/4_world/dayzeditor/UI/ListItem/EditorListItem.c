
ref array<ref EditorListItem> m_ListItemCache = new array<ref EditorListItem>();

EditorListItem EditorListItemFromWidget(Widget w)
{
	
	foreach (ref EditorListItem list_item: m_ListItemCache) {
		if (list_item.GetRoot() == w)
			return list_item;
	}
	
	Print(m_ListItemCache.Count());
	return null;
}



// maybe use widgets instead of ScriptedWidgetEventHandler
class EditorListItem: ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	Widget GetRoot() { return m_Root; }
		
	protected TextWidget m_ListItemText;
	protected ImageWidget m_ListItemIcon;
	
	protected WrapSpacerWidget m_EditorListCategoryHeader;
	
	protected static int COLOR_ON_SELECTED = ARGB(140,41,128,185);
	protected static int COLOR_ON_DESELECTED = ARGB(140,35,35,35);
	
	void EditorListItem()
	{
		EditorPrint("EditorListItem");
		m_Root = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/items/EditorCategory.layout", null);
		
		m_ListItemText 					= TextWidget.Cast(m_Root.FindAnyWidget("EditorListCategoryHeaderLable"));
		m_ListItemIcon 					= ImageWidget.Cast(m_Root.FindAnyWidget("EditorListCategoryHeaderIcon"));
		m_EditorListCategoryHeader 		= WrapSpacerWidget.Cast(m_Root.FindAnyWidget("EditorListCategoryHeader"));
		
		
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
		m_ListItemText.SetText(text);
	}
	
	void SetIcon(string icon)
	{
		m_ListItemIcon.LoadImageFile(0, icon);
		m_ListItemIcon.SetImage(0);
		m_ListItemIcon.Update();
	}
	
	void SetColor(int color)
	{
		m_EditorListCategoryHeader.SetColor(color);
		m_EditorListCategoryHeader.Update();		
	}
	
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return true;
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		Print("DRAG TIME WEEEE");
		return super.OnDrag(w, x, y);
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		
		EditorListItem target = EditorListItemFromWidget(reciever.GetParent().GetParent());
		if (target == null) {
			Print("Drop failed. was null");
			return false;
		}
		
		EditorCollapsibleListItem collapsible;
		if (CastTo(collapsible, target)) {
			collapsible.InsertListItem(EditorListItemFromWidget(w.GetParent().GetParent()));
		}
		return true;
	}
	
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{	
		Print(w);
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
			return true;
		}

		
		return false;
	}
	
}



class EditorPlacedListItem: EditorListItem
{
	private ref EditorObjectData m_Data;
	EditorObjectData GetData() { return m_Data; }
	
	void EditorPlacedListItem(EditorObjectData data)
	{
		Print("EditorPlacedListItem");
		m_Data = data;
		
		SetText(m_Data.Type);
		SetIcon(Editor.GetIconFromMod(Editor.GetModFromObject(m_Data.Type)));
		
		EditorEvents.OnObjectSelected.Insert(EditorObjectSelected);
		EditorEvents.OnObjectDeselected.Insert(EditorObjectDeselected);	
	}
	
	void EditorObjectSelected(Class context, EditorObject target)
	{
		if (target.GetID() == m_Data.GetID())
			SetColor(COLOR_ON_SELECTED);
		
	}
	
	void EditorObjectDeselected(Class context, EditorObject target)
	{		
		if (target.GetID() == m_Data.GetID())
			SetColor(COLOR_ON_DESELECTED); 

	}
	
	
	
}

class EditorPlaceableListItem: EditorListItem
{
	private ref EditorPlaceableObjectData m_Data;
	EditorPlaceableObjectData GetData() { return m_Data; }

	
	void EditorPlaceableListItem(EditorPlaceableObjectData data)
	{
		Print("EditorPlaceableListItem");
		m_Data = data;
		
		SetText(m_Data.Type);
		SetIcon(Editor.GetIconFromMod(Editor.GetModFromObject(m_Data.Type)));
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return true;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("EditorPlaceableListItem::OnClick");
		
		if (button == 0) {
			GetEditor().CreateObjectInHand(m_Data.Type);
			SetFocus(w);
			return true;
		} else if (button == 1) {
			
			if (GetGame().GetInput().LocalValue("UAWalkRunTemp")) {				
				// all very temporary please abstract elsewhere
				if (GetEditor().IsLootEditActive())
					GetEditor().PlaceholderRemoveLootMode();
				else 
					GetEditor().PlaceholderForEditLootSpawns(m_Data.Type);
				
				return true;				
			}
		}

		
		return false;
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
	
}


class EditorCollapsibleListItem: EditorListItem
{
	private ref array<ref EditorListItem> m_CategoryChildren;
	
	protected WrapSpacerWidget m_EditorListCategoryContent;
	
	void EditorCollapsibleListItem()
	{
		Print("EditorCollapsibleListItem");
		m_CategoryChildren = new array<ref EditorListItem>();
		m_EditorListCategoryContent = WrapSpacerWidget.Cast(m_Root.FindAnyWidget("EditorListCategoryContent"));
		SetText("group0");
	}
	
	void ~EditorCollapsibleListItem()
	{
		Print("~EditorCollapsibleListItem");
	}
	
	void InsertListItem(EditorListItem item)
	{
		Print("InsertListItem");
		m_CategoryChildren.Insert(item);
		m_EditorListCategoryContent.AddChild(item.GetRoot());
	}
	
	void RemoveListItem(EditorListItem item)
	{
		int index = m_CategoryChildren.Find(item);
		if (index == -1) return;
		m_CategoryChildren.Remove(index);
	}
	
	
	private bool temp;
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("EditorCollapsibleListItem::OnClick");
		

		if (button == 1) {
			temp = !temp;
			m_EditorListCategoryContent.Show(temp);
		}

		
		return super.OnClick(w, x, y, button);
	}
	
	
}









































/*


class EditorListItemTooltip extends ScriptedWidgetEventHandler
{
	private Widget m_Root;
	private TextWidget m_Title;
	private ItemPreviewWidget m_Preview;

	private ref Timer	m_ToolTipUpdateTimer;
	private ref EditorPlaceableObjectData m_PlaceableObject;
	private EntityAI m_PreviewObj;
	
	void EditorListItemTooltip(EditorPlaceableObjectData obj)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItemTooltip::EditorListItemTooltip - Start");
		#endif
		
		m_Root = Widget.Cast( GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/layouts/EditorListItemTooltip.layout") );
		m_Title = TextWidget.Cast(m_Root.FindAnyWidget("EditorListItemTooltipTitle"));
		m_Preview = ItemPreviewWidget.Cast(m_Root.FindAnyWidget("EditorListItemTooltipPreview"));

		m_PlaceableObject = obj;
		
		m_Root.SetHandler( this );
		
		#ifdef EDITORPRINT
		EditorPrint("EditorListItemTooltip::EditorListItemTooltip - End");
		#endif
	}
	
	void ~EditorListItemTooltip()
	{
		HideTooltip();
		delete m_Root;
	}
		
	void SetToolTip()
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItemTooltip::SetToolTip - Start");
		#endif
		
		if(m_PlaceableObject)
		{
			m_Title.SetText(m_PlaceableObject.GetType());
			m_PreviewObj = EntityAI.Cast(GetGame().CreateObjectEx(m_PlaceableObject.GetType(), vector.Zero, ECE_NONE));
			
			#ifdef EDITORPRINT
			EditorPrint("EditorListItemTooltip::SetToolTip - m_PreviewObj: " + m_PreviewObj);
			#endif
			
			m_Preview.SetItem(m_PreviewObj);
			m_Preview.SetView(m_Preview.GetItem().GetViewIndex());
		}
		
		#ifdef EDITORPRINT
		EditorPrint("EditorListItemTooltip::SetToolTip - End");
		#endif
	}
	
	void SetPos(int x, int y)
	{
		m_Root.SetPos(x, y);
	}
		
	void ShowTooltip()
	{		
		if (!m_ToolTipUpdateTimer)
		{
			m_ToolTipUpdateTimer = new Timer();
			m_ToolTipUpdateTimer.Run( 0.01, this, "Update", NULL, true ); // Call Update all 0.01 seconds
		}
		
		m_Root.Show( true );
		
		SetToolTip();
		
		int x, y;
		GetMousePos(x,y);
		m_Root.SetPos(x, y);
		
		m_Root.Update();
	}
		
	void HideTooltip()
	{		
		if (m_ToolTipUpdateTimer)
			m_ToolTipUpdateTimer.Stop();
		
		m_Root.Show(false);
		m_Root.SetPos(0, 0);
		//m_Title.SetText("");
		
		if(m_PreviewObj)
		{
			m_Preview.SetItem(null);
			GetGame().ObjectDelete(m_PreviewObj);
		}
	}
	
	void SetColor(int color)
	{		
		m_Root.SetColor( color );
	}
	
	void SetTextColor(int color)
	{		
		m_Title.SetColor( color );
	}
	
	void Update( float timeslice )
	{		
		if ( m_Root && m_Root.IsVisible() ) 
			UpdateTooltip();
	}
	
	void UpdateTooltip()
	{
		int mouse_x;
		int mouse_y;
			
		GetGame().GetMousePos( mouse_x, mouse_y );
		m_Root.SetPos( mouse_x, mouse_y );
		m_Root.Update();
	}
	
	bool IsVisable()
	{
		return m_Root.IsVisible();
	}
}
*/
