// Maybe create an EditorPoint abstraction and use that as the BottomCenter, Center, Top center, and 
// all other bounding points for the bboxes. Could be useful? PauseChamp
typedef vector EditorPoint;
class EditorPoint: vector
{

}


class EditorSnapPoint: EditorWorldObject
{
	protected float m_Range;
	protected vector m_SnapPosition;
	protected EditorObject m_EditorObject;
	
	void EditorSnapPoint(EditorObject editor_object, vector position, float range = 5)
	{
		m_EditorObject = editor_object; m_SnapPosition = position; m_Range = range;
		
		m_WorldObject = GetGame().CreateObjectEx("BoundingBoxBase", vector.Zero, ECE_NONE);
		m_WorldObject.SetScale(0.001);
		m_EditorObject.GetWorldObject().AddChild(m_WorldObject, 0);
		m_WorldObject.SetPosition(m_SnapPosition);
		m_WorldObject.Update();
	}
	
	void SetRange(float range) {
		m_Range = range;
	}
	
	bool InRange(EditorSnapPoint point)	{
		return (vector.Distance(m_SnapPosition, point.GetPosition()) < m_Range);
	}
	
	vector GetPosition() {
		return m_SnapPosition;
	}
	
}

