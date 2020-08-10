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
