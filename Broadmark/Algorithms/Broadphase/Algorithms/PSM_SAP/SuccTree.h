#ifndef _PSM_SAP_SUCCTREE_
#define _PSM_SAP_SUCCTREE_
#include <boost\dynamic_bitset.hpp>
#include <math.h>  
namespace PSM {
	class SuccTree
	{
	public:
		SuccTree(int numberOfObjects) {
			t_lenght = (int)std::ceil(log(numberOfObjects) / std::log(k));
			*t = new boost::dynamic_bitset<>[t_lenght];
			// each d-th level, Td, is ceil(n/(pow(k   de bits long
			for (int d = 0; d < t_lenght; ++d) 
				(*t)[d] = boost::dynamic_bitset<>((int)std::ceil(log(numberOfObjects) / std::pow(d, k)), 0);

		}
		~SuccTree() {};
		void ins(int i)
		{
			for (int d = 0; d < t_lenght; ++d)
			{
				(*t)[d][i] = 1;
				if ((*t)[d + 1][(int)std::floor(i*invers_K)] != 0){ return; }
				i = (int)std::floor(i*invers_K);
			}
		}
		void del(int i)
		{
			for (int d = 0; d < t_lenght; ++d)
			{
				(*t)[d][i] = 0;
				if ((*t)[d].any()) { return; }
				i = (int)std::floor(i*invers_K);
			}
		}
		int succ(int i) 
		{
			int d = 0;
			boost::dynamic_bitset<> X = GreaterSiblings(boost::dynamic_bitset<>((*t)[0]), i);
		}
	private:
		///maskar ut all värden som är lägre än bitIndex
		boost::dynamic_bitset<> GreaterSiblings(boost::dynamic_bitset<> d, int bitIndex) 
		{
			// om d = 00100110 och bit index = 2 
			// kommder d_result först få värdet 00110000
			//sedan returnerar det 00000110
			boost::dynamic_bitset<> d_result( (boost::dynamic_bitset<>((*t)[0] << (bitIndex +1)) ));
			return (d_result >> (bitIndex + 1));
		}
		boost::dynamic_bitset<>* t[1]; // levels 
		             int t_lenght;
		static const int k     = 64; //logBase
		       const double invers_K = 1.0 / k;
	};
}
#endif // !_PSM_SAP_SUCCTREE_