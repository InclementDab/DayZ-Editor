
typedef ref array<ref EditorPlaceableListItem> EditorPlaceableListItemSet;

class EditorPlaceableListItem: EditorListItem
{
	string Type, Path, Base;
	
	protected ModStructure m_ModStructure;
	protected PlaceableObjectCategory m_Category = PlaceableObjectCategory.UNKNOWN;
	
	void EditorPlaceableListItem(Widget parent, string type, string path)
	{
		EditorLog.Trace("EditorPlaceableListItem");
		Type = type; Path = path;
		
		// probably really slow
		m_ModStructure = GetModFromObject(Type);
		
		TStringArray path_array = new TStringArray();
		GetGame().ConfigGetFullPath(Path + " " + Type, path_array);
		int i = 0;
		
		if (EditorSettings.AllTypes) {
			foreach (TStringArray current_type: EditorSettings.AllTypes) {
				foreach (string base: path_array) {
					base.ToLower();
					if (current_type.Find(base) + 1) {
						Base = base;
						m_Category = i;	
						break;
					}
				}
				i++;
			}
		}
		
		
		if (m_Category == -1) {
			EditorLog.Warning(string.Format("%1 has no category!", Type));
		}
		
		
		m_TemplateController.ListItemLabel = Type;
		m_TemplateController.NotifyPropertyChanged("ListItemLabel");
		
		if (m_ModStructure) {
			m_TemplateController.ListItemIcon = GetIconFromMod(m_ModStructure);
			m_TemplateController.NotifyPropertyChanged("ListItemIcon");
		} else {
			EditorLog.Debug("Mod not found for %1", Type);
		}
		
#ifndef COMPONENT_SYSTEM
		EditorEvents.OnStartPlacing.Insert(StartPlacing);
		EditorEvents.OnStopPlacing.Insert(StopPlacing);
#endif
		
	}
	
	bool FilterType(BetterString filter)
	{
		if (filter == string.Empty) return true;
		
		string type_lower = Type;
		type_lower.ToLower();
		filter.ToLower();
		
		if (filter[0] == "@") {
			type_lower = m_ModStructure.GetModName();
			filter[0] = "";
			type_lower.ToLower();
		}
		
		return type_lower.Contains(filter);
	}
	
	PlaceableObjectCategory GetCategory() { return m_Category; }
	ModStructure GetModStructure() { return m_ModStructure; }

	void StartPlacing(Class context, EditorPlaceableListItem type)
	{
		if (type == this) {
			Select();
		}
		
		else Deselect();
	}
	
	void StopPlacing(Class context)
	{
		Deselect();
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorPlaceableListItem::OnDrag");	
		GetEditor().CreateInHand(this);
		return true;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget receiver)
	{
		EditorLog.Trace("EditorPlaceableListItem::OnDrop");
		GetEditor().PlaceObjectCommand.Execute(this, null);
		return true;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		EditorTooltip tooltip = new EditorTooltip();
		
		float size_x, size_y, pos_x, pos_y;
		m_LayoutRoot.GetScreenPos(pos_x, pos_y);
		m_LayoutRoot.GetScreenSize(size_x, size_y);
		
		tooltip.SetTitle(Type);
		tooltip.GetLayoutRoot().SetPos(pos_x + size_x, pos_y);
		tooltip.SetContent(GetWorkbenchGame().CreateObjectEx(Type, vector.Zero, ECE_NONE));
		
		if (EditorUIManager.CurrentTooltip) {
			delete EditorUIManager.CurrentTooltip;
		}
		
		EditorUIManager.CurrentTooltip = tooltip;
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (EditorUIManager.CurrentTooltip) {
			delete EditorUIManager.CurrentTooltip;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override void ListItemExecute(ButtonCommandArgs args)
	{
		if (args.GetMouseButton() == 0) {
			GetEditor().CreateInHand(this);
		} else if (args.GetMouseButton() == 1) {
			EditorPlaceableContextMenu placeable_context = new EditorPlaceableContextMenu();
			int x, y;
			GetMousePos(x, y);
			placeable_context.SetPosition(x, y);
		}
	}
}