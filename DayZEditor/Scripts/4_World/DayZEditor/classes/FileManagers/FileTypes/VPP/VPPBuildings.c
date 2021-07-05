
class VPPToEditorBuildingSet
{
   private string m_Name;
   private bool   m_IsActive;
   private ref array<ref VPPToEditorSpawnedBuilding> m_Buildings = {};

   void VPPToEditorBuildingSet(string name, bool isAlive = false)
   {
      m_IsActive = isAlive;
      m_Name = name;
   }

   void ~VPPToEditorBuildingSet()
   {
		foreach (VPPToEditorSpawnedBuilding building : m_Buildings)
			if (building)
         		delete building;
   }

   string GetName()
   {
      return m_Name;
   }
	
   void SetName(string newname)
   {
	  m_Name = newname;
   }

   bool GetActive()
   {
      return m_IsActive;
   }

   void GetSpawnedBuildings(inout array<ref VPPToEditorSpawnedBuilding> buildings)
   {
      buildings = m_Buildings;
   }

   void DeleteBuildingByName(string name)
   {
      ref VPPToEditorSpawnedBuilding building = GetBuildingByName(name);

      if(building)
      {
         m_Buildings.RemoveItem(building);
         delete building;
      }
   }

   void SetBuildingsArray(ref array<ref VPPToEditorSpawnedBuilding> data)
   {
	  m_Buildings = data;
   }

   void AddBuilding(string name, vector pos, vector orientation, bool active)
   {
      m_Buildings.Insert(new VPPToEditorSpawnedBuilding(name, pos, orientation, active));
   }
   
   //use only for client when creating new objects
   VPPToEditorSpawnedBuilding AddBuildingObject(string name, vector pos, vector orientation, bool active, Object activeObj)
   {
		ref VPPToEditorSpawnedBuilding result = new VPPToEditorSpawnedBuilding(name, pos, orientation, active, activeObj);
		m_Buildings.Insert(result);
		return result;
   }

   void SetBuildingActive(string name, bool active)
   {
      autoptr VPPToEditorSpawnedBuilding building = GetBuildingByName(name);

      if(building)
         building.SetActive(active);

      if(!building)
         Print("[BuildingSetManager]:: SetBuildingActive: " + name + " is an invalid name.");
   }

	void SetActive(bool active)
	{
		m_IsActive = active;
		
		foreach(VPPToEditorSpawnedBuilding building : m_Buildings)
		{
			if (building != null)
			{
				if (m_IsActive)
					building.SetActive(building.GetActive());
				else
		         	building.DestroySpawnedEntity();
			}
		}
	}

   array<ref VPPToEditorSpawnedBuilding> GetBuildings()
   {
	 return m_Buildings;
   }

   void ClearBuildings()
   {
	    foreach(VPPToEditorSpawnedBuilding buidling : m_Buildings){
			if (buidling)
				delete buidling;
		}
		m_Buildings = new array<ref VPPToEditorSpawnedBuilding>;
   }
	
	void RemoveBuilding(VPPToEditorSpawnedBuilding instance)
	{
		int index = m_Buildings.Find(instance);
		if (index > -1 && m_Buildings.Get(index) != null)
		{
			m_Buildings.RemoveItem(instance);
			delete instance;
		}
	}
	
	//Only use for client, to update all objects within set (pos,orientation)
	void UpdateBuildingsData()
	{
		foreach(VPPToEditorSpawnedBuilding building : m_Buildings)
		{
			bool networkObj = false;
			Object trackerObj = building.GetObject();
			string networkID = building.GetNetworkId();
			if (trackerObj == null)
			{
				//Find object using network ID
				TStringArray strs = new TStringArray;
				networkID.Split( ",",strs );
				trackerObj = GetGame().GetObjectByNetworkId(strs[1].ToInt(), strs[0].ToInt()); //low,high
				networkObj = true;
			}
			
			if (trackerObj == null) continue;
			building.SetPosition(trackerObj.GetPosition());
			building.SetOrientation(trackerObj.GetOrientation());
			
			if (!networkObj)
				GetGame().ObjectDelete(trackerObj);
		}
	}
	
	void ExportBuildings()
	{
		FileHandle m_FileHandle;
		m_FileHandle = OpenFile("$profile:VPPAdminTools/Exports/"+m_Name+".txt", FileMode.WRITE);
		if (m_FileHandle == 0) return;

		string function = "void SpawnObject(string objType, vector objPos, vector objOrientation)\n";
		function 		+= "{\n";
		function 		+= "\tObject m_Building = Object.Cast(GetGame().CreateObject(objType, objPos));\n";
		function 		+= "\tif (m_Building == null) return;\n";
		function 		+= "\tm_Building.SetAffectPathgraph(true, false);\n";
		function 		+= "\tGetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, m_Building);\n";
		function 		+= "\tm_Building.SetPosition(objPos);\n";
		function 		+= "\tm_Building.SetOrientation(objOrientation);\n";
		function 		+= "}\n\n\n";
		
		FPrintln(m_FileHandle, function);
		
		foreach(VPPToEditorSpawnedBuilding building: m_Buildings)
		{
			string format = "SpawnObject( \"" + building.GetTypeName() + "\", \"" + building.GetPosition().ToString(false) + "\", \"" + building.GetOrientation().ToString(false) + "\" );";
			FPrintln(m_FileHandle, format);
		}
		
		CloseFile(m_FileHandle);
	}

