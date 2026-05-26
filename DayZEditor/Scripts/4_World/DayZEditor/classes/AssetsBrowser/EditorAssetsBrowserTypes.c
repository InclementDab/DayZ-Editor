enum EditorAssetsBrowserPlatformFilter
{
	EDITOR_ASSETS_BROWSER_PLATFORM_FILTER_PC = 0,
	EDITOR_ASSETS_BROWSER_PLATFORM_FILTER_CONSOLE = 1
}

enum EditorAssetsBrowserPlacementTypeFilter
{
	EDITOR_ASSETS_BROWSER_PLACEMENT_TYPE_CONFIG = 0,
	EDITOR_ASSETS_BROWSER_PLACEMENT_TYPE_STATIC = 1
}

class DayZEditorAssetsBrowserTabRegistration: Managed
{
	string Id;
	string Label;
	string Icon;
	int Order = -1;

	void DayZEditorAssetsBrowserTabRegistration(string id, string label, string icon = string.Empty, int order = -1)
	{
		Id = id;
		Label = label;
		Icon = icon;
		Order = order;
	}
}

class DayZEditorAssetsBrowserSubcategoryRegistration: Managed
{
	string TabId;
	string Id;
	string Label;
	string Icon;
	int Order = -1;

	void DayZEditorAssetsBrowserSubcategoryRegistration(string tab_id, string id, string label, string icon = string.Empty, int order = -1)
	{
		TabId = tab_id;
		Id = id;
		Label = label;
		Icon = icon;
		Order = order;
	}
}

class DayZEditorAssetsBrowserSectionRegistration: Managed
{
	string TabId;
	string SubcategoryId;
	string Id;
	string Label;
	int Order = -1;

	void DayZEditorAssetsBrowserSectionRegistration(string tab_id, string subcategory_id, string id, string label, int order = -1)
	{
		TabId = tab_id;
		SubcategoryId = subcategory_id;
		Id = id;
		Label = label;
		Order = order;
	}
}

class DayZEditorAssetsBrowserEntryRegistration: Managed
{
	int MatchType;
	string MatchValue;
	string TabId;
	string SubcategoryId;
	string SectionId;
	int SourceKind;
	string SourceLabel;
	int Priority = 100;

	void DayZEditorAssetsBrowserEntryRegistration(int match_type, string match_value, string tab_id, string subcategory_id, string section_id = string.Empty, int source_kind = DayZEditorAssetsBrowserSourceKind.DAYZ_EDITOR_ASSETS_BROWSER_SOURCE_MODDED, string source_label = string.Empty, int priority = 100)
	{
		MatchType = match_type;
		MatchValue = match_value;
		TabId = tab_id;
		SubcategoryId = subcategory_id;
		SectionId = section_id;
		SourceKind = source_kind;
		SourceLabel = source_label;
		Priority = priority;
	}
}

class EditorAssetsBrowserTabDefinition: Managed
{
	string Id;
	string Label;
	string Icon;
	int Order;

	void EditorAssetsBrowserTabDefinition(string id, string label, int order, string icon = string.Empty)
	{
		Id = id;
		Label = label;
		Order = order;
		Icon = icon;
	}
}

class EditorAssetsBrowserSubcategoryDefinition: Managed
{
	string TabId;
	string Id;
	string Label;
	string Icon;
	int Order;

	void EditorAssetsBrowserSubcategoryDefinition(string tab_id, string id, string label, int order, string icon = string.Empty)
	{
		TabId = tab_id;
		Id = id;
		Label = label;
		Order = order;
		Icon = icon;
	}
}

class EditorAssetsBrowserSectionDefinition: Managed
{
	string TabId;
	string SubcategoryId;
	string Id;
	string Label;
	int Order;

	void EditorAssetsBrowserSectionDefinition(string tab_id, string subcategory_id, string id, string label, int order)
	{
		TabId = tab_id;
		SubcategoryId = subcategory_id;
		Id = id;
		Label = label;
		Order = order;
	}
}

class EditorAssetsBrowserMatchResult: Managed
{
	string TabId;
	string SubcategoryId;
	string SectionId;
	string SourceId;
	string SourceLabel;
	bool Resolved;
}

class EditorAssetsBrowserSourceData: Managed
{
	string Id;
	string Label;

	void EditorAssetsBrowserSourceData(string id, string label)
	{
		Id = id;
		Label = label;
	}
}

