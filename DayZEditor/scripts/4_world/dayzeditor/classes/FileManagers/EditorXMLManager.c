
class EditorXMLCallback<Class T>: XMLCallback
{
	private T m_Data;
	T GetData() { return m_Data; }

	private bool m_Success = false;
	override void OnFailure(ref XMLDocument document)
	{
		Print("EditorXMLCallback::OnFailure");
		m_Failed = false;
	}
	
	override void OnSuccess(ref XMLDocument document)
	{
		Print("EditorXMLCallback::OnSuccess");
		m_Success = true;
	}
	
	bool GetSuccess() { return m_Success; }
}


class EditorLootPoint
{
	private vector m_Position;
	private float m_Range, m_Height;
	private int m_Flags;
	
	void EditorLootPoint(vector position, float range, float height, int flags) 
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
	private string m_Name;
	private int m_Lootmax;
	
	private ref array<string> m_Category;
	private ref array<string> m_Tag;
	private ref array<ref EditorLootPoint> m_LootPoints;
	ref array<ref EditorLootPoint> GetLootPoints() { return m_LootPoints; }
	
	void EditorLootContainer(string name, int lootmax)
	{
		m_Name = name; m_Lootmax = lootmax;
		m_Category = new array<string>();
		m_Tag = new array<string>();
		m_LootPoints = new array<ref EditorLootPoint>();
	}
	
	int InsertCategory(string category)
	{
		m_Category.Insert(category);
		return m_Category.Count();
	}	
	
	int InsertTag(string tag)
	{
		m_Tag.Insert(tag);
		return m_Tag.Count();
	}	
	
	int InsertLootPoint(EditorLootPoint lootpoint)
	{
		m_LootPoints.Insert(lootpoint);
		return m_LootPoints.Count();
	}
	
}


class EditorMapGroupProtoGroup
{
	private string m_Name;
	string GetName() { return m_Name; }
	private int m_Lootmax;
	
	private ref array<string> m_Usage;
	private ref array<ref EditorLootContainer> m_LootContainer;
	ref array<ref EditorLootContainer> GetLootContainer() { return m_LootContainer; }
	
	void EditorMapGroupProtoGroup(string name, int lootmax)
	{
		m_Name = name; m_Lootmax = lootmax;
		m_Usage = new array<string>();
		m_LootContainer = new array<ref EditorLootContainer>();
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
}

class EditorMapGroupProto: XMLCallback
{
	
	ref array<ref EditorMapGroupProtoGroup> m_MapGroupProto;
	ref array<ref EditorMapGroupProtoGroup> GetData() { return m_MapGroupProto; }
	
	private Object m_Building;
	void EditorMapGroupProto(Object building)
	{
		m_Building = building;
	}
		
