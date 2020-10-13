
class EditorXMLCallback<Class T>: XMLCallback
{
	private T m_Data;
	T GetData() { return m_Data; }

	private bool m_Success = false;
	override void OnFailure(ref XMLDocument document)
	{
		EditorLog.Trace("EditorXMLCallback::OnFailure");
		m_Failed = false;
	}
	
	override void OnSuccess(ref XMLDocument document)
	{
		EditorLog.Trace("EditorXMLCallback::OnSuccess");
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
	
	void ~EditorLootContainer()
	{
		delete m_LootPoints;
		delete m_Category;
		delete m_Tag;
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
}

class EditorMapGroupProto: XMLCallback
{
	
	ref array<ref EditorMapGroupProtoGroup> m_MapGroupProto = {};
	
	ref array<EditorObject> m_LootPositions = {};
	
	array<EditorObject> GetLootSpawns() {
		return m_LootPositions;
	}
	
	private Object m_Building;
	void EditorMapGroupProto(Object building)
	{
		m_Building = building;
	}
	
	void ~EditorMapGroupProto()
	{
		foreach (EditorObject editor_object: m_LootPositions) {
			GetEditor().DeleteObject(editor_object, false);
		}
		
		delete m_LootPositions;
		delete m_MapGroupProto;
	}
		
	override void OnSuccess(ref XMLDocument document)
	{
		EditorLog.Trace("EditorMapGroupProto::OnSuccess"); 
		
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
		foreach (EditorMapGroupProtoGroup group_proto: m_MapGroupProto) {
			
			if (group_proto && m_Building && group_proto.GetName() == m_Building.GetType()) {
				EditorLog.Info("Building Found!");
				ref array<ref EditorLootContainer> loot_containers = group_proto.GetLootContainer();
				
				foreach (EditorLootContainer loot_container: loot_containers) {
						
					ref array<ref EditorLootPoint> loot_points = loot_container.GetLootPoints();
					foreach (EditorLootPoint loot_point: loot_points) {

						
					}
				}
								
				return;
			}			
		}
		
		EditorLog.Error("Building was not found!");
		
		thread ErrorMessage(string.Format("%1 not found in MapGroupProto.xml", m_Building.GetType()));
		GetEditor().FinishEditLootSpawns();
	}
	
	void InsertLootPoint(EditorLootPoint loot_point)
	{
		vector loot_pos = loot_point.GetPosition();					
		loot_pos = Vector(loot_pos[2], loot_pos[1], loot_pos[0]);
		EditorObject loot_display = GetEditor().CreateObject(EditorObjectData.Create("DebugCylinder", loot_pos, vector.Zero, EditorObjectFlags.OBJECTMARKER));
		
		// might be bad
		//m_Building.AddChild(loot_display.GetWorldObject(), -1);
		
		vector transform[4] = {
			Vector(1, 0, 0),
			Vector(0, 1, 0),
			Vector(0, 0, 1),
			Vector(loot_pos[2], loot_pos[1], loot_pos[0]) + m_Building.GetPosition()
		};
		
		transform[0][0] = loot_point.GetRange();
		transform[1][1] = loot_point.GetHeight();
		transform[2][2] = loot_point.GetRange();
		
		loot_display.SetTransform(transform);
		m_LootPositions.Insert(loot_display);
		
		m_Building.Update();
	}
	
	private void ErrorMessage(string message)
	{	
		MessageBox.Show("Error", message, MessageBoxButtons.OK);
	}
}




class EditorXMLManager
{

	static void LoadMapGroupProto(out EditorMapGroupProto group_proto, string filename = "$profile:Editor/mapgroupproto.xml")
	{
		GetXMLApi().Read(filename, group_proto);
	}
}
