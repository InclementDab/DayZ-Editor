typedef ref array<ref EditorPlaceableListItem> EditorPlaceableListItemSet;

class EditorPlaceableListItem: EditorListItem
{
	private ref EditorPlaceableObjectData m_Data;
	EditorPlaceableObjectData GetData() { 
		return m_Data; 
	}

	void EditorPlaceableListItem(ref EditorPlaceableObjectData data) 
	{ 
		EditorLog.Trace("EditorPlaceableListItem"); 
		m_Data = data;
		
		m_Controller.SetLabel(m_Data.Type);
		m_Controller.SetIcon(GetIconFromMod(GetModFromObject(m_Data.Type)));
		
		EditorEvents.OnStartPlacing.Insert(StartPlacing);
		EditorEvents.OnStopPlacing.Insert(StopPlacing);
	}
	
	void StartPlacing(Class context, EditorPlaceableObjectData type)
	{
		if (type == m_Data)
			m_Controller.Select();
		else m_Controller.Deselect();
	}
	
	void StopPlacing(Class context)
	{
		m_Controller.Deselect();
	}
	
	override void OnMouseEnter(int x, int y)
	{
		EditorTooltip tooltip = new EditorTooltip();			
		tooltip.SetTitle(m_Data.Type);
		float pos_x, pos_y, size_x, size_y;
		m_LayoutRoot.GetScreenPos(pos_x, pos_y);
		m_LayoutRoot.GetScreenSize(size_x, size_y);
		tooltip.GetLayoutRoot().SetPos(pos_x + size_x, pos_y);
		tooltip.SetContent(GetGame().CreateObjectEx(m_Data.Type, vector.Zero, ECE_NONE));
		
		GetEditorHudController().CurrentTooltip = tooltip;
	}
	
	override void OnMouseLeave(int x, int y)
	{
		if (GetEditorHudController().CurrentTooltip) {
			delete GetEditorHudController().CurrentTooltip;
		}
	}
	
}