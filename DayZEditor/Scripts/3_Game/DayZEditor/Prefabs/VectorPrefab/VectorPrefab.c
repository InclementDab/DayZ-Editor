modded class VectorPrefab
{
	// Override is for step size
    protected int m_DecimalPrecision = 3;
    
    void VectorPrefab(string caption, Class binding_context, string binding_name, int decimal_precision = 3)
    {
        m_DecimalPrecision = decimal_precision;
        
        m_VectorPrefabController.X = StringUtils.FloatToString(m_PrefabBaseController.Value[0], m_DecimalPrecision);
        m_VectorPrefabController.Y = StringUtils.FloatToString(m_PrefabBaseController.Value[1], m_DecimalPrecision);
        m_VectorPrefabController.Z = StringUtils.FloatToString(m_PrefabBaseController.Value[2], m_DecimalPrecision);
        m_VectorPrefabController.NotifyPropertyChanged("X", false);
        m_VectorPrefabController.NotifyPropertyChanged("Y", false);
        m_VectorPrefabController.NotifyPropertyChanged("Z", false);
    }
    
	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		float motion = wheel * 0.1; // precision value
		
		if (KeyState(KeyCode.KC_LCONTROL)) {
			motion *= 10;
		}
		
		if (KeyState(KeyCode.KC_LSHIFT)) {
			motion *= 0.1;
		}
				
		switch (w.GetName()) {
			
			case "x_value": {
                m_VectorPrefabController.X = StringUtils.FloatToString(m_VectorPrefabController.X.Parse() + motion, m_DecimalPrecision);
				m_VectorPrefabController.NotifyPropertyChanged("X");
				break;
			}
			
			case "y_value": {
                m_VectorPrefabController.Y = StringUtils.FloatToString(m_VectorPrefabController.Y.Parse() + motion, m_DecimalPrecision);
				m_VectorPrefabController.NotifyPropertyChanged("Y");
				break;
			}
			
			case "z_value": {
                m_VectorPrefabController.Z = StringUtils.FloatToString(m_VectorPrefabController.Z.Parse() + motion, m_DecimalPrecision);
				m_VectorPrefabController.NotifyPropertyChanged("Z");
				break;
			}
		}
		
		return true;
	}
}