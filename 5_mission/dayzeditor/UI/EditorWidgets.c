
static string layout_dir = "P:/DayZ_Server/dev/DayZEditor/Addons/Editor/Layouts/";

class EditorEvents 
{
	event void OnObjectPlaced(Object obj, vector pos);
}


class EditorUI: UIScriptedMenu
{
	static ref EditorUI m_Instance;
	static EditorUI GetInstance()
	{
		return m_Instance;
	}
	
	protected Widget m_Parent;
	
	protected Widget m_EditorUIFrame;
	protected Widget m_LeftListPanel;
	protected Widget m_RightListPanel;
	
	protected EditBoxWidget m_LeftSearchBar;
	
	protected ScrollWidget m_LeftListPanelScrollBar;
	protected WrapSpacerWidget m_LeftListPanelSpacer;	
	
	protected ScrollWidget m_RightListPanelScrollBar;
	protected WrapSpacerWidget m_RightListPanelSpacer;
		
	protected ref EditorSearchBar m_EditorSearchBar;
	
	ref array<ref EditorListItem> 	m_EditorListItems;
	static ref map<ref Object, ref EditorBrowserListItem> EditorBrowserListItems;
	static ref array<ref EditorObjectMarker> EditorObjectMarkers; // temp
		
	
	void ~EditorUI()
	{
		Print("~EditorUI");
		
	}
	
	void Show(bool state)
	{
		layoutRoot.Show(state);
	}
	
	
	override Widget Init()
	{
		Print("EditorUI::Init");
		layoutRoot 					= GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorUI.layout", m_Parent);
		
		m_EditorUIFrame 			= layoutRoot.FindAnyWidget("EditorUIFrame");
		
		m_LeftListPanel 			= layoutRoot.FindAnyWidget("LeftListPanel");
		m_RightListPanel 			= layoutRoot.FindAnyWidget("RightListPanel");

		m_LeftSearchBar 			= EditBoxWidget.Cast(layoutRoot.FindAnyWidget("LeftPanelSearchBar"));
		
		m_LeftListPanelScrollBar 	= ScrollWidget.Cast(layoutRoot.FindAnyWidget("LeftPanelScrollBar"));
		m_LeftListPanelSpacer 		= WrapSpacerWidget.Cast(layoutRoot.FindAnyWidget("LeftListPanelSpacer"));
		
		m_RightListPanelScrollBar 	= ScrollWidget.Cast(layoutRoot.FindAnyWidget("RightPanelScrollBar"));
		m_RightListPanelSpacer 		= WrapSpacerWidget.Cast(layoutRoot.FindAnyWidget("RightListPanelSpacer"));

		m_EditorSearchBar = new EditorSearchBar(m_LeftListPanelSpacer);
		m_LeftSearchBar.SetHandler(m_EditorSearchBar);
		
		EditorBrowserListItems = new ref map<ref Object, ref EditorBrowserListItem>;
		EditorObjectMarkers = new array<ref EditorObjectMarker>;
		
		layoutRoot.Show(false);
		
		RefreshItemList();
		return layoutRoot;
	}
				
