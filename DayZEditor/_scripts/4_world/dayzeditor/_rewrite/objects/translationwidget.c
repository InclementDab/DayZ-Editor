
class RotationWidget: ItemBase
{
	private EditorObject m_ParentObject = null;
	void RotationWidget()
	{
		Print("RotationWidget");
	}
	
	
	void SetEditorObject(EditorObject parent_object)
	{
		m_ParentObject = parent_object;
	}
	
	void SetRotation(vector ori) 
	{
		m_ParentObject.SetOrientation(ori);
		m_ParentObject.Update();
		
		//SetPosition(m_ParentObject.GetTopCenter());
	}
	
	EditorObject GetEditorObject() { return m_ParentObject; }
}



class TranslationWidget: ItemBase
{
	private EditorObject m_ParentObject = null;
	private RotationWidget m_RotationWidget;
	
	void TranslationWidget()
	{
		Print("TranslationWidget");
		
		m_RotationWidget = RotationWidget.Cast(GetGame().CreateObjectEx("RotationWidget", vector.Zero, ECE_NONE));
		AddChild(m_RotationWidget, -1);
		Update();
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
		
		//UpdatePosition();
	}
	
	void UpdatePosition()
	{
		SetPosition(m_ParentObject.GetTopCenter());
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
		SetPosition(m_ParentObject.GetTopCenter());
		m_RotationWidget.SetEditorObject(m_ParentObject);
	}
	
	RotationWidget GetRotationWidget()
	{
		return m_RotationWidget;
	}
	
	private bool mouse_inside = false;
	bool IsMouseInside() { return mouse_inside; }
	
	EditorObject GetEditorObject() { return m_ParentObject; }
}


