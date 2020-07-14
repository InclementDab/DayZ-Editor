
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
	WrapSpacerWidget GetLeftListPanelSpacer() { return m_LeftListPanelSpacer; }
	
	protected ScrollWidget m_RightListPanelScrollBar;
	protected WrapSpacerWidget m_RightListPanelSpacer;
		
	protected ref EditorSearchBar m_EditorSearchBar;
	static ref array<ref EditorListItem>	EditorListItems;
		
	
	void EditorUI()
	{
		Print("EditorUI");
	}
	
	void ~EditorUI()
	{
		Print("~EditorUI");
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
		EditorListItems 			= new array<ref EditorListItem>;
		LoadItems();
		
		layoutRoot.Show(true);
		m_Instance = this;
		return layoutRoot;
	}
	
	
	void Show(bool state)
	{
		layoutRoot.Show(state);
	}
	
	void LoadItems()
	{
		Print("LoadItems");
		foreach (string item: Editor.EditorListObjects) {
			Print(item);
			EditorListItem editor_list_item;
			Widget list_item = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorListItem.layout", m_LeftListPanelSpacer);
			list_item.GetScript(editor_list_item);
			editor_list_item.SetObject(item);
			EditorListItems.Insert(editor_list_item);
		}
	}

	
	
	Widget GetBrowserObjectFromEntity(Object obj)
	{
		foreach (EditorObject list_item: Editor.EditorPlacedObjects) {
			if (obj == list_item.GetWorldObject()) {
				return list_item.GetLayoutRoot();
			}
		}
		return null;
	}
		
	override void Update(float timeslice)
	{

 		
		
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
						Editor.SelectObject(Editor.ObjectUnderCursor);
			}	
		}
		return false;
	}
	
	
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		Editor.CursorAllowedToSelect = true; // this is stupid
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
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
		Editor.EditorPlacedObjects.Insert(editor_object);
	}

	EditorObject CreateEditorObjectFromExisting(Object obj)
	{		
		return null;
		Print("EditorUI::CreateEditorObjectFromExisting");
		EditorObject editor_object;
		Widget editor_object_display = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorObjectMarker.layout");
		editor_object_display.GetScript(editor_object);
		m_RightListPanelSpacer.AddChild(editor_object.Initialize(obj));
		Editor.EditorPlacedObjects.Insert(editor_object);
		return editor_object;
	}
}






