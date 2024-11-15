class EditorPlaceableTooltip: ScriptView
{
	TextWidget HeaderLabel;
	ItemPreviewWidget ContentItem0, ContentItem1, ContentItem2, ContentItem3;
	ImageWidget BackupImage;
	protected Object m_MyObject;

	void ~EditorPlaceableTooltip()
	{
		if (m_MyObject) {
			m_MyObject.Delete();
		}
	}
	
	void SetTitle(string title)
	{
		HeaderLabel.SetText(title);
	}

	void SetContent(Object item)
	{
		m_MyObject = item;
		EntityAI entity = EntityAI.Cast(item);
		if (entity) {
			vector min_max[2];
			entity.GetCollisionBox(min_max);
			vector max_dir = min_max[1];
			max_dir.Normalize();
			vector angle_max = max_dir.VectorToAngles();
			Print(angle_max);
			
			ContentItem0.SetItem(entity);
			ContentItem0.SetView(0);
			ContentItem0.SetModelOrientation(-angle_max);
			
			ContentItem1.SetItem(entity);
			ContentItem1.SetView(0);
			ContentItem1.SetModelPosition(Vector(0, 15, 0));
			ContentItem1.SetModelOrientation(Vector(45, -130, 0));
			ContentItem1.Update();
			
			ContentItem2.SetItem(entity);
			ContentItem2.SetView(0);
			ContentItem2.SetModelOrientation(Vector(0, 0, 0));
			
			ContentItem3.SetItem(entity);
			ContentItem3.SetView(0);
			//ContentItem3.SetModelOrientation(Vector(-30, -30, 0));
			BackupImage.Show(false);
		}
	}
	
	void SetPosition(float x, float y)
	{
		int sx, sy;
		GetScreenSize(sx, sy);
		
		// bounds Y height to screen size
		// 150 is static height of tooltip
		if (y + 300 > sy) {
			y = sy - 300;
		}
		
		m_LayoutRoot.SetPos(x, y);
	}

	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/layouts/tooltips/EditorTooltip.layout";
	}
}