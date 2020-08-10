/*
class EditorListItem: UILinkedObject
{
	private Widget						m_EditorListItemPanel;
	private ImageWidget					m_EditorListItemIcon;
	private TextWidget 					m_EditorListItemText;
	private ref EditorPlaceableObject 	m_PlaceableObject;
	
	private static int COLOR_ON_SELECTED = ARGB(140,41,128,185);
	private static int COLOR_ON_DESELECTED = ARGB(140,35,35,35);
	
	
	
	
	private ref EditorListItemTooltip m_Tooltip;
	
	// Getters
	EditorPlaceableObject GetPlaceableObject() { return m_PlaceableObject; }
	override void OnWidgetScriptInit(Widget w)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::OnWidgetScriptInit - Start");
		#endif
		
		super.OnWidgetScriptInit(w);
		m_EditorListItemPanel = Widget.Cast(m_Root.FindAnyWidget("EditorListItemPanel"));
		m_EditorListItemIcon = ImageWidget.Cast(m_Root.FindAnyWidget("EditorListItemIcon"));
		m_EditorListItemText = TextWidget.Cast(m_Root.FindAnyWidget("EditorListItemText"));
		
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::OnWidgetScriptInit - End");
		#endif
	}
	
	void SetObject(EditorPlaceableObject target)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::SetObject - Start");
		#endif
		
		m_PlaceableObject = target;
		m_EditorListItemText.SetText(m_PlaceableObject.GetType());
		GetIcon();
		m_EditorListItemText.Update();
		
		m_Tooltip = new EditorListItemTooltip(m_PlaceableObject);
		
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::SetObject - End");
		#endif
	}
	
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::OnMouseButtonDown - Start");
		#endif
		
		if (button == 0) {
			//if (w == GetFocus()) return true;
			if (GetEditor().IsPlacing()) {
				delete Editor.ObjectInHand;
			}
			
			SetFocus(w);
			return true;
		} else if (button == 1) {
			
			if (GetGame().GetInput().LocalValue("UAWalkRunTemp")) {
				
				// all very temporary please abstract elsewhere
				if (GetEditor().IsLootEditActive()) {
					GetEditor().PlaceholderRemoveLootMode();
				} else {
					GetEditor().PlaceholderForEditLootSpawns(m_PlaceableObject.GetType());
				}
				
				return true;
				
			}
		}
		
		return false;
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::OnFocus - Start");
		#endif
		
		if (w == m_EditorListItemPanel) {
			m_EditorListItemPanel.SetColor(COLOR_ON_SELECTED);
			GetEditor().CreateObjectInHand(m_PlaceableObject.GetType());	
			return true;
		}
		
		return false;
	}	
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		EditorPrint("EditorListItem::OnFocusLost");
		if (w == m_EditorListItemPanel) {
			m_EditorListItemPanel.SetColor(COLOR_ON_DESELECTED);
			return true;
		}
		
		return false;
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::OnMouseEnter - Start");
		#endif
		
		if (w == m_EditorListItemPanel)
		{
			m_EditorListItemPanel.SetColor(COLOR_ON_SELECTED);
			m_Tooltip.ShowTooltip();
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::OnMouseLeave - Start");
		#endif
		
		if (w == m_EditorListItemPanel)
		{
			if (GetFocus() != w) {
				m_EditorListItemPanel.SetColor(COLOR_ON_DESELECTED);
			}
			
			m_Tooltip.HideTooltip();
			return true;
		}
		return false;
	}
	
	private void GetIcon()
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::GetIcon - Start");
		EditorPrint("EditorListItem::GetIcon - m_PlaceableObject: " + m_PlaceableObject.Type());
		#endif
		
		string path;
		if(m_PlaceableObject.IsInherited(BuildingBase))
		{
			path = "set:dayz_editor_gui image:building_icon";
		} else if(m_PlaceableObject.IsInherited(PlantSuper))
		{
			path = "set:dayz_editor_gui image:tree_icon";
		}
		else {path = "DayZEditor/gui/images/dayz_editor_icon_black.edds"}
		
		SetIcon(path);
	}
	
	void SetIcon(string path)
	{
		m_EditorListItemIcon.LoadImageFile(0, path);
		m_EditorListItemIcon.SetImage(0);
		m_EditorListItemIcon.Update();
	}
}



class EditorPlacedListItem: UILinkedObject
{
	private ImageWidget		m_EditorPlacedListItemIcon;
	protected TextWidget 	m_EditorPlacedListItemText;	
	protected Widget 		m_EditorPlacedListItemPanel;	
	
	private static int COLOR_ON_SELECTED = ARGB(140,41,128,185);
	private static int COLOR_ON_DESELECTED = ARGB(140,35,35,35);

	
	void ~EditorPlacedListItem()
	{
		Print("EditorPlacedListItem");
		
		delete m_EditorPlacedListItemText;
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	
	
	override void OnWidgetScriptInit(Widget w)
	{
		Print("EditorPlacedListItem::OnWidgetScriptInit");
		
		super.OnWidgetScriptInit(w);
		
		m_EditorPlacedListItemPanel = m_Root.FindAnyWidget("EditorPlacedListItemPanel");
		m_EditorPlacedListItemText = TextWidget.Cast(m_Root.FindAnyWidget("EditorPlacedListItemText"));
		m_EditorPlacedListItemIcon = ImageWidget.Cast(m_Root.FindAnyWidget("EditorPlacedListItemIcon"));
		
		EditorEvents.OnObjectSelected.Insert(EditorObjectSelected);
		EditorEvents.OnObjectDeselected.Insert(EditorObjectDeselected);	
	
	}
	
	void EditorObjectSelected(Class context, EditorObject target)
	{
		if (target == m_EditorObject) {
			m_EditorPlacedListItemPanel.SetColor(COLOR_ON_SELECTED);
			m_EditorPlacedListItemPanel.Update();
		}
	}
	
	void EditorObjectDeselected(Class context, EditorObject target)
	{		
		if (target == m_EditorObject) {
			m_EditorPlacedListItemPanel.SetColor(COLOR_ON_DESELECTED); 
			m_EditorPlacedListItemPanel.Update();
		}

	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorPlacedListItem::OnMouseButtonDown");
		
		//EditorEvents.SelectObject(this, m_EditorObject);
		Input input = GetGame().GetInput();
		if (button == 0) {
			
			// If Holding Shift
			if (input.LocalValue("UATurbo")) {
				
				// If root object is already selected
				if (m_EditorObject.IsSelected()) {
					EditorEvents.DeselectObject(this, m_EditorObject);
					return true;
				}
				
				EditorEvents.SelectObject(this, m_EditorObject);
				if (GetEditor().GetObjectManager().GetSelectedObjects().Count() != 0) {
					
					Widget root_object = m_Root.GetParent().GetChildren();
					bool selection_found = GetEditor().GetObjectManager().CheckIfRootIsSelected(root_object);
					
					
					// Search down the browser for first selected object
					while (!selection_found) {
						root_object = root_object.GetSibling();
						selection_found = GetEditor().GetObjectManager().CheckIfRootIsSelected(root_object);
						if (root_object == null) break;	
					}
										
					// Search until last selected object
					while (selection_found) {
						EditorEvents.SelectObject(this, GetEditor().GetObjectManager().GetFromUILinkedRoot(root_object));
						root_object = root_object.GetSibling();
						selection_found = !GetEditor().GetObjectManager().CheckIfRootIsSelected(root_object);
						if (root_object == null) break;
					}
					return true;		
				}			
				
			} else if (input.LocalValue("UAWalkRunTemp")) {
				GetEditor().GetObjectManager().ToggleSelection(m_EditorObject);		
			} else {
				EditorEvents.ClearSelection(this);
				EditorEvents.SelectObject(this, m_EditorObject);
			}
		}
		return true;
	}
	

	
	
	

	
	override void SetObject(notnull EditorObject target)
	{
		Print("EditorPlacedListItem::SetObject");
		
		super.SetObject(target);
		m_EditorPlacedListItemText.SetText(target.GetType());
		m_EditorPlacedListItemText.Update();
		
		// this is really slow here todo
		SetIcon(Editor.GetIconFromMod(Editor.GetModFromObject(target.GetType())));
	}
	
	void SetIcon(string path)
	{
		m_EditorPlacedListItemIcon.LoadImageFile(0, path);
		m_EditorPlacedListItemIcon.SetImage(0);
		m_EditorPlacedListItemIcon.Update();
	}

}
*/