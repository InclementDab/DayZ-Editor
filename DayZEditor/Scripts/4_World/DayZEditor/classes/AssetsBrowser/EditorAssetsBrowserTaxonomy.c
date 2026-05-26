class EditorAssetsBrowserTaxonomy: Managed
{
	protected static ref array<ref DayZEditorAssetsBrowserTabRegistration> s_BuiltinTabs;
	protected static ref array<ref DayZEditorAssetsBrowserSubcategoryRegistration> s_BuiltinSubcategories;
	protected static ref map<string, bool> s_BuiltinSubcategoryKeys;

	protected ref array<ref EditorAssetsBrowserTabDefinition> m_Tabs = {};
	protected ref array<ref EditorAssetsBrowserSubcategoryDefinition> m_Subcategories = {};
	protected ref map<string, ref EditorAssetsBrowserTabDefinition> m_TabById = new map<string, ref EditorAssetsBrowserTabDefinition>();
	protected ref map<string, ref EditorAssetsBrowserSubcategoryDefinition> m_SubcategoryByKey = new map<string, ref EditorAssetsBrowserSubcategoryDefinition>();
	protected ref map<string, ref EditorAssetsBrowserSectionDefinition> m_SectionByKey = new map<string, ref EditorAssetsBrowserSectionDefinition>();
	protected ref map<string, int> m_NextSectionOrderByBucket = new map<string, int>();
	protected int m_NextTabOrder;
	protected int m_NextSubcategoryOrder;

	void EditorAssetsBrowserTaxonomy()
	{
		SeedBuiltins();
		SortDefinitions();
	}

	static bool IsBuiltinSubcategoryId(string tab_id, string subcategory_id)
	{
		EnsureBuiltinDefinitions();
		return s_BuiltinSubcategoryKeys.Contains(GetBuiltinSubcategoryKey(tab_id, subcategory_id));
	}

	static bool IsBuiltinTabId(string tab_id)
	{
		EnsureBuiltinDefinitions();
		foreach (DayZEditorAssetsBrowserTabRegistration tab_registration: s_BuiltinTabs) {
			if (tab_registration && tab_registration.Id == tab_id) {
				return true;
			}
		}

		return false;
	}

	protected void SeedBuiltins()
	{
		EnsureBuiltinDefinitions();

		foreach (DayZEditorAssetsBrowserTabRegistration tab_registration: s_BuiltinTabs) {
			AddTab(tab_registration.Id, tab_registration.Label, tab_registration.Icon, tab_registration.Order);
		}

		foreach (DayZEditorAssetsBrowserSubcategoryRegistration subcategory_registration: s_BuiltinSubcategories) {
			AddSubcategory(subcategory_registration.TabId, subcategory_registration.Id, subcategory_registration.Label, subcategory_registration.Icon, subcategory_registration.Order);
		}
	}

	protected static void EnsureBuiltinDefinitions()
	{
		if (s_BuiltinTabs && s_BuiltinSubcategories && s_BuiltinSubcategoryKeys) {
			return;
		}

		s_BuiltinTabs = new array<ref DayZEditorAssetsBrowserTabRegistration>();
		s_BuiltinSubcategories = new array<ref DayZEditorAssetsBrowserSubcategoryRegistration>();
		s_BuiltinSubcategoryKeys = new map<string, bool>();

		AddBuiltinTabDefinition("structures", "Structures", 0);
		AddBuiltinSubcategoryDefinition("structures", "buildings", "Buildings", 0);
		AddBuiltinSubcategoryDefinition("structures", "basebuilding", "Basebuilding", 1);
		AddBuiltinSubcategoryDefinition("structures", "barriers", "Barriers", 2);
		AddBuiltinSubcategoryDefinition("structures", "infrastructure", "Infrastructure", 3);
		AddBuiltinSubcategoryDefinition("structures", "signs", "Signs", 4);
		AddBuiltinSubcategoryDefinition("structures", "military", "Military", 5);
		AddBuiltinSubcategoryDefinition("structures", "industrial", "Industrial", 6);
		AddBuiltinSubcategoryDefinition("structures", "misc", "Misc", 7);

		AddBuiltinTabDefinition("props", "Props", 1);
		AddBuiltinSubcategoryDefinition("props", "furniture", "Furniture", 0);
		AddBuiltinSubcategoryDefinition("props", "storage", "Storage", 1);
		AddBuiltinSubcategoryDefinition("props", "decor", "Decor", 2);
		AddBuiltinSubcategoryDefinition("props", "appliances", "Appliances", 3);
		AddBuiltinSubcategoryDefinition("props", "wrecks", "Wrecks", 4);
		AddBuiltinSubcategoryDefinition("props", "misc", "Misc", 5);

		AddBuiltinTabDefinition("vehicles", "Vehicles", 2);
		AddBuiltinSubcategoryDefinition("vehicles", "cars", "Cars", 0);
		AddBuiltinSubcategoryDefinition("vehicles", "trucks", "Trucks", 1);
		AddBuiltinSubcategoryDefinition("vehicles", "boats", "Boats", 2);
		AddBuiltinSubcategoryDefinition("vehicles", "parts", "Parts", 3);
		AddBuiltinSubcategoryDefinition("vehicles", "misc", "Misc", 4);

		AddBuiltinTabDefinition("nature", "Nature", 3);
		AddBuiltinSubcategoryDefinition("nature", "trees_bushes", "Trees-Bushes", 0);
		AddBuiltinSubcategoryDefinition("nature", "plants", "Plants", 1);
		AddBuiltinSubcategoryDefinition("nature", "rocks", "Rocks", 2);
		AddBuiltinSubcategoryDefinition("nature", "terrain", "Terrain", 3);
		AddBuiltinSubcategoryDefinition("nature", "misc", "Misc", 4);

		AddBuiltinTabDefinition("clothing", "Clothing", 4);
		AddBuiltinSubcategoryDefinition("clothing", "head", "Head", 0);
		AddBuiltinSubcategoryDefinition("clothing", "face", "Face", 1);
		AddBuiltinSubcategoryDefinition("clothing", "eyewear", "Eyewear", 2);
		AddBuiltinSubcategoryDefinition("clothing", "body", "Body", 3);
		AddBuiltinSubcategoryDefinition("clothing", "legs", "Legs", 4);
		AddBuiltinSubcategoryDefinition("clothing", "feet", "Feet", 5);
		AddBuiltinSubcategoryDefinition("clothing", "hands", "Hands", 6);
		AddBuiltinSubcategoryDefinition("clothing", "vest", "Vest", 7);
		AddBuiltinSubcategoryDefinition("clothing", "back", "Back", 8);
		AddBuiltinSubcategoryDefinition("clothing", "belt", "Belt", 9);
		AddBuiltinSubcategoryDefinition("clothing", "accessories", "Accessories", 10);
		AddBuiltinSubcategoryDefinition("clothing", "misc", "Misc", 11);

		AddBuiltinTabDefinition("gear", "Gear", 5);
		AddBuiltinSubcategoryDefinition("gear", "firearms", "Firearms", 0);
		AddBuiltinSubcategoryDefinition("gear", "pistols", "Pistols", 1);
		AddBuiltinSubcategoryDefinition("gear", "shotguns", "Shotguns", 2);
		AddBuiltinSubcategoryDefinition("gear", "melee", "Melee", 3);
		AddBuiltinSubcategoryDefinition("gear", "archery", "Archery", 4);
		AddBuiltinSubcategoryDefinition("gear", "launchers", "Launchers", 5);
		AddBuiltinSubcategoryDefinition("gear", "explosives", "Explosives", 6);
		AddBuiltinSubcategoryDefinition("gear", "nonlethal", "Non-Lethal", 7);
		AddBuiltinSubcategoryDefinition("gear", "magazines", "Magazines", 8);
		AddBuiltinSubcategoryDefinition("gear", "weapon_attachments", "Attachments", 9);
		AddBuiltinSubcategoryDefinition("gear", "optics", "Optics", 10);
		AddBuiltinSubcategoryDefinition("gear", "misc", "Misc", 11);

		AddBuiltinTabDefinition("consumables", "Consumables", 6);
		AddBuiltinSubcategoryDefinition("consumables", "food", "Food & Drinks", 0);
		AddBuiltinSubcategoryDefinition("consumables", "medical", "Medical", 1);
		AddBuiltinSubcategoryDefinition("consumables", "tools", "Tools", 2);
		AddBuiltinSubcategoryDefinition("consumables", "misc", "Misc", 3);

		AddBuiltinTabDefinition("creatures", "Creatures", 7);
		AddBuiltinSubcategoryDefinition("creatures", "animals", "Animals", 0);
		AddBuiltinSubcategoryDefinition("creatures", "infected", "Infected", 1);
		AddBuiltinSubcategoryDefinition("creatures", "humans", "Humans", 2);
		AddBuiltinSubcategoryDefinition("creatures", "misc", "Misc", 3);

		AddBuiltinTabDefinition("effects", "Effects", 8);
		AddBuiltinSubcategoryDefinition("effects", "lights", "Lights", 0);
		AddBuiltinSubcategoryDefinition("effects", "particles", "Particles", 1);
		AddBuiltinSubcategoryDefinition("effects", "scripted", "Scripted", 2);
		AddBuiltinSubcategoryDefinition("effects", "misc", "Misc", 3);

		AddBuiltinTabDefinition("misc", "Misc", 9);
		AddBuiltinSubcategoryDefinition("misc", "editor", "Editor", 0);
		AddBuiltinSubcategoryDefinition("misc", "unknown", "Unknown", 1);
	}

	protected static void AddBuiltinTabDefinition(string id, string label, int order, string icon = string.Empty)
	{
		s_BuiltinTabs.Insert(new DayZEditorAssetsBrowserTabRegistration(id, label, icon, order));
	}

	protected static void AddBuiltinSubcategoryDefinition(string tab_id, string id, string label, int order, string icon = string.Empty)
	{
		s_BuiltinSubcategories.Insert(new DayZEditorAssetsBrowserSubcategoryRegistration(tab_id, id, label, icon, order));
		s_BuiltinSubcategoryKeys.Insert(GetBuiltinSubcategoryKey(tab_id, id), true);
	}

	protected static string GetBuiltinSubcategoryKey(string tab_id, string subcategory_id)
	{
		return string.Format("%1|%2", tab_id, subcategory_id);
	}

	void AddTab(string id, string label, string icon = string.Empty, int order = -1)
	{
		EditorAssetsBrowserTabDefinition existing_tab = m_TabById[id];
		if (existing_tab) {
			if (label != string.Empty) {
				existing_tab.Label = label;
			}

			if (icon != string.Empty) {
				existing_tab.Icon = icon;
			}

			if (order >= 0) {
				existing_tab.Order = order;
			}
			return;
		}

		if (order < 0) {
			order = m_NextTabOrder++;
		} else {
			m_NextTabOrder = Math.Max(m_NextTabOrder, order + 1);
		}

		EditorAssetsBrowserTabDefinition tab_definition = new EditorAssetsBrowserTabDefinition(id, label, order, icon);
		m_Tabs.Insert(tab_definition);
		m_TabById[id] = tab_definition;
	}

	void AddSubcategory(string tab_id, string id, string label, string icon = string.Empty, int order = -1)
	{
		string key = GetSubcategoryKey(tab_id, id);
		EditorAssetsBrowserSubcategoryDefinition existing_subcategory = m_SubcategoryByKey[key];
		if (existing_subcategory) {
			if (label != string.Empty) {
				existing_subcategory.Label = label;
			}

			if (icon != string.Empty) {
				existing_subcategory.Icon = icon;
			}

			if (order >= 0) {
				existing_subcategory.Order = order;
			}
			return;
		}

		if (!m_TabById[tab_id]) {
			EditorLog.Error("EditorAssetsBrowserTaxonomy::AddSubcategory missing tab %1 for subcategory %2", tab_id, id);
			return;
		}

		if (order < 0) {
			order = m_NextSubcategoryOrder++;
		} else {
			m_NextSubcategoryOrder = Math.Max(m_NextSubcategoryOrder, order + 1);
		}

		EditorAssetsBrowserSubcategoryDefinition subcategory_definition = new EditorAssetsBrowserSubcategoryDefinition(tab_id, id, label, order, icon);
		m_Subcategories.Insert(subcategory_definition);
		m_SubcategoryByKey[key] = subcategory_definition;
	}

	void AddSection(string tab_id, string subcategory_id, string id, string label = string.Empty, int order = -1)
	{
		if (!HasSubcategory(tab_id, subcategory_id)) {
			EditorLog.Error("EditorAssetsBrowserTaxonomy::AddSection invalid target %1/%2 for section %3", tab_id, subcategory_id, id);
			return;
		}

		string bucket_key = GetSectionBucketKey(tab_id, subcategory_id);
		if (!m_NextSectionOrderByBucket.Contains(bucket_key)) {
			m_NextSectionOrderByBucket[bucket_key] = 0;
		}

		string key = GetSectionKey(tab_id, subcategory_id, id);
		EditorAssetsBrowserSectionDefinition existing_section = m_SectionByKey[key];
		if (existing_section) {
			if (label != string.Empty) {
				existing_section.Label = label;
			}

			if (order >= 0) {
				existing_section.Order = order;
				m_NextSectionOrderByBucket[bucket_key] = Math.Max(m_NextSectionOrderByBucket[bucket_key], order + 1);
			}
			return;
		}

		if (label == string.Empty) {
			label = EditorAssetsBrowserInference.FormatLabel(id);
		}

		if (order < 0) {
			order = m_NextSectionOrderByBucket[bucket_key];
		}

		m_NextSectionOrderByBucket[bucket_key] = Math.Max(m_NextSectionOrderByBucket[bucket_key], order + 1);
		m_SectionByKey[key] = new EditorAssetsBrowserSectionDefinition(tab_id, subcategory_id, id, label, order);
	}

	bool HasSubcategory(string tab_id, string id)
	{
		return m_SubcategoryByKey[GetSubcategoryKey(tab_id, id)] != null;
	}

	string GetSectionLabel(string tab_id, string subcategory_id, string section_id)
	{
		EditorAssetsBrowserSectionDefinition definition = m_SectionByKey[GetSectionKey(tab_id, subcategory_id, section_id)];
		if (definition) {
			return definition.Label;
		}

		return EditorAssetsBrowserInference.FormatLabel(section_id);
	}

	int GetSectionOrder(string tab_id, string subcategory_id, string section_id)
	{
		EditorAssetsBrowserSectionDefinition definition = m_SectionByKey[GetSectionKey(tab_id, subcategory_id, section_id)];
		if (definition) {
			return definition.Order;
		}

		return -1;
	}

	array<ref EditorAssetsBrowserTabDefinition> GetTabs()
	{
		return m_Tabs;
	}

	array<ref EditorAssetsBrowserSubcategoryDefinition> GetSubcategories()
	{
		return m_Subcategories;
	}

	void FinalizeDefinitions()
	{
		SortDefinitions();
	}

	protected void SortDefinitions()
	{
		SortTabs();
		SortSubcategories();
	}

	protected void SortTabs()
	{
		ref array<ref EditorAssetsBrowserTabDefinition> sorted = {};
		foreach (EditorAssetsBrowserTabDefinition definition: m_Tabs) {
			InsertTabSorted(sorted, definition);
		}

		m_Tabs = sorted;
	}

	protected void SortSubcategories()
	{
		ref array<ref EditorAssetsBrowserSubcategoryDefinition> sorted = {};
		foreach (EditorAssetsBrowserSubcategoryDefinition definition: m_Subcategories) {
			InsertSubcategorySorted(sorted, definition);
		}

		m_Subcategories = sorted;
	}

	protected void InsertTabSorted(array<ref EditorAssetsBrowserTabDefinition> definitions, EditorAssetsBrowserTabDefinition definition)
	{
		int insert_index = definitions.Count();
		for (int i = 0; i < definitions.Count(); i++) {
			if (SortTabBefore(definition, definitions[i])) {
				insert_index = i;
				break;
			}
		}

		definitions.InsertAt(definition, insert_index);
	}

	protected void InsertSubcategorySorted(array<ref EditorAssetsBrowserSubcategoryDefinition> definitions, EditorAssetsBrowserSubcategoryDefinition definition)
	{
		int insert_index = definitions.Count();
		for (int i = 0; i < definitions.Count(); i++) {
			if (SortSubcategoryBefore(definition, definitions[i])) {
				insert_index = i;
				break;
			}
		}

		definitions.InsertAt(definition, insert_index);
	}

	protected bool SortTabBefore(EditorAssetsBrowserTabDefinition lhs, EditorAssetsBrowserTabDefinition rhs)
	{
		return CompareOrderedLabels(lhs.Order, lhs.Label, rhs.Order, rhs.Label);
	}

	protected bool SortSubcategoryBefore(EditorAssetsBrowserSubcategoryDefinition lhs, EditorAssetsBrowserSubcategoryDefinition rhs)
	{
		if (lhs.TabId != rhs.TabId) {
			EditorAssetsBrowserTabDefinition lhs_tab = m_TabById[lhs.TabId];
			EditorAssetsBrowserTabDefinition rhs_tab = m_TabById[rhs.TabId];
			if (lhs_tab && rhs_tab && lhs_tab.Order != rhs_tab.Order) {
				return lhs_tab.Order < rhs_tab.Order;
			}

			return lhs.TabId < rhs.TabId;
		}

		return CompareOrderedLabels(lhs.Order, lhs.Label, rhs.Order, rhs.Label);
	}

	protected bool CompareOrderedLabels(int lhs_order, string lhs_label, int rhs_order, string rhs_label)
	{
		if (lhs_order != rhs_order) {
			return lhs_order < rhs_order;
		}

		string lhs_sort = lhs_label;
		string rhs_sort = rhs_label;
		lhs_sort.ToLower();
		rhs_sort.ToLower();
		return lhs_sort < rhs_sort;
	}

	protected string GetSubcategoryKey(string tab_id, string id)
	{
		return string.Format("%1|%2", tab_id, id);
	}

	protected string GetSectionKey(string tab_id, string subcategory_id, string id)
	{
		return string.Format("%1|%2|%3", tab_id, subcategory_id, id);
	}

	protected string GetSectionBucketKey(string tab_id, string subcategory_id)
	{
		return string.Format("%1|%2", tab_id, subcategory_id);
	}
}
