modded class DayZGame
{
	protected ref array<ref EditorPlaceableItem> m_AllPlaceableItems = {};
	
	protected ref map<EditorPlaceableItemCategory, ref array<EditorPlaceableItem>> m_PlaceableItems = new map<EditorPlaceableItemCategory, ref array<EditorPlaceableItem>>();
	
	protected ref map<string, EditorPlaceableItem> m_PlaceableObjectsByType = new map<string, EditorPlaceableItem>();
	
	// lookup table by p3d
	protected ref map<string, ref array<EditorPlaceableItem>> m_PlaceableObjectsByP3d = new map<string, ref array<EditorPlaceableItem>>();
	
	// args: string
	ref ScriptInvoker OnProgressReport;
			
	void DayZGame()
	{	
#ifndef SERVER
		delete m_loading;
		m_loading = new EditorLoadingScreen(this);		
		m_loading.Show();
#endif
	}
		
	override bool OnInitialize()
	{
		// Loads placeable objects	
		g_Game.ReportProgress("Loading Placeable Objects");
				
		array<string> config_paths = { CFG_VEHICLESPATH, CFG_WEAPONSPATH };
				
		// handle config objects
		foreach (string path: config_paths) {
			for (int i = 0; i < ConfigGetChildrenCount(path); i++) {
				string type;
		        ConfigGetChildName(path, i, type);
				if (ConfigGetInt(path + " " + type + " scope") < 1) {
					continue;
				}
				
				if (IsForbiddenItem(type)) {
					continue;
				}
				
				m_AllPlaceableItems.Insert(new EditorConfigPlaceableItem(path, type));
		    }
		}
		
		array<string> paths = { "DZ/plants", "DZ/plants_bliss", "DZ/rocks", "DZ/rocks_bliss" };
		foreach (string model_path: paths) {
			array<ref CF_File> files = {};
			if (!CF_Directory.GetFiles(model_path + "/*", files, FindFileFlags.ARCHIVES)) {
				continue;
			}
				
			foreach (CF_File file: files) {		
				if (!file || file.GetExtension() != ".p3d") {
					continue;
				}
				
				m_AllPlaceableItems.Insert(new EditorStaticPlaceableItem(file.GetFullPath()));
			}
		}
	
		// Statics that belong to Editor / DF
		//m_AllPlaceableItems.Insert(new EditorScriptedPlaceableItem(NetworkSpotLight));
		//m_AllPlaceableItems.Insert(new EditorScriptedPlaceableItem(NetworkPointLight));
		//m_AllPlaceableItems.Insert(new EditorScriptedPlaceableItem(NetworkParticleBase));
		
		foreach (EditorPlaceableItem placeable_item: m_AllPlaceableItems) {
			
		
		
			if (!m_PlaceableItems[placeable_item.GetCategory()]) {
				m_PlaceableItems[placeable_item.GetCategory()] = {};
			}
			
			if (!m_PlaceableObjectsByP3d[placeable_item.GetModel()]) {
				m_PlaceableObjectsByP3d[placeable_item.GetModel()] = {};
			}
			
			m_PlaceableItems[placeable_item.GetCategory()].Insert(placeable_item);
			m_PlaceableObjectsByP3d[placeable_item.GetModel()].Insert(placeable_item);
			m_PlaceableObjectsByType[placeable_item.GetName()] = placeable_item;
		}
	
		return super.OnInitialize();
	}
	
	void ReportProgress(string report)
	{
		if (!OnProgressReport) {
			OnProgressReport = new ScriptInvoker();
		}
		
		OnProgressReport.Invoke(report);
	}

	EditorPlaceableItem GetPlaceableObject(string type)
	{
		return m_PlaceableObjectsByType[type];
	}
	
	array<ref EditorPlaceableItem> GetPlaceableObjects()
	{
		return m_AllPlaceableItems;
	}
	
	array<EditorPlaceableItem> GetReplaceableObjects(string p3d)
	{
		return m_PlaceableObjectsByP3d[p3d];
	}

	map<EditorPlaceableItemCategory, ref array<EditorPlaceableItem>> GetPlaceableItemsByCategory()
	{
		return m_PlaceableItems;
	}

	static bool IsForbiddenItem(string model)
	{
		//! In theory should be safe but just in case
		if (model.Contains("Fx")) return true;
		if (model == "ItemOptics") return true;

		//! Cursed items
		if (model == "AKM_TESTBED") return true;
		if (model == "Red9") return true;
		if (model == "QuickieBow") return true;
		if (model == "LargeTentBackPack") return true;
		if (model == "SurvivorMale_Base" || model == "SurvivorFemale_Base") return true;
		if (model == "Land_VASICore" || model == "FlagCarrierCore") return true;
		if (GetGame().IsKindOf(model, "GP25Base")) return true;
		if (GetGame().IsKindOf(model, "M203Base")) return true;
		if (model == "ItemOptics_Base") return true;
		
		//! Everything is fine... I hope... :pain:
		return false;
	}
}