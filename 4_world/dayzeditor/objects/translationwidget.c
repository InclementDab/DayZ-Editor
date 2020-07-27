

class TranslationWidget: ItemBase
{
	private EditorObject m_ParentObject;
	private Object m_RotationWidget;
	private vector m_Position;
	
	
	// made this into an IEntity by mistake and the rotation object placed...
	void TranslationWidget()
	{
		Print("TranslationWidget");
		//GetOnViewIndexChanged();
		
		
		//TStringArray textures = GetHiddenSelectionsTextures();
		//SetObjectTexture(GetHiddenSelectionIndex("TranslateY"), textures[0]);
		//SetObjectTexture(GetHiddenSelectionIndex("TranslateZ"), textures[0]);		
		//m_RotationWidget = GetGame().CreateObjectEx("RotationWidget", vector.Zero, ECE_NONE);
		//AddChild(m_RotationWidget, -1);
	}
	
	override void EEDelete(EntityAI parent)
	{
		GetGame().ObjectDelete(m_RotationWidget);
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(ThisGameSucks);
	}
	
	void ~TranslationWidget()
	{
		Print("~TranslationWidget");
		GetGame().ObjectDelete(m_RotationWidget);
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(ThisGameSucks);
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
		vector end_pos = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * EditorSettings.OBJECT_VIEW_DISTANCE;
		SetTransform(transform);
		SetOrientation(-m_ParentObject.GetOrientation());
		//SetOrientation(vector.Up);
		
		vector contact_pos, contact_dir;
		int contact_comp;
		set<Object> o = new set<Object>();
		DayZPhysics.RaycastRV(start_pos, end_pos, contact_pos, contact_dir, contact_comp, o);
		
		
		
		if (o[0] == this || o[0] == m_RotationWidget) 
			OnMouseEnter(contact_comp);
		else 
			OnMouseLeave(contact_comp);
		
		if (input.LocalPress("UAFire") && mouse_inside) {
			OnMouseButtonDown(o[0].GetActionComponentName(contact_comp));
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
	
	string motion_type;
	void OnMouseButtonDown(string index)
	{
		motion_type = index;
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DelayedDragCall, 40);
	}
	
	void OnMouseButtonUp(int index)
	{
		bool is_working = false;
	}
	
	void DelayedDragCall()
	{
		Input input = GetGame().GetInput();
		
		if (input.LocalValue("UAFire")) {
			//EditorEvents.DragInvoke(this, m_ParentObject);
			set <Object> o;
			object_offset = m_ParentObject.GetPosition() - MousePosToRay(o);
			GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(DragUpdate);
			
		}
	}
	
	vector object_offset;
	void DragUpdate()
	{
		Input input = GetGame().GetInput();
		if (input.LocalRelease("UAFire")) {
			GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(DragUpdate);
			return;
		}
		
		set<Object> o;
		vector object_position = m_ParentObject.GetPosition();
		vector object_transform[4];
		
		m_ParentObject.GetTransform(object_transform);
		
		//vector ground, ground_dir; int component;
		//DayZPhysics.RaycastRV(object_position, object_position + object_transform[1] * -1000, ground, ground_dir, component, o, NULL, m_ParentObject, false, true); // set to ground only
		
	
		vector cursor_position = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * vector.Distance(GetGame().GetCurrentCameraPosition(), object_position);
		
		//vector cursor_delta = cursor_position - object_position;
		//float angle = Math.Atan2(cursor_delta[0], cursor_delta[2]) * Math.RAD2DEG;	
		m_ParentObject.SetOrigin(GetPosition());
		vector direction = vector.Direction(cursor_position, object_position);
		vector angle = -direction.VectorToAngles();
		vector object_orientation = m_ParentObject.GetOrientation();
		switch (motion_type) {
			
			case "translatex": {
				object_position[0] = cursor_position[0] + object_offset[0];
				break;
			}
			
			case "translatez": {
				object_position[1] = cursor_position[1] + object_offset[1];
				break;
			}
			
			case "translatey": {
				object_position[2] = cursor_position[2] + object_offset[2];
				break;
			}
			
			
			case "rotationx": {
				
				object_orientation[2] = angle[2];
				break;
			}
			
			case "rotationy": {
				object_orientation[1] = angle[1];
				break;
			}
			case "rotationz": {
				object_orientation[0] = angle[0];
				break;
			}
		}
		
		
		
		m_ParentObject.SetOrientation(object_orientation);
		m_ParentObject.SetPosition(object_position);			
		
		
		
	}
	
	override int GetViewIndex()
	{
		
		return 1;
	}

	private bool mouse_inside = false;
	bool IsMouseInside()
	{
 		return mouse_inside;
	}
}


