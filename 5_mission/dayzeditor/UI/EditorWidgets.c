
static string layout_dir = "P:/DayZ_Server/dev/DayZEditor/Addons/Editor/Layouts/";




class EditorEvents 
{
	event void OnObjectPlaced(Object obj, vector pos);
}

class EditorContextMenu: ScriptedWidgetEventHandler
{
	static ref EditorContextMenu instance;
	protected Widget m_Root;
	
	protected void OnWidgetScriptInit(Widget w)
	{
		Print("EditorObjectMarkerHandler::OnWidgetScriptInit");
		m_Root = w;
		m_Root.SetHandler(this);
	
		instance = this;
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (Editor.CurrentSelectedObject == null) return false;
		
		switch (w.GetName()) {
			case ("CtxProperties"):
				break;
			
			case ("CtxAlignToGround"):
				vector v1 = Editor.CurrentSelectedObject.GetPosition();	
				vector v2 = GetGame().GetSurfaceOrientation(v1[0], v1[2]);
				Editor.CurrentSelectedObject.SetPosition(v2);
			
			
				break;
			
			case ("CtxDelete"):
				//Editor.DeleteObject(Editor.CurrentSelectedObject);
				break;
		}
		
		return true;
	}
	
	static void ShowContextMenu(int x, int y)
	{
		instance.m_Root.SetPos(x, y);
		instance.m_Root.Show(true);
	}
	
	static void HideContextMenu()
	{
		instance.m_Root.Show(false);
	}
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
	static ref array<ref EditorObject> EditorPlacedObjects;
		
	
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
				
		EditorPlacedObjects = new ref array<ref EditorObject>;
		
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
	
	
	Widget GetBrowserObjectFromEntity(Object obj)
	{
		foreach (EditorObject list_item: EditorPlacedObjects) {
			if (obj == list_item.GetWorldObject()) {
				return list_item.GetLayoutRoot();
			}
		}
		return null;
	}
		
	override void Update(float timeslice)
	{
		Input input = GetGame().GetInput();
		if (input.LocalPress("UATempRaiseWeapon")) {
			int x, y;
			GetMousePos(x, y);			
			EditorContextMenu.ShowContextMenu(x, y);
		}
 		
		
		super.Update(timeslice);
		
	}

	
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorUI::OnMouseButtonDown");
		
			if (button == 0) {
				EditorContextMenu.HideContextMenu();
				if (w == GetFocus()) return true;
				
				if (w.GetName() == "EditorListItemPanel") { // idk why but i cant use OnMouseButtonDown in editorobject
					if (Editor.IsPlacing())
						delete Editor.ObjectInHand;
					
					SetFocus(w.GetParent());
					return true;
				}
				
				if (Editor.IsPlacing()) {
					EntityAI e = Editor.ObjectInHand.GetProjectionEntity();
					Editor.PlaceObject(e.GetType(), e.GetWorldPosition(), vector.Up);				
		
					if (!GetGame().GetInput().LocalValue("UATurbo")) {
						delete Editor.ObjectInHand;
					} 
					return true;
					
				} else {
					//return false; // todo: check if object under cursor is one we placed
					if (Editor.ObjectUnderCursor != null)
						Editor.SetActiveObject(Editor.ObjectUnderCursor);
			}	
		}
		return false;
	}
	
	
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		Print("EditorUI::OnMouseEnter");
		Editor.CursorAllowedToSelect = true; // this is stupid
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		Print("EditorUI::OnMouseLeave");
		Editor.CursorAllowedToSelect = false;
		return true;
	}
		
		
	void OnObjectPlaced(Object obj, vector position, vector orientation)
	{
		Print("EditorUI::OnObjectPlaced");
				
		EditorObject editor_object;
		Widget editor_object_display = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorObjectMarker.layout");
		editor_object_display.GetScript(editor_object);
		m_RightListPanelSpacer.AddChild(editor_object.Initialize(obj));
		EditorPlacedObjects.Insert(editor_object);
	}

	EditorObject CreateEditorObjectFromExisting(Object obj)
	{		
		return null;
		Print("EditorUI::CreateEditorObjectFromExisting");
		EditorObject editor_object;
		Widget editor_object_display = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorObjectMarker.layout");
		editor_object_display.GetScript(editor_object);
		m_RightListPanelSpacer.AddChild(editor_object.Initialize(obj));
		EditorPlacedObjects.Insert(editor_object);
		return editor_object;
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


class EditorUICartesian: ScriptedWidgetEventHandler
{
	static ItemPreviewWidget m_Root;
	//protected ItemPreviewWidget m_EditorUICartesian;
	
	static EntityAI m_UICartiesian;
	
	void EditorUICartesian()
	{
		Print("EditorUICartesian");
		m_UICartiesian = GetGame().CreateObject("BoundingBox", "0 0 0");
		m_UICartiesian.SetOrientation(vector.Up);
		m_UICartiesian.SetOrigin(vector.Zero);
	}
	
	void ~EditorUICartesian()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		GetGame().ObjectDelete(m_UICartiesian);
	}
	
	protected void OnWidgetScriptInit(Widget w)
	{
		Print("EditorUICartesian::OnWidgetScriptInit");
		m_Root = ItemPreviewWidget.Cast(w);
		m_Root.SetHandler(this);
		m_Root.SetItem(m_UICartiesian);
		m_Root.SetForceFlipEnable(true);
		m_Root.SetForceFlip(true);
		m_Root.SetView(0);
		//m_Root.SetView(m_Root.GetItem().GetViewIndex());
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void Update()
	{
		//vector v = GetGame().GetCurrentCameraDirection();	
		//m_Root.SetModelOrientation(v);
	}
	
	
}
