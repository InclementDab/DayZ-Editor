using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DayZ_Bin_Editor.Enfusion
{
    public struct vector
    {
        public float X, Y, Z;

        public vector(float x = 0, float y = 0, float z = 0)
        {
            X = x; Y = y; Z = z;
        }

        public float this[int index] {
            get {
                switch (index) {
                    case 0: return X;
                    case 1: return Y;
                    case 2: return Z;
                    default: throw new IndexOutOfRangeException();
                }
            }

            set {
                switch (index) {
                    case 0: X = value; break;
                    case 1: Y = value; break;
                    case 2: Z = value; break;
                    default: throw new IndexOutOfRangeException();
                }
            }
        }

        public static readonly vector Zero = new vector();

        public override string ToString()
        {
            return $"{X}, {Y}, {Z}";
        }
    }
}
