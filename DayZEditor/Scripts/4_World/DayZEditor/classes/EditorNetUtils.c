/**
 * @class EditorNetUtils
 * @brief Provides static utility functions for efficient network data serialization.
 *
 * This class handles the quantization and bitpacking of object transform data (position, rotation, scale)
 * into a compact 116-bit payload distributed across four 32-bit integers.
 * This reduces network traffic for transform updates compared to sending raw float values.
 */
class EditorNetUtils
{
	// -- Position Quantization (68 bits total)
	static const float POS_X_PRECISION = 0.001; // For a range of [0, 16384] (16384 / 0.001 = 16,384,000 states -> 24 bits)
	static const float POS_Y_PRECISION = 0.001; // For a range of [-100, 600] (700 / 0.001 = 700,000 states -> 20 bits)
	static const float POS_Z_PRECISION = 0.001; // For a range of [0, 16384] (16384 / 0.001 = 16,384,000 states -> 24 bits)
	static const float POS_Y_OFFSET = 100.0;    // Offset to make all Y values positive for quantization

	// -- Rotation Quantization (33 bits total)
	static const float ROT_PRECISION = 0.25;  // For a range of [-180, 180] (360 / 0.25 = 1440 states -> 11 bits)
	static const float ROT_OFFSET = 180.0;    // Offset to make all rotation values positive

	// -- Scale Quantization (15 bits total)
	static const float SCALE_PRECISION = 0.05; // For a range of [0.05, 1000] (999.95 / 0.05 = 19999 states -> 15 bits)
	static const float SCALE_OFFSET = 0.05;  // Scale values start at 0.05

	/**
	 * @brief Packs position, orientation, and scale into four 32-bit integers.
	 * @param position The world position of the object.
	 * @param orientation The orientation (yaw, pitch, roll) of the object.
	 * @param scale The uniform scale of the object.
	 * @param packedData An array of 4 integers to store the packed result.
	 */
	static void PackTransform(vector position, vector orientation, float scale, out int packedData[4])
	{
		// 1. Quantize floats to integers with desired precision
		CF_Uint posX = (position[0] / POS_X_PRECISION);
		CF_Uint posY = ((position[1] + POS_Y_OFFSET) / POS_Y_PRECISION);
		CF_Uint posZ = (position[2] / POS_Z_PRECISION);
		
		CF_Uint rotYaw = ((orientation[0] + ROT_OFFSET) / ROT_PRECISION);
		CF_Uint rotPitch = ((orientation[1] + ROT_OFFSET) / ROT_PRECISION);
		CF_Uint rotRoll = ((orientation[2] + ROT_OFFSET) / ROT_PRECISION);
		
		CF_Uint scl = ((scale - SCALE_OFFSET) / SCALE_PRECISION);

		// 2. Assemble the four integers using bitwise operations based on the specified layout
		//    pack1 (32 bits): Position X (24 bits) | Position Y (lower 8 bits)
		packedData[0] = (posX & 0xFFFFFF) | ((posY & 0xFF) << 24);
		
		//    pack2 (32 bits): Position Y (upper 12 bits) | Position Z (lower 20 bits)
		packedData[1] = ((posY >> 8) & 0xFFF) | ((posZ & 0xFFFFF) << 12);
		
		//    pack3 (32 bits): Position Z (upper 4 bits) | Yaw (11 bits) | Pitch (11 bits) | Roll (lower 6 bits)
		packedData[2] = ((posZ >> 20) & 0xF) | ((rotYaw & 0x7FF) << 4) | ((rotPitch & 0x7FF) << 15) | ((rotRoll & 0x3F) << 26);
		
		//    pack4 (32 bits): Roll (upper 5 bits) | Scale (15 bits)
		packedData[3] = ((rotRoll >> 6) & 0x1F) | ((scl & 0x7FFF) << 5);
	}

	/**
	 * @brief Unpacks four 32-bit integers back into position, orientation, and scale.
	 * @param packedData An array of 4 packed integers.
	 * @param position The resulting world position.
	 * @param orientation The resulting orientation.
	 * @param scale The resulting uniform scale.
	 */
	static void UnpackTransform(int packedData[4], out vector position, out vector orientation, out float scale)
	{
		CF_Uint p1 = packedData[0];
		CF_Uint p2 = packedData[1];
		CF_Uint p3 = packedData[2];
		CF_Uint p4 = packedData[3];

		// 1. Extract quantized integer values from the packed data using masks and shifts
		CF_Uint posX = p1 & 0xFFFFFF;
		CF_Uint posY = ((p1.ShiftRight(24) & 0xFF)) | ((p2 & 0xFFF) << 8);
		CF_Uint posZ = ((p2.ShiftRight(12) & 0xFFFFF)) | ((p3 & 0xF) << 20);

		CF_Uint rotYaw = (p3.ShiftRight(4)) & 0x7FF;
		CF_Uint rotPitch = (p3.ShiftRight(15)) & 0x7FF;
		CF_Uint rotRoll = ((p3.ShiftRight(26) & 0x3F)) | ((p4 & 0x1F) << 6);
		
		CF_Uint scl = (p4.ShiftRight(5)) & 0x7FFF;

		// 2. Dequantize integers back to floats
		position[0] = posX * POS_X_PRECISION;
		position[1] = (posY * POS_Y_PRECISION) - POS_Y_OFFSET;
		position[2] = posZ * POS_Z_PRECISION;
		
		orientation[0] = (rotYaw * ROT_PRECISION) - ROT_OFFSET;
		orientation[1] = (rotPitch * ROT_PRECISION) - ROT_OFFSET;
		orientation[2] = (rotRoll * ROT_PRECISION) - ROT_OFFSET;
		
		scale = (scl * SCALE_PRECISION) + SCALE_OFFSET;
	}
};