
class VPPToEditorBuildingSet
{
   private string m_Name;
   private bool   m_IsActive;
   private ref array<ref VPPToEditorSpawnedBuilding> m_Buildings;

   void VPPToEditorBuildingSet(string name, bool isAlive = false)
   {
      m_IsActive = isAlive;
      m_Name = name;
      m_Buildings = new array<ref VPPToEditorSpawnedBuilding>;
   }

   void ~VPPToEditorBuildingSet()
   {
      foreach (VPPToEditorSpawnedBuilding building : m_Buildings)
      {
		if (building != null)
         	delete building;
      }
   }


    void AddBuilding(string name, vector pos, vector orientation, bool active)
    {
        m_Buildings.Insert(new VPPToEditorSpawnedBuilding(name, pos, orientation, active));
    }
	
	void SetActive(bool active)
	{
		m_IsActive = active;
	}
	
	void GetSpawnedBuildings(inout array<ref VPPToEditorSpawnedBuilding> buildings)
    {
       buildings = m_Buildings;
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
	}
	
	
}