class EditorAssetsBrowserFilterState: Managed
{
	string TabId;
	string SubcategoryId;
	string SearchText;
	int PlatformFilter = EditorAssetsBrowserPlatformFilter.EDITOR_ASSETS_BROWSER_PLATFORM_FILTER_PC;
	int PlacementTypeFilter = EditorAssetsBrowserPlacementTypeFilter.EDITOR_ASSETS_BROWSER_PLACEMENT_TYPE_CONFIG;
	bool FavoritesOnly;
	ref array<string> SelectedSourceIds = {};

	void SetSearchText(string search_text)
	{
		SearchText = search_text;
		SearchText.ToLower();
	}
}

class EditorAssetsBrowserEntry: Managed
{
	EditorPlaceableItem Placeable;
	EditorPlaceableItem PreviewPlaceable;
	string StableId;
	string DisplayName;
	string SearchText;
	string TabId;
	string SubcategoryId;
	string SectionId;
	string SectionLabel;
	string SourceId;
	string SourceLabel;
	string SortKey;
	bool CanPreview;

	void EditorAssetsBrowserEntry(EditorPlaceableItem placeable, EditorAssetsBrowserMatchResult match_result, string display_name, EditorAssetsBrowserTaxonomy taxonomy)
	{
		Placeable = placeable;
		PreviewPlaceable = placeable;
		StableId = EditorAssetsBrowserCatalog.GetStableId(placeable);
		DisplayName = display_name;
		TabId = match_result.TabId;
		SubcategoryId = match_result.SubcategoryId;
		SectionId = match_result.SectionId;
		SourceId = match_result.SourceId;
		SourceLabel = match_result.SourceLabel;
		CanPreview = EditorAssetsBrowserInference.CanPreview(placeable, match_result);

		if (taxonomy) {
			SectionLabel = taxonomy.GetSectionLabel(TabId, SubcategoryId, SectionId);
		} else {
			SectionLabel = EditorAssetsBrowserInference.FormatLabel(match_result.SectionId);
		}

		SortKey = DisplayName;
		SortKey.ToLower();

		SearchText = string.Format("%1 %2 %3 %4 %5 %6 %7 %8", DisplayName, placeable.Type, placeable.Name, placeable.Path, TabId, SubcategoryId, SectionLabel, SourceLabel);
		SearchText.ToLower();
	}

	bool IsFavorite()
	{
		return Placeable && Placeable.IsFavorite();
	}

	void SetPreviewPlaceable(EditorPlaceableItem placeable)
	{
		if (placeable) {
			PreviewPlaceable = placeable;
		}
	}

	bool Matches(EditorAssetsBrowserFilterState state, string tab_override = string.Empty, string subcategory_override = string.Empty)
	{
		if (tab_override != string.Empty && TabId != tab_override) {
			return false;
		}

		if (subcategory_override != string.Empty && SubcategoryId != subcategory_override) {
			return false;
		}

		if (state) {
			if (state.TabId != string.Empty && TabId != state.TabId) {
				return false;
			}

			if (state.SubcategoryId != string.Empty && SubcategoryId != state.SubcategoryId) {
				return false;
			}

			if (state.PlatformFilter == EditorAssetsBrowserPlatformFilter.EDITOR_ASSETS_BROWSER_PLATFORM_FILTER_CONSOLE) {
				if (!Placeable || !Placeable.ConsoleFriendly) {
					return false;
				}
			}

			if (state.SelectedSourceIds && state.SelectedSourceIds.Count() > 0 && state.SelectedSourceIds.Find(SourceId) == -1) {
				return false;
			}

			if (Placeable && Placeable.Category != EditorPlaceableItemCategory.SCRIPTED) {
				if (state.PlacementTypeFilter == EditorAssetsBrowserPlacementTypeFilter.EDITOR_ASSETS_BROWSER_PLACEMENT_TYPE_CONFIG) {
					if (Placeable.Category != EditorPlaceableItemCategory.CONFIG) {
						return false;
					}
				} else {
					if (Placeable.Category != EditorPlaceableItemCategory.STATIC) {
						return false;
					}
				}
			}

			if (state.FavoritesOnly && !IsFavorite()) {
				return false;
			}

			if (state.SearchText != string.Empty && !SearchText.Contains(state.SearchText)) {
				return false;
			}
		}

		return true;
	}
}

class EditorAssetsBrowserSectionData: Managed
{
	string Id;
	string Label;
	int Order = -1;
	ref array<ref EditorAssetsBrowserEntry> Entries = {};

	void EditorAssetsBrowserSectionData(string id, string label, int order = -1)
	{
		Id = id;
		Label = label;
		Order = order;
	}
}
