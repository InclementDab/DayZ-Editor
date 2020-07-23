

class TranslationWidget: BuildingBase
{
	private EditorObject m_ParentObject;
	
	private vector m_Position;
	
	void TranslationWidget()
	{
		Print("TranslationWidget");
		//TStringArray textures = GetHiddenSelectionsTextures();
		//SetObjectTexture(GetHiddenSelectionIndex("TranslateY"), textures[0]);
		//SetObjectTexture(GetHiddenSelectionIndex("TranslateZ"), textures[0]);		
		
	}
	
	
	void ~TranslationWidget()
	{
		Print("~TranslationWidget");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(ThisGameSucks);
	}

	
	
	
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		
	}

	void ThisGameSucks()
	{
		Input input = GetGame().GetInput();
		
		vector transform[4];
		float scale = vector.Distance(GetGame().GetCurrentCameraPosition(), m_ParentObject.GetPosition()) / 15;
		
		transform[0][0] = scale;
		transform[1][1] = scale;
		transform[2][2] = scale;
		transform[3] = m_Position;
		
		
		vector start_pos = GetGame().GetCurrentCameraPosition();
		vector end_pos = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * OBJECT_VIEW_DISTANCE;
		SetTransform(transform);
		SetOrientation(-m_ParentObject.GetOrientation());
		
		vector contact_pos, contact_dir;
		int contact_comp;
		set<Object> o = new set<Object>();
		DayZPhysics.RaycastRV(start_pos, end_pos, contact_pos, contact_dir, contact_comp, o);
		
		if (o[0] == this) 
			OnMouseEnter(contact_comp);
		else 
			OnMouseLeave(contact_comp);
		
		if (input.LocalPress("UAFire") && mouse_inside) {
			OnMouseButtonDown(contact_comp);
		} else if (input.LocalRelease("UAFire")) {
			OnMouseButtonUp(contact_comp);
		}
	}
	
	
	
	void SetEditorObject(EditorObject parent_object, vector position)
	{
		m_Position = position;
		m_ParentObject = parent_object;
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(ThisGameSucks);
		m_ParentObject.AddChild(this, -1);
		m_ParentObject.Update();
	}	
	
	
	void OnMouseEnter(int index)
	{
		
		if (mouse_inside) return;
		mouse_inside = true;
		Print("TranslationWidget::OnMouseEnter");
	}
	
	void OnMouseLeave(int index)
	{
		
		if (!mouse_inside) return;
		mouse_inside = false;
		Print("TranslationWidget::OnMouseLeave");		
	}
	
	
	void OnMouseButtonDown(int index)
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DelayedDragCall, 40);
	}
	
	void OnMouseButtonUp(int index)
	{
		
	}
	
	void DelayedDragCall()
	{
		Input input = GetGame().GetInput();
		
		if (input.LocalValue("UAFire")) {
			EditorEvents.DragInvoke(this, m_ParentObject);
		}
	}
	
	

	private bool mouse_inside = false;
	bool IsMouseInside()
	{
		return mouse_inside;
	}
	
}