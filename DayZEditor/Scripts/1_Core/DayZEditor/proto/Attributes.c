class RegisterBrush: Class
{
	static ref array<ref Param3<typename, string, bool>> Instances = {};
	
	void RegisterBrush(typename brush, string name = "Default Brush", bool show_in_list = true)
	{
		if (!Instances) {
			Instances = {};
		}
				
		Instances.Insert(new Param3<typename, string, bool>(brush, name, show_in_list));
	}	
}

class RegisterEditorCommand: Class
{
	static ref array<typename> Instances = {};
	
	void RegisterEditorCommand(typename brush)
	{
		if (!Instances) {
			Instances = {};
		}
			
		Instances.Insert(brush);
	}		
}

enum DayZEditorAssetsBrowserSourceKind
{
	DAYZ_EDITOR_ASSETS_BROWSER_SOURCE_VANILLA = 0,
	DAYZ_EDITOR_ASSETS_BROWSER_SOURCE_MODDED = 1
}

enum DayZEditorAssetsBrowserEntryMatchType
{
	DAYZ_EDITOR_ASSETS_BROWSER_MATCH_TYPE = 0,
	DAYZ_EDITOR_ASSETS_BROWSER_MATCH_MODEL_PREFIX = 1,
	DAYZ_EDITOR_ASSETS_BROWSER_MATCH_ANCESTRY = 2
}

class RegisterDayZEditorAssetsBrowserSection: Class
{
	static ref array<ref Param5<string, string, string, string, int>> Instances = {};

	void RegisterDayZEditorAssetsBrowserSection(string tab_id, string subcategory_id, string id, string label, int order = -1)
	{
		if (!Instances) {
			Instances = {};
		}

		Instances.Insert(new Param5<string, string, string, string, int>(tab_id, subcategory_id, id, label, order));
	}
}

class RegisterDayZEditorAssetsBrowserEntry: Class
{
	static ref array<ref Param8<int, string, string, string, string, int, string, int>> Instances = {};

	void RegisterDayZEditorAssetsBrowserEntry(string match_value, string tab_id, string subcategory_id, string section_id = string.Empty, bool model_prefix = false, int source_kind = DayZEditorAssetsBrowserSourceKind.DAYZ_EDITOR_ASSETS_BROWSER_SOURCE_MODDED, string source_label = string.Empty, int priority = 100)
	{
		if (!Instances) {
			Instances = {};
		}

		int match_type = DayZEditorAssetsBrowserEntryMatchType.DAYZ_EDITOR_ASSETS_BROWSER_MATCH_TYPE;
		if (model_prefix) {
			match_type = DayZEditorAssetsBrowserEntryMatchType.DAYZ_EDITOR_ASSETS_BROWSER_MATCH_MODEL_PREFIX;
		}

		Instances.Insert(new Param8<int, string, string, string, string, int, string, int>(match_type, match_value, tab_id, subcategory_id, section_id, source_kind, source_label, priority));
	}
}

class RegisterDayZEditorAssetsBrowserEntryByAncestor: Class
{
	void RegisterDayZEditorAssetsBrowserEntryByAncestor(string ancestor, string tab_id, string subcategory_id, string section_id = string.Empty, int source_kind = DayZEditorAssetsBrowserSourceKind.DAYZ_EDITOR_ASSETS_BROWSER_SOURCE_MODDED, string source_label = string.Empty, int priority = 100)
	{
		if (!RegisterDayZEditorAssetsBrowserEntry.Instances) {
			RegisterDayZEditorAssetsBrowserEntry.Instances = {};
		}

		RegisterDayZEditorAssetsBrowserEntry.Instances.Insert(new Param8<int, string, string, string, string, int, string, int>(DayZEditorAssetsBrowserEntryMatchType.DAYZ_EDITOR_ASSETS_BROWSER_MATCH_ANCESTRY, ancestor, tab_id, subcategory_id, section_id, source_kind, source_label, priority));
	}
}
