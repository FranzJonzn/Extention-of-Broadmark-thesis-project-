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
			t = new boost::dynamic_bitset<>[t_lenght];
			// each d-th level, Td, is ceil(n/(pow(k   de bits long
			for (int d = 0; d < t_lenght; ++d) 
				(*t)[d] = new boost::dynamic_bitset<>((int)std::ceil(log(numberOfObjects) / std::pow(d, k)), 0);


		}
		~SuccTree() {};

		void ins(int i)
		{
			for (int d = 0; d < t_lenght; ++d)
			{
				(*t)[d][i] = 1
			}

		}
		void del(int i);

	private:

		boost::dynamic_bitset<>* t; // levels 

	
		int t_lenght;

		static const int k = 64; //logBase
	};
}
#endif // !_PSM_SAP_SUCCTREE_