	override void OnSuccess(ref XMLDocument document)
	{
		Print("EditorMapGroupProto::OnSuccess");
		m_MapGroupProto = new array<ref EditorMapGroupProtoGroup>();
		
		XMLElement prototype = document.Get(1).GetContent();
		
		for (int i = 0; i < prototype.Count(); i++) {
			
			XMLTag group_tag = prototype.Get(i);
			if (group_tag.GetName() != "group") 
				continue;
			
			XMLAttribute group_name = group_tag.GetAttribute("name");
			XMLAttribute group_lootmax = group_tag.GetAttribute("lootmax");
			
			if (group_name != null) 
				string grp_name = group_name.ValueAsString();
			
			if (group_lootmax != null)
				int grp_lootmax = group_lootmax.ValueAsInt();
			
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
						if (container_name != null)
							string cont_name = container_name.ValueAsString();
						if (container_lootmax != null)
							int cont_lootmax = container_lootmax.ValueAsInt();
						EditorLootContainer container = new EditorLootContainer(cont_name, cont_lootmax);
						
						XMLElement container_children = group_child.GetContent();
						for (int k = 0; k < container_children.Count(); k++) {
							
							XMLTag container_child = container_children.Get(k);

							switch (container_child.GetName()) {
								
								case "category": {
									XMLAttribute category_name = container_child.GetAttribute("name");
									container.InsertCategory(category_name.ValueAsString());
									break;
								}
								
								case "tag": {
									XMLAttribute tag_name = container_child.GetAttribute("name");
									container.InsertTag(tag_name.ValueAsString());
									break;
								}
								
								case "point": {
									XMLAttribute point_pos 		= container_child.GetAttribute("pos");
									XMLAttribute point_range 	= container_child.GetAttribute("range");
									XMLAttribute point_height 	= container_child.GetAttribute("height");
									XMLAttribute point_flags 	= container_child.GetAttribute("flags");
									
									if (point_pos != null) 
										vector pos = point_pos.ValueAsVector(); 
									
									if (point_range != null)
										float range = point_range.ValueAsFloat(); 
									
									if (point_height != null)
										float height = point_height.ValueAsFloat();
									
									if (point_flags != null)
										int flags = point_flags.ValueAsInt();
									
									container.InsertLootPoint(new EditorLootPoint(pos, range, height, flags));
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
		foreach (ref EditorMapGroupProtoGroup group_proto: m_MapGroupProto) {
			
			if (group_proto.GetName() == m_Building.GetType()) {
				Print("Building Found!");
				ref array<ref EditorLootContainer> loot_containers = group_proto.GetLootContainer();
				
				foreach (EditorLootContainer loot_container: loot_containers) {
						
					ref array<ref EditorLootPoint> loot_points = loot_container.GetLootPoints();
					foreach (EditorLootPoint loot_point: loot_points) {
						vector loot_pos = loot_point.GetPosition();
						
						//Object loot_display = GetGame().CreateObjectEx("DebugCylinder", Vector(-loot_pos[2], loot_pos[1], loot_pos[0]), ECE_NONE);
						
						
						EditorObject loot_display = GetEditor().GetObjectManager().CreateObject(new EditorObjectData("DebugCylinder", Vector(-loot_pos[2], loot_pos[1], loot_pos[0]), vector.Zero, EditorObjectFlags.OBJECTMARKER | EditorObjectFlags.BBOX));
						
						// might be bad
						m_Building.AddChild(loot_display.GetWorldObject(), -1);
						
						
						
						vector transform[4] = {
							Vector(1, 0, 0),
							Vector(0, 1, 0),
							Vector(0, 0, 1),
							Vector(-loot_pos[2], loot_pos[1], loot_pos[0])
						};
						
						transform[0][0] = loot_point.GetRange() * 2.0;
						transform[1][1] = loot_point.GetHeight() * 2.0;
						transform[2][2] = loot_point.GetRange() * 2.0;
						
						loot_display.SetTransform(transform);
						
						m_Building.Update();
					}
				}
								
				return;
			}			
		}
		
		Print("Building was not found!");
	}
}

// abstract to EditorXMLCallback
class XMLEditorBrushes: XMLCallback
{
	private ref EditorBrushSettingsSet m_Data = new EditorBrushSettingsSet();
	EditorBrushSettingsSet GetData() { return m_Data; }
	
	private bool m_Success = true;
	
	void XMLEditorBrushes()
	{
		Print("XMLEditorBrushes");
	}
		
	override void OnSuccess(ref XMLDocument document)
	{
		EditorPrint("XMLEditorBrushes::OnSuccess");
		m_Success = true;
		
		XMLElement brush_types = document.Get(1).GetContent();
		
		ref set<string> object_type_list = new set<string>();
		// <BrushTypes>
		for (int i = 0; i < brush_types.Count(); i++) {
			
			XMLTag brush = brush_types.Get(i);
			XMLElement brush_objects = brush.GetContent();
			
			EditorBrushSettings brush_settings = new EditorBrushSettings();
			brush_settings.Name = brush.GetAttribute("name").ValueAsString();

			
			
			// <BrushObject>
			for (int j = 0; j < brush_objects.Count(); j++) {
				XMLTag brush_object = brush_objects.Get(j);
				
				// attributes
				string object_type;
				float object_frequency;
				
				// type attribute
				XMLAttribute object_type_attribute = brush_object.GetAttribute("type");
				if (object_type_attribute == null) {
					EditorPrint("XMLEditorBrushes: Object type not specified, skipping...");
					continue;
				}
				
				object_type = object_type_attribute.ValueAsString();
				if (object_type_list.Insert(object_type) == -1) {
					EditorPrint("XMLEditorBrushes: Duplicate brush name found, skipping...");
					continue;
				}
				
				// frequency attribute
				XMLAttribute object_frequency_attribute = brush_object.GetAttribute("frequency");
				if (object_frequency_attribute == null) {
					object_frequency = 1.0;
				} else {
					object_frequency = object_frequency_attribute.ValueAsFloat();
				}
				
				brush_settings.InsertPlaceableObject(object_type, object_frequency);
				
				
			}
				
			m_Data.Insert(brush_settings);
		}
		
		GetEditor().SetBrushTypes(m_Data);
	}
	
	override void OnFailure(ref XMLDocument document)
	{
		Print("XMLEditorBrushes::OnFailure");
		m_Success = false;
	}
	
}


class EditorXMLManager
{

	static void LoadMapGroupProto(out ref EditorMapGroupProto group_proto, string filename = "$profile:Editor/mapgroupproto.xml")
	{
		GetXMLApi().Read(filename, group_proto);
		
		//return group_proto.GetSuccess();
	}
	
	static void LoadBrushes(out ref XMLEditorBrushes brush_set, string filename)
	{
		GetXMLApi().Read(filename, brush_set);
		
	}

}
