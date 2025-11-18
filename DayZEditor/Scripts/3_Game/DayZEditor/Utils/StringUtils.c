class StringUtils
{
    static string FloatToString(float value, int decimal_places)
    {
        // 1. Standard Rounding for integers
        if (decimal_places <= 0)
            return Math.Round(value).ToString();

        // 2. Handle Sign manually
        string sign = "";
        if (value < 0)
        {
            sign = "-";
            value = Math.AbsFloat(value);
        }

        // 3. Split Integer and Fraction
        float intPart = Math.Floor(value);
        float fracPart = value - intPart;

        // 4. Round Fraction
        float scale = Math.Pow(10, decimal_places);
        float roundedFrac = Math.Round(fracPart * scale);

        // 5. Handle Rollover
        if (roundedFrac >= scale)
        {
            intPart += 1;
            roundedFrac = 0;
        }

        // 6. Stringify Integer
        string sInt = intPart.ToString();
        int dotIndex = sInt.IndexOf(".");
        if (dotIndex != -1) sInt = sInt.Substring(0, dotIndex);

        // 7. Stringify Fraction
        string sFrac = roundedFrac.ToString();
        dotIndex = sFrac.IndexOf(".");
        if (dotIndex != -1) sFrac = sFrac.Substring(0, dotIndex);

        // 8. Pad Zeros
        while (sFrac.Length() < decimal_places)
        {
            sFrac = "0" + sFrac;
        }

        return sign + sInt + "." + sFrac;
    }
}