	void RefreshItemList()
	{
		Print("EditorUI::ReloadItemList");		
		if (!EditorMissionGameplay.m_Editor) return;
		
		foreach (string editor_object: Editor.EditorListObjects) {	

			EditorListItem editor_list_item;
			Widget list_item = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorListItem.layout", m_LeftListPanelSpacer);
			list_item.GetScript(editor_list_item);
			editor_list_item.SetObject(editor_object);		
		}
	}
	
	
	EditorBrowserListItem GetBrowserObjectFromEntity(Object obj)
	{
		foreach (ref Object list_obj, ref EditorBrowserListItem list_item: EditorBrowserListItems) {
			if (obj == list_obj) {
				return list_item;
			}
		}
		return null;
	}
		
		
	Shape shape;
	override void Update(float timeslice)
	{
		set<Object> collisions = new set<Object>;
		vector hitpos = Editor.MousePosToRay(collisions);
		
 		if (collisions.Count() > 0) {
			for (int i = 0; i < collisions.Count(); i++) {
				
				vector min, max;
				Object o = collisions[i];
				o.GetWorldBounds(min, max);						 
				Editor.SetObjectUnderCursor(o);
				
				vector mat[4];
				o.GetTransform(mat);
				if(shape) {
					shape.Destroy();
					shape = NULL;
				}
				
				shape = Shape.Create(ShapeType.BBOX, 0x1fff7f7f, ShapeFlags.TRANSP|ShapeFlags.NOZWRITE, min, max);
				
				if (shape) {
					shape.SetMatrix(mat);
					//Print(max);
				}
			}
		} 
		
		super.Update(timeslice);
		
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		Print("EditorUI::OnDrag");
		
		return super.OnDrag(w, x, y);
	}
	
	
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorUI::OnMouseButtonDown");
		if (EditorMissionGameplay.m_Editor.IsPlacing()) {			
			EntityAI e = Editor.ObjectInHand.GetProjectionEntity();
			Editor.PlaceObject(e.GetType(), e.GetWorldPosition(), vector.Up);				

			if (!GetGame().GetInput().LocalValue("UATurbo")) {
				delete Editor.ObjectInHand;
			} 
			return true;
			
		} else {
			Editor.SetActiveObject(Editor.ObjectUnderCursor);
		}	
		
		return true;
	}
		
	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		Print("EditorUI::OnMouseWheel");
		Print(wheel);
		
		if (wheel == -1) {
			// move camera up and down
		}
		
		
		return super.OnMouseWheel(w, x, y, wheel);
	}	
	
	void OnObjectPlaced(Object obj, vector position, vector orientation)
	{
		Print("EditorUI::OnObjectPlaced");
		
		// Create Browser List Item
		EditorBrowserListItem editor_browser_item;
		Widget list_item = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorBrowserListItem.layout", m_RightListPanelSpacer);
		list_item.GetScript(editor_browser_item);
		editor_browser_item.SetObject(obj);
		EditorBrowserListItems.Insert(obj, editor_browser_item);
		
		// Create Object Marker
		EditorObjectMarker editor_object_marker;
		Widget marker_item = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorObjectMarker.layout");
		marker_item.GetScript(editor_object_marker);
		editor_object_marker.Init(obj);
		EditorObjectMarkers.Insert(editor_object_marker);
	}

}




class EditorBrowserListItem: ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected Object m_Object;
	
	protected Widget m_EditorListItemPanel;
	protected TextWidget m_EditorListItemText;	
	
	
	void ~EditorBrowserListItem()
	{
		Print("~EditorBrowserListItem");
		delete m_Root;
	}

	void SetObject(Object obj)
	{
		Print("EditorBrowserItem::SetObject");
		m_Object = obj;
		m_EditorListItemText.SetText(m_Object.GetType());
	}
	
	protected void OnWidgetScriptInit(Widget w)
	{
		Print("EditorBrowserItem::OnWidgetScriptInit");
		m_Root = w;
		m_Root.SetHandler(this);
		m_EditorListItemPanel = m_Root.FindAnyWidget("EditorListItemPanel");
		m_EditorListItemText = TextWidget.Cast(m_Root.FindAnyWidget("EditorListItemText"));
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorBrowserItem::OnMouseButtonDown");
		if (button == 0) {
			if (w == GetFocus()) return true;
			if (Editor.IsPlacing()) {
				delete Editor.ObjectInHand;
			}
			
			SetFocus(w);
			return true;
		}
		
		return false;
	}
	
	
	override bool OnFocus(Widget w, int x, int y)
	{
		Print("EditorBrowserItem::OnFocus");
		w.SetColor(COLOR_BLUE);
		Editor.SetActiveObject(m_Object);
		return true;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		Print("EditorBrowserItem::OnFocusLost");
		w.SetColor(0xFFFFFFFF);
		return true;
	}		
	
	Object GetObject()
	{
		return m_Object;
	}
	
	Widget GetLayoutRoot()
	{
		return m_EditorListItemPanel;
	}
	
}




