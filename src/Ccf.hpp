/* Class CCF 
 * Correlation function matrix for storing two dimensional cross-
 * correlation values. 
 * Inherits from Matrix2<double>
 * Includes method for locating peaks */

#ifndef CCF_H
#define CCF_H

#include "../lib/Mat2.hpp"
#include "Peak.hpp"
#include <cmath>

namespace PivEng {

	class CCF : public Mat2<double>
	{
		public:
			typedef std::shared_ptr<CCF> Sptr;

			CCF(unsigned int rows, unsigned int cols);
			~CCF();

			void findPeaks(Peak::PeaksVec& pv, int maxDisp);
			bool isLocalPeak(int j);
			int offsetX();
			int offsetY();

		private:	
			const double BIG_DOUBLE = 9999999999.9;
			bool inRange(double subject, double minimum, double maximum);
			const int m_offsetX,
			  	  	  m_offsetY;
	};

}

#endif
