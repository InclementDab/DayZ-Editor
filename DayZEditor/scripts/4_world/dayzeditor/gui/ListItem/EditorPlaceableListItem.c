typedef ref array<ref EditorPlaceableListItem> EditorPlaceableListItemSet;

class EditorPlaceableListItem: EditorListItem
{
	protected ref EditorPlaceableObjectData m_EditorPlaceableObjectData;
	EditorPlaceableObjectData GetData() { 
		return m_EditorPlaceableObjectData; 
	}
	
	void SetPlaceableObjectData(EditorPlaceableObjectData data)
	{
		m_EditorPlaceableObjectData = data;
		GetListItemController().ListItemLabel = m_EditorPlaceableObjectData.Type;
		GetListItemController().NotifyPropertyChanged("ListItemLabel");
		
		GetListItemController().ListItemIcon = GetIconFromMod(GetModFromObject(m_EditorPlaceableObjectData.Type));
		GetListItemController().NotifyPropertyChanged("ListItemIcon");
		
		
		EditorEvents.OnStartPlacing.Insert(StartPlacing);
		EditorEvents.OnStopPlacing.Insert(StopPlacing);
	}
	
	void StartPlacing(Class context, EditorPlaceableObjectData type)
	{
		if (type == m_EditorPlaceableObjectData)
			Select();
		else Deselect();
	}
	
	void StopPlacing(Class context)
	{
		Deselect();
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		EditorTooltip tooltip = new EditorTooltip();			
		tooltip.SetTitle(m_EditorPlaceableObjectData.Type);
		float pos_x, pos_y, size_x, size_y;
		m_LayoutRoot.GetScreenPos(pos_x, pos_y);
		m_LayoutRoot.GetScreenSize(size_x, size_y);
		tooltip.GetLayoutRoot().SetPos(pos_x + size_x, pos_y);
		tooltip.SetContent(GetGame().CreateObjectEx(m_EditorPlaceableObjectData.Type, vector.Zero, ECE_NONE));
		
		GetEditorHudController().CurrentTooltip = tooltip;
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (GetEditorHudController().CurrentTooltip) {
			delete GetEditorHudController().CurrentTooltip;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override void ListItemExecute(ButtonCommandArgs args)
	{
		
		if (args.GetMouseButton() == 0) {
			GetEditor().CreateInHand(GetData());
			Select();
		} else if (args.GetMouseButton() == 1) {
			EditorPlaceableContextMenu placeable_context = new EditorPlaceableContextMenu();
			int x, y;
			GetMousePos(x, y);
			placeable_context.SetPosition(x, y);
		}
	}
}