
enum EditorPlaceableItemCategory {
	BUILDING = 0,
	VEHICLE = 1,
	ENTITY = 2,
	HUMAN = 3, 
	UNKNOWN = -1
};

class EditorPlaceableItem
{
	static const map<string, EditorPlaceableItemCategory> LOADED_TYPES = GetTypes();
	static const array<ref ModStructure> LOADED_MODS = ModLoader.GetMods();
	
	string Type;
	string Icon;
	string ModelName;
	TStringArray Path;
	
	ModStructure Mod;
	EditorPlaceableItemCategory Category;
	
	protected Object m_Object;
	
	private void EditorPlaceableItem(string type)
	{
		Type = type;				
		
		m_Object = GetGame().CreateObjectEx(Type, vector.Zero, ECE_NONE);

		if (!IsValidObject(m_Object)) {
			delete this;
		}
		
		m_Object.ClearFlags(EntityFlags.VISIBLE | EntityFlags.SOLID | EntityFlags.TOUCHTRIGGERS, false);	
		
		
		//GetGame().ConfigGetObjectFullPath(m_Object, Path);
		
		//Mod = LoadModData(Type, CfgPath);
		//Category = LoadItemCategory();
		
		if (Category == EditorPlaceableItemCategory.UNKNOWN) {
			EditorLog.Warning(string.Format("%1 has no category!", Type));
		}
	}
	
	// CAN RETURN NULL
	static EditorPlaceableItem Create(string type)
	{
		return new EditorPlaceableItem(type);
	}
	
	// If model volume is 0, return false
	private static bool IsValidObject(Object target)
	{
		vector size[2];		
		target.ClippingInfo(size);
		return (Math.AbsFloat(size[0][0]) + Math.AbsFloat(size[1][0]) + Math.AbsFloat(size[0][1]) + Math.AbsFloat(size[1][1]) + Math.AbsFloat(size[0][2]) + Math.AbsFloat(size[1][2]) > 0);
	}
	
	private ModStructure LoadModData(string type, string cfg_path)
	{
		string model_path = GetGame().ConfigGetTextOut("CfgVehicles " + type + " model");
		if (model_path == "UNKNOWN_P3D_FILE") {
			EditorLog.Debug("Not loading mod data for %1", type);
			return null;
		}
	
		foreach (ModStructure mod: LOADED_MODS) {
			string dir;
			GetGame().ConfigGetText(string.Format("%1 dir", mod.GetModPath()), dir);
			Print(string.Format("%1 dir", mod.GetModPath()));
			dir.ToLower(); 
			model_path.ToLower();
			
			if (model_path.Contains(dir))
				return mod;
		}
		
		return null;
	}
	
	
	private EditorPlaceableItemCategory LoadItemCategory(string type, string cfg_path)
	{
		TStringArray path_array = {};
		GetGame().ConfigGetFullPath(string.Format("%1 %2", cfg_path, type), path_array);	
		/*
		foreach (string base: path_array) {
			base.ToLower();
			if (current_type.Find(base) + 1) {
				m_Category = i;	

			}
		}*/
	
		
		return -1;
	}
	
	
	static map<string, EditorPlaceableItemCategory> GetTypes()
	{
		map<string, EditorPlaceableItemCategory> types_map = new map<string, EditorPlaceableItemCategory>();
		
		types_map.Insert("house", 				EditorPlaceableItemCategory.BUILDING);
		types_map.Insert("transport", 			EditorPlaceableItemCategory.VEHICLE);
		types_map.Insert("inventory_base", 		EditorPlaceableItemCategory.ENTITY);
		types_map.Insert("edible_base", 		EditorPlaceableItemCategory.ENTITY);
		types_map.Insert("clothing_base", 		EditorPlaceableItemCategory.ENTITY);
		types_map.Insert("weapon_base", 		EditorPlaceableItemCategory.ENTITY);
		types_map.Insert("dz_lightai", 			EditorPlaceableItemCategory.HUMAN);
		types_map.Insert("survivorbase", 		EditorPlaceableItemCategory.HUMAN);
		
		return types_map;
	}
	
	// todo refactor
	static string GetIconFromMod(ref ModStructure m_ModInfo)
	{
		EditorLog.Trace("GetIconFromMod");
		if (m_ModInfo != null) {
			string logo = m_ModInfo.GetModLogo();
			if (logo == string.Empty)
				logo = m_ModInfo.GetModLogoSmall();
			if (logo == string.Empty)
				logo = m_ModInfo.GetModLogoOver();
			if (logo == string.Empty)
				logo = m_ModInfo.GetModActionURL();
			if (logo != string.Empty)
				return logo;	
		}
		// default
		return "DayZEditor/gui/images/dayz_editor_icon_black.edds";
	}
}

class EditorPlaceableListItem: EditorListItem
{
	protected ref EditorPlaceableItem m_PlaceableItem;
	
	void EditorPlaceableListItem(EditorPlaceableItem placeable_item)
	{
		m_PlaceableItem = placeable_item;
		
		m_TemplateController.ListItemLabel = placeable_item.Type;
		m_TemplateController.NotifyPropertyChanged("ListItemLabel");
	}
}

/*
class EditorPlaceableListItem: EditorListItem
{
	
	void EditorPlaceableListItem(Widget parent, string type, string path)
	{
		EditorLog.Trace("EditorPlaceableListItem");
		
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
		if (!GetEditor().IsPlacing()) {
			GetEditor().CreateInHand(this);
		}
		
		return true;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget receiver)
	{
		EditorLog.Trace("EditorPlaceableListItem::OnDrop");
		
		if (GetEditor().IsPlacing()) {
			GetEditor().CommandManager.PlaceObjectCommand.Execute(this, null);
		}
		
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
	
	bool ListItemExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorPlaceableListItem::ListItemExecute");
		
		if (args.GetMouseButton() == 0 && !GetEditor().IsPlacing()) {
			GetEditor().CreateInHand(this);
		} 
		
		else if (args.GetMouseButton() == 1) {
			EditorPlaceableContextMenu placeable_context = new EditorPlaceableContextMenu();
			int x, y;
			GetMousePos(x, y);
			placeable_context.SetPosition(x, y);
		}
		
		return true;
	}
}