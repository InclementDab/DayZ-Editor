modded class SliderPrefab
{
    protected int m_DecimalPrecision;
    protected float m_ScrollStep;
    
    // Widget References
    protected TextWidget m_ReadoutText;
    protected EditBoxWidget m_InputEditBox;

    void SliderPrefab(string caption, Class binding_context, string binding_name, float min = 0, float max = 1, float button_increment = 0.1, float resolution = Math.EPSILON)
    {

        m_ScrollStep = button_increment;
        m_DecimalPrecision = 0;

        // Auto-Calculate Precision
        float check = button_increment;
        if (check < 1.0 && check > 0.0000001) 
        {
            while (check < 0.99 && m_DecimalPrecision < 6)
            {
                check *= 10;
                m_DecimalPrecision++;
            }
        }

        if (m_LayoutRoot)
        {
            m_ReadoutText = TextWidget.Cast(m_LayoutRoot.FindAnyWidget("EditorOptionSliderReadout"));
            m_InputEditBox = EditBoxWidget.Cast(m_LayoutRoot.FindAnyWidget("EditorOptionSliderInput"));
        }

        UpdateTextFormat();
    }

    override void PrefabPropertyChanged(string property_name)
    {
        super.PrefabPropertyChanged(property_name);
        
        if (property_name == "CalculatedValue" || property_name == "Value")
        {
            UpdateTextFormat();
        }
    }

    protected void UpdateTextFormat()
    {
        if (!m_PrefabBaseController) return;

        // Format the value safely
        string fmt = StringUtils.FloatToString(m_PrefabBaseController.CalculatedValue, m_DecimalPrecision);

        if (m_ReadoutText)
        {
            m_ReadoutText.SetText(fmt);
        }

        // Only update edit box if not focused 
        if (m_InputEditBox && GetFocus() != m_InputEditBox)
        {
            m_InputEditBox.SetText(fmt);
        }
    }

    override bool OnMouseWheel(Widget w, int x, int y, int wheel)
    {
        if (w != m_LayoutRoot && w.GetParent() != m_LayoutRoot) return false;

        float motion = wheel * m_ScrollStep;

        if (KeyState(KeyCode.KC_LCONTROL)) motion *= 10.0;
        if (KeyState(KeyCode.KC_LSHIFT)) motion *= 0.1;

        float val = m_PrefabBaseController.CalculatedValue + motion;
        val = Math.Clamp(val, m_Min, m_Max);

        m_PrefabBaseController.CalculatedValue = val;
        m_PrefabBaseController.NotifyPropertyChanged("CalculatedValue");
        
        return true;
    }
}