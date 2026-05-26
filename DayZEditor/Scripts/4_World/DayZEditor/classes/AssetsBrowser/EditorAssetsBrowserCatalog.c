class EditorAssetsBrowserCatalog: Managed
{
	protected ref EditorAssetsBrowserTaxonomy m_Taxonomy;
	protected ref array<ref EditorAssetsBrowserEntry> m_Entries = {};
	protected ref map<string, ref EditorAssetsBrowserEntry> m_EntriesByStableId = new map<string, ref EditorAssetsBrowserEntry>();
	protected ref map<string, ref array<ref EditorAssetsBrowserEntry>> m_EntriesByTab = new map<string, ref array<ref EditorAssetsBrowserEntry>>();
	protected ref map<string, ref array<ref EditorAssetsBrowserEntry>> m_EntriesBySubcategory = new map<string, ref array<ref EditorAssetsBrowserEntry>>();
	protected ref array<ref EditorAssetsBrowserSourceData> m_Sources = {};

	static string GetStableId(EditorPlaceableItem placeable)
	{
		return string.Format("%1|%2|%3", placeable.Category, placeable.Type, placeable.Path);
	}

	void Build(array<ref EditorPlaceableItem> placeable_items)
	{
		EditorAssetsBrowserInference.LoadDefinitions(placeable_items);
		m_Taxonomy = EditorAssetsBrowserInference.CreateTaxonomy();
		m_Entries.Clear();
		m_EntriesByStableId.Clear();
		m_EntriesByTab.Clear();
		m_EntriesBySubcategory.Clear();
		m_Sources.Clear();

		ref map<string, int> entry_keys = new map<string, int>();

		foreach (EditorPlaceableItem placeable_item: placeable_items) {
			if (!placeable_item) {
				continue;
			}

			if (!ShouldIncludePlaceable(placeable_item)) {
				continue;
			}

			string display_name = EditorAssetsBrowserInference.GetDisplayName(placeable_item);
			EditorAssetsBrowserMatchResult match_result = EditorAssetsBrowserInference.Classify(placeable_item);
			if (!match_result || !match_result.Resolved) {
				continue;
			}

			EditorAssetsBrowserEntry entry = new EditorAssetsBrowserEntry(placeable_item, match_result, display_name, m_Taxonomy);
			string entry_key = GetDedupeKey(entry);
			if (entry_key != string.Empty && entry_keys.Contains(entry_key)) {
				int existing_index = entry_keys[entry_key];
				EditorAssetsBrowserEntry existing_entry = m_Entries[existing_index];
				m_EntriesByStableId[entry.StableId] = existing_entry;
				continue;
			}

			m_Entries.Insert(entry);
			m_EntriesByStableId[entry.StableId] = entry;
			entry_keys[entry_key] = m_Entries.Count() - 1;
		}

		IndexEntries();
		IndexSources();
	}

	EditorAssetsBrowserTaxonomy GetTaxonomy()
	{
		if (!m_Taxonomy) {
			m_Taxonomy = EditorAssetsBrowserInference.CreateTaxonomy();
		}

		return m_Taxonomy;
	}

	EditorAssetsBrowserEntry FindByPlaceable(EditorPlaceableItem placeable)
	{
		if (!placeable) {
			return null;
		}

		return m_EntriesByStableId[GetStableId(placeable)];
	}

	array<ref EditorAssetsBrowserSourceData> GetSources()
	{
		return m_Sources;
	}

	array<ref EditorAssetsBrowserSectionData> GetSections(EditorAssetsBrowserFilterState state)
	{
		ref array<ref EditorAssetsBrowserSectionData> sections = {};
		ref map<string, ref EditorAssetsBrowserSectionData> sections_by_id = new map<string, ref EditorAssetsBrowserSectionData>();
		array<ref EditorAssetsBrowserEntry> candidates = GetCandidateEntries(state);
		if (!candidates) {
			return sections;
		}

		foreach (EditorAssetsBrowserEntry entry: candidates) {
			if (!entry || !entry.Matches(state)) {
				continue;
			}

			EditorAssetsBrowserSectionData section = sections_by_id[entry.SectionId];
			if (!section) {
				section = new EditorAssetsBrowserSectionData(entry.SectionId, m_Taxonomy.GetSectionLabel(entry.TabId, entry.SubcategoryId, entry.SectionId), m_Taxonomy.GetSectionOrder(entry.TabId, entry.SubcategoryId, entry.SectionId));
				InsertSectionSorted(sections, section);
				sections_by_id[entry.SectionId] = section;
			}

			InsertEntrySorted(section.Entries, entry);
		}

		return sections;
	}

	int CountEntriesForTab(EditorAssetsBrowserFilterState state, string tab_id)
	{
		array<ref EditorAssetsBrowserEntry> candidates = m_EntriesByTab[tab_id];
		if (!candidates) {
			return 0;
		}

		int count = 0;
		foreach (EditorAssetsBrowserEntry entry: candidates) {
			if (entry && entry.Matches(state, tab_id)) {
				count++;
			}
		}

		return count;
	}

	int CountEntriesForSubcategory(EditorAssetsBrowserFilterState state, string tab_id, string subcategory_id)
	{
		array<ref EditorAssetsBrowserEntry> candidates = m_EntriesBySubcategory[GetSubcategoryKey(tab_id, subcategory_id)];
		if (!candidates) {
			return 0;
		}

		int count = 0;
		foreach (EditorAssetsBrowserEntry entry: candidates) {
			if (entry && entry.Matches(state, tab_id, subcategory_id)) {
				count++;
			}
		}

		return count;
	}

	bool HasMatches(EditorAssetsBrowserFilterState state)
	{
		array<ref EditorAssetsBrowserEntry> candidates = GetCandidateEntries(state);
		if (!candidates) {
			return false;
		}

		foreach (EditorAssetsBrowserEntry entry: candidates) {
			if (entry && entry.Matches(state)) {
				return true;
			}
		}

		return false;
	}

	protected void IndexEntries()
	{
		foreach (EditorAssetsBrowserEntry entry: m_Entries) {
			if (!entry) {
				continue;
			}

			EnsureBucket(m_EntriesByTab, entry.TabId).Insert(entry);
			EnsureBucket(m_EntriesBySubcategory, GetSubcategoryKey(entry.TabId, entry.SubcategoryId)).Insert(entry);
		}
	}

	protected void IndexSources()
	{
		foreach (EditorAssetsBrowserEntry entry: m_Entries) {
			if (!entry || entry.SourceId == string.Empty || entry.SourceLabel == string.Empty || FindSourceIndex(entry.SourceId) != -1) {
				continue;
			}

			InsertSourceSorted(new EditorAssetsBrowserSourceData(entry.SourceId, entry.SourceLabel));
		}
	}

	protected array<ref EditorAssetsBrowserEntry> EnsureBucket(map<string, ref array<ref EditorAssetsBrowserEntry>> buckets, string key)
	{
		if (!buckets[key]) {
			buckets[key] = {};
		}

		return buckets[key];
	}

	protected array<ref EditorAssetsBrowserEntry> GetCandidateEntries(EditorAssetsBrowserFilterState state)
	{
		if (state) {
			if (state.TabId != string.Empty && state.SubcategoryId != string.Empty) {
				return m_EntriesBySubcategory[GetSubcategoryKey(state.TabId, state.SubcategoryId)];
			}

			if (state.TabId != string.Empty) {
				return m_EntriesByTab[state.TabId];
			}
		}

		return m_Entries;
	}

	protected static void InsertEntrySorted(array<ref EditorAssetsBrowserEntry> entries, EditorAssetsBrowserEntry entry)
	{
		int insert_index = entries.Count();
		for (int i = 0; i < entries.Count(); i++) {
			if (entry.SortKey < entries[i].SortKey) {
				insert_index = i;
				break;
			}
		}

		entries.InsertAt(entry, insert_index);
	}

	protected static void InsertSectionSorted(array<ref EditorAssetsBrowserSectionData> sections, EditorAssetsBrowserSectionData section)
	{
		int insert_index = sections.Count();
		for (int i = 0; i < sections.Count(); i++) {
			if (SortSectionBefore(section, sections[i])) {
				insert_index = i;
				break;
			}
		}

		sections.InsertAt(section, insert_index);
	}

	protected int FindSourceIndex(string source_id)
	{
		for (int i = 0; i < m_Sources.Count(); i++) {
			if (m_Sources[i] && m_Sources[i].Id == source_id) {
				return i;
			}
		}

		return -1;
	}

	protected void InsertSourceSorted(EditorAssetsBrowserSourceData source)
	{
		int insert_index = m_Sources.Count();
		for (int i = 0; i < m_Sources.Count(); i++) {
			if (SortSourceBefore(source, m_Sources[i])) {
				insert_index = i;
				break;
			}
		}

		m_Sources.InsertAt(source, insert_index);
	}

	protected static bool SortSourceBefore(EditorAssetsBrowserSourceData lhs, EditorAssetsBrowserSourceData rhs)
	{
		bool lhs_is_vanilla = lhs && lhs.Id == "vanilla";
		bool rhs_is_vanilla = rhs && rhs.Id == "vanilla";
		if (lhs_is_vanilla != rhs_is_vanilla) {
			return !lhs_is_vanilla;
		}

		string lhs_lower = lhs.Label;
		string rhs_lower = rhs.Label;
		lhs_lower.ToLower();
		rhs_lower.ToLower();
		if (lhs_lower != rhs_lower) {
			return lhs_lower < rhs_lower;
		}

		return lhs.Id < rhs.Id;
	}

	protected static bool ShouldIncludePlaceable(EditorPlaceableItem placeable)
	{
		if (!placeable) {
			return false;
		}

		if (placeable.Category == EditorPlaceableItemCategory.SCRIPTED) {
			return true;
		}

		string model_name = placeable.GetModelName();
		model_name.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);
		model_name.Replace(SystemPath.SEPERATOR + SystemPath.SEPERATOR, SystemPath.SEPERATOR);
		model_name.ToLower();
		model_name.TrimInPlace();

		if (model_name == string.Empty) {
			return false;
		}

		if (model_name == "bmp" || model_name == "bmp.p3d") {
			return false;
		}

		return true;
	}

	protected static bool SortSectionBefore(EditorAssetsBrowserSectionData lhs, EditorAssetsBrowserSectionData rhs)
	{
		if ((lhs.Order >= 0) != (rhs.Order >= 0)) {
			return lhs.Order >= 0;
		}

		if (lhs.Order != rhs.Order && lhs.Order >= 0 && rhs.Order >= 0) {
			return lhs.Order < rhs.Order;
		}

		bool lhs_fallback = IsFallbackSection(lhs.Id);
		bool rhs_fallback = IsFallbackSection(rhs.Id);
		if (lhs_fallback != rhs_fallback) {
			return !lhs_fallback;
		}

		string lhs_label = lhs.Label;
		string rhs_label = rhs.Label;
		lhs_label.ToLower();
		rhs_label.ToLower();
		return lhs_label < rhs_label;
	}

	protected static bool IsFallbackSection(string section_id)
	{
		return section_id == "unknown" || section_id == "misc";
	}

	protected static string GetDedupeKey(EditorAssetsBrowserEntry entry)
	{
		if (!entry || !entry.Placeable) {
			return string.Empty;
		}

		switch (entry.Placeable.Category) {
			case EditorPlaceableItemCategory.CONFIG: {
				string type_lower = entry.Placeable.Type;
				type_lower.ToLower();
				return "config|" + type_lower;
			}

			case EditorPlaceableItemCategory.STATIC: {
				string normalized_model = EditorAssetsBrowserInference.NormalizeDedupePath(entry.Placeable.GetModelName());
				if (normalized_model == string.Empty) {
					normalized_model = EditorAssetsBrowserInference.NormalizeDedupePath(entry.Placeable.Path);
				}

				if (normalized_model == string.Empty) {
					return string.Empty;
				}

				return "static|" + normalized_model;
			}

			case EditorPlaceableItemCategory.SCRIPTED: {
				string type_lower_scripted = entry.Placeable.Type;
				type_lower_scripted.ToLower();
				return "scripted|" + type_lower_scripted;
			}
		}

		return string.Empty;
	}

	protected string GetSubcategoryKey(string tab_id, string subcategory_id)
	{
		return string.Format("%1|%2", tab_id, subcategory_id);
	}
}
