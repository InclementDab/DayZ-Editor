
class EditorContextMenu: UILinkedObject
{
	
	void EditorContextMenu()
	{
		Print("EditorContextMenu");
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		switch (w.GetName()) {
			
			
			case "SetTargetButton": {
				Editor.SetCameraTarget(m_EditorObject);
				m_Root.Show(false);
				return true;
			}
			
		}
		
		
		return false;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		m_Root.Show(false);
		return true;
	}
	
}



class EditorMap: EditorWidgetEventHandler
{

	void EditorMap()
	{
		Print("EditorMap");
				
	}
	
	void ~EditorMap()
	{
		Print("~EditorMap");
	}
	
	void OnObjectCreated(Class context, EditorObject obj)
	{
		MapWidget map_widget = GetMapWidget();
		map_widget.AddChild(obj.GetMapMarker());
	}
	
	
	private int start_x, start_y;
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Input input = GetGame().GetInput();	
		MapWidget map_widget = GetMapWidget();
		
		if (button == 0) {
			if (Editor.IsPlacing()) {
				EntityAI e = Editor.ObjectInHand.GetProjectionEntity();
				vector mat[4];
				e.GetTransform(mat);
				EditorObject editor_object = Editor.CreateObject(e.GetType(), mat);
				editor_object.Select();
				if (!input.LocalValue("UATurbo")) delete Editor.ObjectInHand;
				return true;
			} else {
				EditorUI.EditorCanvas.Clear();
				EditorUI ui = EditorUI.GetInstance();
				GetCursorPos(ui.start_x, ui.start_y);
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(ui.DelayedDragBoxCheck, 40);
				return true;
			}
			
		} else if (button == 2) {
			vector teleport_dest = map_widget.ScreenToMap(Vector(x, y, 0));
			vector current_pos = Editor.ActiveCamera.GetPosition();
			teleport_dest[1] = current_pos[1] - GetGame().SurfaceY(current_pos[0], current_pos[2]) + GetGame().SurfaceY(teleport_dest[0], teleport_dest[2]);
			Editor.ActiveCamera.SetPosition(teleport_dest);			
			
			return true;
			
		}
		
		return false;
	}
	
	MapWidget GetMapWidget() { return MapWidget.Cast(m_Root); }
}



class EditorListItem: EditorWidgetEventHandler
{
	protected TextWidget 	m_EditorListItemText;
	protected string 		m_Name;
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		m_EditorListItemText = m_Root.FindAnyWidget("EditorListItemText");
	}
	
	void SetObject(string name)
	{
		m_Name = name;
		m_EditorListItemText.SetText(m_Name);
		m_EditorListItemText.Update();
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
	
	override bool OnFocus(Widget w, int x, int y)
	{
		Print("EditorListItem::OnFocus");
		w.SetColor(ARGB(90, 191, 95, 95));
		Editor.CreateObjectInHand(m_Name);		
		return true;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		Print("EditorListItem::OnFocusLost");
		w.SetColor(ARGB(0, 255, 255, 255));
		return true;
	}		
	
	string GetText()
	{
		return m_Name;
	}
}

class EditorPlacedListItem: UILinkedObject
{
	
	protected TextWidget 	m_EditorPlacedListItemText;	
	protected Widget 		m_EditorPlacedListItemPanel;	
	
