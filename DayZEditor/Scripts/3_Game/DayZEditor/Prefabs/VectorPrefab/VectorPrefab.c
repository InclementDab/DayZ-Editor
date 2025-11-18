modded class VectorPrefab
{
	// Override is for step size
    protected int m_DecimalPrecision = 3;
    protected EditBoxWidget m_EditX, m_EditY, m_EditZ;
    
    void VectorPrefab(string caption, Class binding_context, string binding_name, int decimal_precision = 3)
    {
        m_DecimalPrecision = decimal_precision;
        
        if (m_LayoutRoot)
        {
            m_EditX = EditBoxWidget.Cast(m_LayoutRoot.FindAnyWidget("x_value"));
            m_EditY = EditBoxWidget.Cast(m_LayoutRoot.FindAnyWidget("y_value"));
            m_EditZ = EditBoxWidget.Cast(m_LayoutRoot.FindAnyWidget("z_value"));
        }

        UpdateWidgets();
    }
    
    override void PrefabPropertyChanged(string property_name)
    {
        super.PrefabPropertyChanged(property_name);

        if (property_name == "Value")
        {
            UpdateWidgets();
        }
    }

    protected void UpdateWidgets()
    {
        if (m_VectorPrefabController && m_PrefabBaseController)
        {
            Widget focused = GetFocus();

            // 1. Update X if not focused
            if (m_EditX != focused)
            {
                m_VectorPrefabController.X = StringUtils.FloatToString(m_PrefabBaseController.Value[0], m_DecimalPrecision);
                m_VectorPrefabController.NotifyPropertyChanged("X", false);
            }

            // 2. Update Y if not focused
            if (m_EditY != focused)
            {
                m_VectorPrefabController.Y = StringUtils.FloatToString(m_PrefabBaseController.Value[1], m_DecimalPrecision);
                m_VectorPrefabController.NotifyPropertyChanged("Y", false);
            }

            // 3. Update Z if not focused
            if (m_EditZ != focused)
            {
                m_VectorPrefabController.Z = StringUtils.FloatToString(m_PrefabBaseController.Value[2], m_DecimalPrecision);
                m_VectorPrefabController.NotifyPropertyChanged("Z", false);
            }
        }
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
                m_VectorPrefabController.X = StringUtils.FloatToString(m_PrefabBaseController.Value[0] + motion, m_DecimalPrecision);
				m_VectorPrefabController.NotifyPropertyChanged("X");
				break;
			}
			
			case "y_value": {
                m_VectorPrefabController.Y = StringUtils.FloatToString(m_PrefabBaseController.Value[1] + motion, m_DecimalPrecision);
				m_VectorPrefabController.NotifyPropertyChanged("Y");
				break;
			}
			
			case "z_value": {
                m_VectorPrefabController.Z = StringUtils.FloatToString(m_PrefabBaseController.Value[2] + motion, m_DecimalPrecision);
				m_VectorPrefabController.NotifyPropertyChanged("Z");
				break;
			}
		}
		
		return true;
	}
}