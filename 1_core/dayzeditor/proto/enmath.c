

vector AverageVectors(vector v1, vector v2)
{
	vector result;
	
	result[0] = (v1[0] + v2[0])/2;
	result[1] = (v1[1] + v2[1])/2;
	result[2] = (v1[2] + v2[2])/2;
	
	return result;
}

vector DivideVectorByNumber(vector v1, float v2)
{
	vector result;
	
	result[0] = v1[0]/2;
	result[1] = v1[1]/2;
	result[2] = v1[2]/2;
	
	return result;
}


vector DivideVector(vector v1, vector v2)
{
	vector result;
	
	result[0] = v1[0]/v2[0];
	result[1] = v1[1]/v2[1];
	result[2] = v1[2]/v2[2];
	
	return result;
}