	private static int COLOR_ON_SELECTED = COLOR_BLUE;
	private static int COLOR_ON_DESELECTED = 0xFFFFFFFF;
	
	
	void ~EditorPlacedListItem()
	{
		Print("~PlacedObjectListItem");
		delete m_EditorPlacedListItemText;
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		Print("EditorPlacedListItem::OnWidgetScriptInit");
		super.OnWidgetScriptInit(w);
		
		m_EditorPlacedListItemPanel = m_Root.FindAnyWidget("EditorPlacedListItemPanel");
		m_EditorPlacedListItemText = TextWidget.Cast(m_Root.FindAnyWidget("EditorPlacedListItemText"));
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorPlacedListItem::OnMouseButtonDown");
		Input input = GetGame().GetInput();
		EditorObject editor_object = EditorObject.GetFromUILinkedRoot(m_Root);
		// LMB
		if (button == 0) {
			
			// If Holding Shift
			if (input.LocalValue("UATurbo")) {
				
				// If root object is already selected
				if (editor_object.IsSelected()) {
					editor_object.Deselect();
					return true;
				}
				
				m_EditorObject.Select(false);
				if (Editor.SelectedObjects.Count() != 0) {
					Widget root_object = m_Root.GetParent().GetChildren();
					bool selection_found = EditorObject.CheckIfRootIsSelected(root_object);
					
					// Search down the browser for first selected object
					while (!selection_found) {
						root_object = root_object.GetSibling();
						selection_found = EditorObject.CheckIfRootIsSelected(root_object);
						if (root_object == null) break;	
					}
										
					// Search until last selected object
					while (selection_found) {
						EditorObject.GetFromUILinkedRoot(root_object).Select(false);
						root_object = root_object.GetSibling();
						selection_found = !EditorObject.CheckIfRootIsSelected(root_object);
						if (root_object == null) break;
					}
					return true;		
				}			
				
			} else if (input.LocalValue("UAWalkRunTemp")) {
				if (editor_object.IsSelected())
					editor_object.Deselect();
				else editor_object.Select(false);		
			} else {
				m_EditorObject.Select();
			}
		}
		
		return true;
		
	}
	
	void Update()
	{
		if (m_EditorObject.IsSelected()) {
			m_EditorPlacedListItemPanel.SetColor(COLOR_ON_SELECTED);
			m_EditorPlacedListItemPanel.Update();
		} else {
			m_EditorPlacedListItemPanel.SetColor(ARGB(0, 255, 255, 255)); 
			m_EditorPlacedListItemPanel.Update();
		}
	}

	
	override void SetObject(notnull EditorObject target)
	{
		Print("EditorPlacedListItem::SetObject");
		super.SetObject(target);
		m_EditorPlacedListItemText.SetText(target.GetType());	
	}
	
}



class MenuBarFile: EditorWidgetEventHandler
{
	protected WrapSpacerWidget m_FileListWrapSpacer;
	
	override void OnWidgetScriptInit(Widget w)
	{
		Print("MenuBarFile::OnWidgetScriptInit");
		super.OnWidgetScriptInit(w);
		
		m_FileListWrapSpacer = m_Root.FindAnyWidget("FileListWrapSpacer");
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == m_Root)
			m_FileListWrapSpacer.Show(true);
		
		// todo handle new, open etc...
		
		return true;
	}
}


class PlaceableSearchBar: EditorWidgetEventHandler
{

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		Print("PlaceableSearchBar::OnChange");
		
		
		WrapSpacerWidget spacer = WrapSpacerWidget.Cast(Editor.ActiveEditorUI.GetRoot().FindAnyWidget("LeftbarSpacer"));
		string filter = EditBoxWidget.Cast(w).GetText();
		Widget child = spacer.GetChildren();
		while (child != null) {
			
			EditorListItem editor_list_item;
			child.GetScript(editor_list_item);
			
			if (editor_list_item != null) {
				string Config_Lower = editor_list_item.GetText();
				Config_Lower.ToLower();
				if (filter == "") return false;
	           	child.Show(Config_Lower.Contains(filter));				
			}
						
			child = child.GetSibling();

			
		}
		
		return true;
	}
	
	
	override bool OnFocus(Widget w, int x, int y)
	{
		Print("PlaceableSearchBar::OnFocus");
		
		Editor.ActiveCamera.MoveEnabled = false;
		
		return super.OnFocus(w, x, y);
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		Print("PlaceableSearchBar::OnFocusLost");
		
		Editor.ActiveCamera.MoveEnabled = true;
		
		return super.OnFocusLost(w, x, y);
	}
	
}