	VPPToEditorSpawnedBuilding GetVPPToEditorSpawnedBuilding(VPPToEditorSpawnedBuilding compare)
	{
	  foreach(VPPToEditorSpawnedBuilding building : m_Buildings)
      {
         if(building.GetName() == compare.GetName() || building == compare)
         {
            return building;
         }
      }
      return null;
	}
	
   VPPToEditorSpawnedBuilding GetBuildingByName(string name)
   {
      foreach(VPPToEditorSpawnedBuilding building : m_Buildings)
      {
         if(building.GetName() == name)
         {
            return building;
         }
      }
      return null;
   }
	
   VPPToEditorSpawnedBuilding GetBuildingByType(Object compare)
   {
	  foreach(VPPToEditorSpawnedBuilding building : m_Buildings)
      {
         if(building.GetObject() == compare)
         {
            return building;
         }
      }
      return null;
   }
	
   VPPToEditorSpawnedBuilding GetBuildingByNetworkID(string id)
   {
	  foreach(VPPToEditorSpawnedBuilding building : m_Buildings)
      {
         if(building.GetNetworkId() == id)
         {
            return building;
         }
      }
      return null;
   }
}


class VPPToEditorSpawnedBuilding
{
	private string m_Name;
	private string m_ObjectName;
	private vector m_Position;
	private vector m_Direction;
	private vector m_Orientation;
	private bool   m_IsActive;
	
	[NonSerialized()]
	private Object m_Building;
	private string m_NetWorkId;

	void VPPToEditorSpawnedBuilding(string name, vector pos, vector orientation, bool isAlive = true, Object obj = null)
	{
		m_IsActive = isAlive;
		m_ObjectName = name;
		m_Position = pos;
		m_Orientation = orientation;

		string posStr = Math.Floor(pos[0]).ToString() + Math.Floor(pos[1]).ToString() + Math.Floor(pos[2]).ToString();
		m_Name = name + "-" + posStr;
		
		if (obj != null)
			m_Building = obj;
		else
			SetActive(isAlive);
	}

	void ~VPPToEditorSpawnedBuilding()
	{
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			if (m_Building) {
				DestroySpawnedEntity();
			} else {
				TStringArray strs = new TStringArray;
				m_NetWorkId.Split( ",",strs );
				autoptr Object netWrkObj = GetGame().GetObjectByNetworkId(strs[1].ToInt(), strs[0].ToInt()); //low,high
				GetGame().ObjectDelete(netWrkObj);
			}
		}

		if (GetGame().IsClient())
		{
			if (m_Building)
				GetGame().ObjectDeleteOnClient(m_Building);
		}
	}
	
	string GetTypeName()
	{
		return m_ObjectName;
	}
	
	string GetName()
	{
		return m_Name;
	}
	
	Object GetObject()
	{
		return m_Building;
	}
	
	vector GetPosition()
	{
		return m_Position;
	}
	
	void SetPosition(vector pos)
	{
		m_Position = pos;
		
		if(m_Building)
			m_Building.SetPosition(pos);
	}
	
	vector GetOrientation()
	{
		return m_Orientation;
	}
	
	void SetOrientation(vector o)
	{
		m_Orientation = o;
		
		if(m_Building)
			m_Building.SetOrientation(o);
	}
	
	vector GetDirection()
	{
		return m_Direction;
	}

	void SetDirection(vector d)
	{
		m_Direction = d;
		
		if(m_Building)
			m_Building.SetDirection(d);
	}

	bool GetActive()
	{
		return m_IsActive;
	}
		
	void SetActive(bool active)
	{
		m_IsActive = active;

		if( !m_IsActive)
		{
			DestroySpawnedEntity();
		}

		if (m_IsActive)
		{
			if (!m_Building)
			{
				SpawnObject();
			}
		}
	}
	
	void DestroySpawnedEntity()
	{
		if (m_Building != null)
			GetGame().ObjectDelete(m_Building);
	}
	
	bool IsObject(Object obj)
	{
		if (m_Building)
			return m_Building == obj;
		
		return m_ObjectName == obj.GetDisplayName();
	}
	
	string GetNetworkId(bool clean = false)
	{
		if (clean) {
			string edited = m_NetWorkId;
			edited.Replace(",", "");
			return edited;
		}
		
		return m_NetWorkId;
	}

	void SpawnObject()
	{
		m_Building = Object.Cast(GetGame().CreateObject(m_ObjectName, m_Position));
		if (m_Building == null) return;
		
		int low, high;
		m_Building.GetNetworkID( low, high );
		m_NetWorkId = high.ToString() + "," + low.ToString();
		if (m_Building.CanAffectPathgraph())
		{
			m_Building.SetAffectPathgraph(true, false);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, m_Building);
			m_Building.SetPosition(m_Position);
			m_Building.SetOrientation(m_Orientation);
		}
	}
}