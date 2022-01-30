class EditorLootPointData
{
	protected vector m_Position;
	protected float m_Range, m_Height;
	protected int m_Flags;
	
	void EditorLootPointData(vector position, float range, float height, int flags) 
	{
		m_Position = position;
		m_Range = range;
		m_Height = height;
		m_Flags = flags;
	}
	
	vector GetPosition() { return m_Position; }
	
	float GetRange() { return m_Range; }
	float GetHeight() { return m_Height; }
}

class EditorLootContainer
{
	protected string m_Name;
	protected int m_Lootmax;
	
	ref array<string> Categories = {};
	ref array<string> Tags = {};
	ref array<ref EditorLootPointData> LootPoints = {};
	
	void EditorLootContainer(string name, int lootmax)
	{
		m_Name = name; m_Lootmax = lootmax;
	}
	
	void ~EditorLootContainer()
	{
		delete LootPoints;
		delete Categories;
		delete Tags;
	}
}


class EditorMapGroupProtoGroup
{
	protected string m_Name;
	protected int m_Lootmax;
	
	protected ref array<string> m_Usage;
	protected ref array<ref EditorLootContainer> m_LootContainer;
	
	void EditorMapGroupProtoGroup(string name, int lootmax)
	{
		m_Name = name; m_Lootmax = lootmax;
		m_Usage = new array<string>();
		m_LootContainer = new array<ref EditorLootContainer>();
	}
	
	void ~EditorMapGroupProtoGroup()
	{
		delete m_LootContainer;
		delete m_Usage;
	}
	
	int InsertUsage(string usage)
	{
		m_Usage.Insert(usage);
		return m_Usage.Count();
	}
	
	int InsertLootContainer(EditorLootContainer container)
	{
		m_LootContainer.Insert(container);
		return m_LootContainer.Count();
	}
	
	array<ref EditorLootContainer> GetLootContainer() 
	{ 
		return m_LootContainer; 
	}
	
	string GetName() 
	{ 
		return m_Name; 
	}
}

class EditorMapGroupProto: XMLCallback
{
	protected Object m_Building;
	
	ref array<ref EditorMapGroupProtoGroup> m_MapGroupProto = {};
	
	ref array<EditorObject> m_LootPositions = {};
		
	void EditorMapGroupProto(Object building)
	{
		m_Building = building;
		
		EditorEvents.OnObjectDeleted.Insert(OnObjectDeleted);
	}
	
	void ~EditorMapGroupProto()
	{
		foreach (EditorObject editor_object: m_LootPositions) {
			GetEditor().DeleteObject(editor_object, false);
		}
		
		delete m_LootPositions;
		delete m_MapGroupProto;
	}
	
	array<EditorObject> GetLootSpawns() 
	{
		return m_LootPositions;
	}
	
	Object GetBuilding() 
	{
		return m_Building;
	}
	
	void OnObjectDeleted(Class context, EditorObject target)
	{
		m_LootPositions.RemoveItem(target);
	}
		
