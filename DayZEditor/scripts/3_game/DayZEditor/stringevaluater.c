

typedef string StringEvaluater;
class StringEvaluater: string
{
	static int pos;
	static int ch;
	
	float Parse()
	{
		pos = -1;
		ch = 0;
		NextChar();
		float x = ParseExpression();
		if (pos < value.Length()) {
			Error("Unexpected: " + ch);
		}
		return x;
	}
	
	private void NextChar() 
	{
		pos++;
		if (pos < value.Length()) {
			ch = value.Get(pos).Hash();
		} else {
			ch = -1;
		}
	}
	
	private bool Eat(int charToEat) 
	{
	    while (ch == 32) NextChar();
	    if (ch == charToEat) {
	        NextChar();
	        return true;
	    }
	    return false;
	}
	
	private float ParseExpression() 
	{
	    float x = ParseTerm();
	    while (!false) {
	        if      (Eat("+".Hash())) x += ParseTerm(); // addition
	        else if (Eat("-".Hash())) x -= ParseTerm(); // subtraction
	        else return x;
	    }
		
		return x;
	}
	
	private float ParseTerm() 
	{
	    float x = ParseFactor();
	    while (true) {
	        if      (Eat("*".Hash())) x *= ParseFactor(); // multiplication
	        else if (Eat("/".Hash())) x /= ParseFactor(); // division
	        else return x;
	    }
		
		return x;
	}
	
	private float ParseFactor() 
	{
	    if (Eat("+".Hash())) return ParseFactor(); // unary plus
	    if (Eat("-".Hash())) return -ParseFactor(); // unary minus
	
	    float x;
	    int startPos = pos;
	    if (Eat("(".Hash())) { // parentheses
	        x = ParseExpression();
	        Eat(")".Hash());
	    } else if ((ch >= "0".Hash() && ch <= "9".Hash()) || ch == ".".Hash()) { // numbers
	        while ((ch >= "0".Hash() && ch <= "9".Hash()) || ch == ".".Hash()) 
				NextChar();

	        x = (value.Substring(startPos, pos - startPos)).ToFloat();
	    } else if (ch >= "a".Hash() && ch <= "z".Hash()) { // functions
	        while (ch >= "a".Hash() && ch <= "z".Hash()) NextChar();
	        string fnc = value.Substring(startPos, pos - startPos);
	        x = ParseFactor();
	        if (fnc == "sqrt") x = Math.Sqrt(x);
	        else if (fnc == "sin") x = Math.Sin(x * Math.DEG2RAD);
	        else if (fnc == "cos") x = Math.Cos(x * Math.DEG2RAD);
	        else if (fnc == "tan") x = Math.Tan(x * Math.DEG2RAD);
	        else Error("Unknown function: " + fnc);
	    } else {
	        //Error("Unexpected: " + ch);
	    }
	
	    if (Eat("^".Hash())) x = Math.Pow(x, ParseFactor()); // exponentiation
	
	    return x;
	}
}