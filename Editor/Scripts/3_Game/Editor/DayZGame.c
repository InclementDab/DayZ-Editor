modded class DayZGame
{
	protected ref array<ref EditorPlaceableObjectData> m_AllPlaceableItems = {};
	
	protected ref map<EditorObjectDataCategory, ref array<EditorPlaceableObjectData>> m_PlaceableItems = new map<EditorObjectDataCategory, ref array<EditorPlaceableObjectData>>();
	
	protected ref map<string, EditorPlaceableObjectData> m_PlaceableObjectsByType = new map<string, EditorPlaceableObjectData>();
	
	// lookup table by p3d
	protected ref map<string, ref array<EditorPlaceableObjectData>> m_PlaceableObjectsByP3d = new map<string, ref array<EditorPlaceableObjectData>>();

#ifndef EDITOR_DECLARE
#define EDITOR_DECLARE
	protected ref Editor m_Editor;
	
	Editor GetEditor()
	{
		return m_Editor;
	}
#endif
		
	override void OnUpdate(bool doSim, float timeslice)
	{
		super.OnUpdate(doSim, timeslice);
				
		if (m_Editor) {		
			m_Editor.Update(timeslice);
			if (doSim && m_Editor.IsDirty()) {
				m_Editor.Synchronize();
			}
		}
	}
	
	override bool OnInitialize()
	{				
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
		
		foreach (EditorPlaceableObjectData placeable_item: m_AllPlaceableItems) {
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
	
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, target, rpc_type, ctx);
		
		switch (rpc_type) {					
			case Editor.RPC_CREATE_SESSION: {
				if (GetGame().IsDedicatedServer()) {
					Error("RPC_CREATE_SESSION can only be called on clients");
					break;
				}
				
				PlayerIdentity identity;
				if (!ctx.Read(identity)) {	
					Error("Invalid identity");
					break;
				}
				
				m_Editor = new Editor(identity);
				m_Editor.Read(ctx, 0);
				break;
			}
		}
	}

	EditorPlaceableObjectData GetPlaceableObject(string type)
	{
		return m_PlaceableObjectsByType[type];
	}
	
	array<ref EditorPlaceableObjectData> GetPlaceableObjects()
	{
		return m_AllPlaceableItems;
	}
	
	array<EditorPlaceableObjectData> GetReplaceableObjects(string p3d)
	{
		return m_PlaceableObjectsByP3d[p3d];
	}

	map<EditorObjectDataCategory, ref array<EditorPlaceableObjectData>> GetPlaceableItemsByCategory()
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

	override void OnRespawnEvent(int time)
	{
	}
	
	override void EnterLoginQueue(UIMenuPanel parent)
	{
	}
	
	override void EnterLoginTime(UIMenuPanel parent)
	{
	}
		
	// the weird loading screen order is
	// game starts loading into server. creates the login queue
	// after a period in queue, it deletes the login queue and creates login timer
	// then after login timer is done, loading is done
	
	// Game Loading Screen
	// this function gets called CONSTANTLY while loading in 
	override void UpdateLoginQueue(float timeslice)
	{
		// probably need to make sure we are in the main menu here somehow
	}
	
	// this function only gets called one time when you get loaded into the server,
	// the loginTime is the amount of seconds until the engine will put you in
	override void OnLoginTimeEvent(int loginTime)
	{
		//LoginTimeEvent.Invoke(loginTime);
	}
	
	override void LoginTimeCountdown()
	{
	}
	
	// appears to be the best place to delete the loading screen
	override void CancelLoginTimeCountdown()
	{
		
	}
	
	override void CancelLoginQueue()
	{
	}
}

modded class DayZGame
{
	override void RegisterConversionTemplates(out TypeConversionHashMap type_conversions)
	{
		super.RegisterConversionTemplates(type_conversions);
		type_conversions.Insert(EditorBrushData, TypeConversionBrush);
		type_conversions.Insert(EditorBrushObject, TypeConversionBrushObject);
		type_conversions.Insert(DropdownListPrefabItemBase, DropdownListPrefabItemConverter);
		type_conversions.Insert(EditorFile, TypeConversionEditorFile);
	}
}

class TypeConversionBrush: TypeConversionTemplate<EditorBrushData>
{	
	override void SetString(string value) 
	{
		m_Value.Type = value;
	}
	
	override string GetString() 
	{
		return m_Value.Type;
	}
}

class TypeConversionBrushObject: TypeConversionTemplate<EditorBrushObject>
{
	override void SetString(string value) 
	{
		m_Value.Type = value;
	}
	
	override string GetString() 
	{
		return m_Value.Type;
	}
	
	override Widget GetWidget() 
	{
		return GetGame().GetWorkspace().CreateWidgets("Editor/gui/Layouts/options/EditorDialogOptionEditBrush.layout");
	}
}


class TypeConversionEditorFile: TypeConversionTemplate<EditorFile>
{
	override void SetString(string value) 
	{
		if (m_Value) {
			m_Value.FileName = value;
		}
	}
	
	override string GetString() 
	{
		if (m_Value) {
			return m_Value.FileName;
		}
		
		return string.Empty;
	}
}

class DropdownListPrefabItemConverter: TypeConversionTemplate<DropdownListPrefabItemBase>
{
	override string GetString() 
	{
		if (m_Value) {
			return m_Value.GetText();
		}
		
		return string.Empty;
	}
	
	override Widget GetWidget() 
	{
		// Todo: why can this be null? not sure
		if (m_Value) {
			return m_Value.GetLayoutRoot();
		}
		
		return null;
	}
	
	override void SetWidget(Widget value) 
	{
		m_Value.OnWidgetScriptInit(value);
	}
}