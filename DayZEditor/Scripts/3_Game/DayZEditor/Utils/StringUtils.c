class StringUtils
{
    static string FloatToString(float value, int decimal_places)
    {
        string str = value.ToString(false);
        int dot_pos = str.IndexOf(".");
        
        if (dot_pos == -1) {
            if (decimal_places > 0) {
                str += ".";
                for (int i = 0; i < decimal_places; i++) {
                    str += "0";
                }
            }
            return str;
        }
        
        int desired_length = dot_pos + decimal_places + 1;
        
        if (str.Length() <= desired_length) {
            while (str.Length() < desired_length) {
                str += "0";
            }
            return str;
        }
        
        return str.Substring(0, desired_length);
    }
}
