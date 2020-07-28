

class TranslationWidget: ItemBase
{
	private EditorObject m_ParentObject = null;
	private Object m_RotationWidget;
	
	void TranslationWidget()
	{
		Print("TranslationWidget");
		Print(ClearEventMask(EntityEvent.ALL));		
		//TStringArray textures = GetHiddenSelectionsTextures();
		//SetObjectTexture(GetHiddenSelectionIndex("TranslateY"), textures[0]);
		//SetObjectTexture(GetHiddenSelectionIndex("TranslateZ"), textures[0]);		
		//m_RotationWidget = GetGame().CreateObjectEx("RotationWidget", vector.Zero, ECE_NONE);
		//AddChild(m_RotationWidget, -1);
		
		
	}

	override void EEInit()
	{
		super.EEInit();
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(UpdateFrame);
	}
	
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(UpdateFrame);
		
	}
	
	void UpdateFrame()
	{
	
		if (m_ParentObject == null) return;

		// Scale object to camera distance
		float scale = vector.Distance(GetGame().GetCurrentCameraPosition(), GetPosition()) / 20;
		vector transform[4];
		GetTransform(transform);
		transform[0][0] = scale;
		transform[1][1] = scale;
		transform[2][2] = scale;
		SetTransform(transform);
		Update();
	}
	
	
	void SetTranslationPosition(vector pos) 
	{
		SetPosition(pos);
		Update();
		pos[1] = pos[1] - m_ParentObject.GetSize()[1] / 2;
		m_ParentObject.SetPosition(pos);
		m_ParentObject.Update();
	}
	
	void UpdatePosition()
	{
		vector pos;
		pos = m_ParentObject.GetPosition();
		pos[1] = pos[1] + m_ParentObject.GetSize()[1] / 2;
		SetPosition(pos);
		Update();
		
	}

	void ~TranslationWidget()
	{
		Print("~TranslationWidget");
		GetGame().ObjectDelete(m_RotationWidget);
	}



	void SetEditorObject(EditorObject parent_object)
	{
		m_ParentObject = parent_object;
		vector start_pos = m_ParentObject.GetPosition();
		start_pos[1] = start_pos[1] + m_ParentObject.GetSize()[1] / 2;
		SetPosition(start_pos);
	}
	
	

	private bool mouse_inside = false;
	bool IsMouseInside() { return mouse_inside; }
	
	EditorObject GetEditorObject() { return m_ParentObject; }
}