static EntityAI CreateDummyEntityAI(string name)
{	
	Object o = GetGame().CreateObject(name, "0 0 0");
	EntityAI e = EntityAI.Cast(o);
	GetGame().ObjectDelete(o);
	return e;
}

class EditorListItem: ScriptedWidgetEventHandler
{
	
	protected Widget m_Root;
	private string m_ObjectName;
	protected Widget m_EditorListItemPanel;
	protected TextWidget m_EditorListItemText;	

	protected Widget m_EditorListItemTooltip;
	protected TextWidget m_EditorListItemTooltipTitle;
	protected ItemPreviewWidget m_EditorListItemTooltipPreview;
	
	protected EntityAI m_ItemPreview;
		
	
	void EditorListItem()
	{
		//Print("EditorListItem");
	}
	
	void ~EditorListItem()
	{
		//Print("~EditorListItem");
		delete m_EditorListItemTooltip;
		delete m_Root;
	}
	
	protected void OnWidgetScriptInit(Widget w)
	{
		//Print("EditorListItem::OnWidgetScriptInit");
		m_Root = w;
		m_Root.SetHandler(this);
		
		m_EditorListItemPanel 				= m_Root.FindAnyWidget("EditorListItemPanel");
		m_EditorListItemText 				= TextWidget.Cast(m_Root.FindAnyWidget("EditorListItemText"));
		
		m_EditorListItemTooltip 			= GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorListItemTooltip.layout");						
		m_EditorListItemTooltipTitle 		= TextWidget.Cast(m_EditorListItemTooltip.FindAnyWidget("EditorListItemTooltipTitle"));
		m_EditorListItemTooltipPreview 		= ItemPreviewWidget.Cast(m_EditorListItemTooltip.FindAnyWidget("EditorListItemTooltipPreview"));
		
	}		
	
	
	
	void SetObject(string name)
	{
		m_ObjectName = name;
		m_EditorListItemText.SetText(m_ObjectName);
		m_ItemPreview = GetGame().CreateObject(m_ObjectName, "0 0 0");
		m_EditorListItemTooltipTitle.SetText(m_ObjectName);
		m_EditorListItemTooltipPreview.SetItem(m_ItemPreview);
		//m_EditorListItemTooltipPreview.SetModelPosition("0 0 0");
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		Print("EditorListItem::OnFocus");
		w.SetColor(COLOR_BLUE);
		
		Editor.CreateObjectInHand(m_ObjectName);		
		
		return true;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		Print("EditorListItem::OnFocusLost");
		w.SetColor(0xFFFFFFFF);
		return true;
	}		
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorListItem::OnMouseButtonDown");
		if (button == 0) {
			if (w == GetFocus()) return true;
			if (Editor.IsPlacing()) {
				delete Editor.ObjectInHand;
			}
			
			SetFocus(w);
			return true;
		}
		
		return false;
	}
	
		
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		m_EditorListItemTooltip.SetPos(x, y);
		m_EditorListItemTooltip.Show(true);
		m_EditorListItemTooltipPreview.Update();
		
		return true;
	}	
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		m_EditorListItemTooltip.Show(false);
		
		//delete m_EditorListItemTooltip;
		return true;
	}
	
	string GetObjectName()
	{
		return m_ObjectName;
	}
}




class EditorSearchBar: ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected WrapSpacerWidget m_SearchContext;
	
	void EditorSearchBar(WrapSpacerWidget searchContext)
	{
		m_SearchContext = searchContext;
	}
		
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		Print("EditorSearchBar::OnChange");
		string filter = EditBoxWidget.Cast(w).GetText();
		
		Widget child = m_SearchContext.GetChildren();
		while (child != null) {
			EditorListItem editor_list_item;
			child.GetScript(editor_list_item);
			
			if (editor_list_item != null) {
				string Config_Lower = editor_list_item.GetObjectName();
				Config_Lower.ToLower();
	           	child.Show(!(filter != "" && !Config_Lower.Contains(filter)));				
			}
						
			child = child.GetSibling();
		}
		
		return super.OnChange(w, x, y, finished);
	}
	
}