	override void OnSuccess(XMLDocument document)
	{
		EditorLog.Trace("EditorMapGroupProto::OnSuccess"); 
		
		if (document[1] == null) return;
		XMLElement prototype = document[1].GetContent();
		
		for (int i = 0; i < prototype.Count(); i++) {
			
			XMLTag group_tag = prototype[i];
			if (group_tag.GetName() != "group") {
				continue;
			}
						
			string grp_name;
			if (group_tag.GetAttribute("name")) {
				grp_name = group_tag.GetAttribute("name").ValueAsString();
			}
			
			int grp_lootmax;
			if (group_tag.GetAttribute("lootmax")) {
				grp_lootmax = group_tag.GetAttribute("lootmax").ValueAsInt();
			}
			
			EditorMapGroupProtoGroup group = new EditorMapGroupProtoGroup(grp_name, grp_lootmax);
			XMLElement group_children = group_tag.GetContent();
			
			for (int j = 0; j < group_children.Count(); j++) {
				
				XMLTag group_child = group_children.Get(j);				
				switch (group_child.GetName()) {
				
					case "usage": {
						XMLAttribute usage_name = group_child.GetAttribute("name");
						group.InsertUsage(usage_name.ValueAsString());
						break;
					}
					
					case "container": {
						XMLAttribute container_name = group_child.GetAttribute("name");
						XMLAttribute container_lootmax = group_child.GetAttribute("lootmax");
						
						string cont_name;
						int cont_lootmax;
						
						if (container_name) {
							cont_name = container_name.ValueAsString();
						}
						
						if (container_lootmax) {
							cont_lootmax = container_lootmax.ValueAsInt();
						}
						
						EditorLootContainer container = new EditorLootContainer(cont_name, cont_lootmax);
						XMLElement container_children = group_child.GetContent();
						for (int k = 0; k < container_children.Count(); k++) {
							
							XMLTag container_child = container_children.Get(k);

							switch (container_child.GetName()) {
								
								case "category": {
									container.Categories.Insert(container_child.GetAttribute("name").ValueAsString());
									break;
								}
								
								case "tag": {
									container.Tags.Insert(container_child.GetAttribute("name").ValueAsString());
									break;
								}
								
								case "point": {									
									vector pos;
									float range, height;
									int flags;
									
									if (container_child.GetAttribute("pos")) {
										pos = container_child.GetAttribute("pos").ValueAsVector(); 
									}
									
									if (container_child.GetAttribute("range")) {
										range = container_child.GetAttribute("range").ValueAsFloat(); 
									}
									
									if (container_child.GetAttribute("height")) {
										height = container_child.GetAttribute("height").ValueAsFloat();
									}
									
									if (container_child.GetAttribute("flags")) {
										flags = container_child.GetAttribute("flags").ValueAsInt();
									}
									
									pos = pos + m_Building.GetPosition();
									container.LootPoints.Insert(new EditorLootPointData(pos, range, height, flags));
									break;
								}
								
								default: {
									continue;
								}
							}
						}
						
						group.InsertLootContainer(container);
						
						break;
					}
					
					default: {
						continue;
					}
				}
			}
						
			m_MapGroupProto.Insert(group);
		}
		
		
		// Draw objects 
		foreach (EditorMapGroupProtoGroup group_proto: m_MapGroupProto) {
			if (group_proto && m_Building && group_proto.GetName() == m_Building.GetType()) {
				EditorLog.Info("Building Found!");	
				array<ref EditorLootContainer> containers = group_proto.GetLootContainer();
				foreach (EditorLootContainer cont: containers) {
					foreach (EditorLootPointData loot_point: cont.LootPoints) {
						InsertLootPoint(loot_point);
					}
					
					EditorLog.Info("Loading %1 loot points for %2", cont.LootPoints.Count().ToString(), m_Building.GetType());
				}
				return;
			}			
		}
		
		EditorLog.Warning("Building was not found!");
	}
	
	void InsertLootPoint(EditorLootPointData loot_point_data)
	{
		EditorLog.Info("Inserting Loot Point %1", loot_point_data.GetPosition().ToString());
		vector loot_pos = loot_point_data.GetPosition();	
		EditorObject loot_display = GetEditor().CreateObject(EditorObjectData.Create("EditorLootPoint", loot_pos, vector.Zero, 1, EditorObjectFlags.OBJECTMARKER));

		EditorLootPoint loot_point = EditorLootPoint.Cast(loot_display.GetWorldObject());
		loot_point.Range = loot_point_data.GetRange();
		loot_point.Height = loot_point_data.GetHeight();
				
		vector transform[4] = {
			Vector(1, 0, 0),
			Vector(0, 1, 0),
			Vector(0, 0, 1),
			loot_pos
		};
		
		
		transform[0][0] = loot_point.Range;
		transform[1][1] = loot_point.Height;
		transform[2][2] = loot_point.Range;
		
		loot_display.SetTransform(transform);
		m_LootPositions.Insert(loot_display);
		
		m_Building.Update();
	}
	
	protected void ErrorMessage(string message)
	{	
		MessageBox.Show("Error", message, MessageBoxButtons.OK);
	}
}

class EditorXMLManager
{
	static void LoadMapGroupProto(out EditorMapGroupProto group_proto, string filename)
	{
		GetXMLApi().Read(filename, group_proto);
	}
}

class EditorXMLCallback<Class T>: XMLCallback
{
	protected T m_Data;
	T GetData() { return m_Data; }
	

	protected bool m_Success = false;
	override void OnFailure(XMLDocument document)
	{
		EditorLog.Trace("EditorXMLCallback::OnFailure");
		m_Failed = false;
	}
	
	override void OnSuccess(XMLDocument document)
	{
		EditorLog.Trace("EditorXMLCallback::OnSuccess");
		m_Success = true;
	}
	
	bool GetSuccess() { return m_Success; }
	
	void LoadData(out Class data, XMLDocument document)
	{
		typename type = data.Type();
				
		for (int i = 0; i < type.GetVariableCount(); i++) {
			typename var_type = type.GetVariableType(i);
			
		}
	}
}
