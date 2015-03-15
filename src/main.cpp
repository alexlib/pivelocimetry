// #include <iostream>
#include <memory>

#include "ocv_image.h"
#include "int_map.h"
#include "config_file.h"
#include "pivoptions.h"
#include "xcorr2.h"
#include "ccf.h"
#include "grid.h"
#include "piv_point.h"

int main(int argc, char** argv)
{
	// CLI Args
	// std::cout << "argc: " << argc << std::endl;

	// Load a raw images and extract pixel intensity maps
	std::unique_ptr<IntMappable> rawIm1(new OCVImage("../img/1.bmp")),
								 rawIm2(new OCVImage("../img/2.bmp"));

	// Extract pixel intensity maps
	std::unique_ptr<IntMap> i1(new IntMap(rawIm1)),
		                    i2(new IntMap(rawIm2));

	/* Load options from config file
	 * Enable select config file from command line, failing that, use
	 * a default config file */
	std::unique_ptr<PivOptions> analysisOptions(new PivOptions(
				ConfigFile::parse("../config/default.cfg")
			));	

	// Create a grid
	std::unique_ptr<Grid> grid( new Grid(analysisOptions, i1));
	// std::cout << grid->get_xCoord(4) << std::endl;

	/* Create a PIV point at a grid point */
	// PIVPoint p = PIVPoint(255, 255, analysisOptions);
	// std::cout << *p.get_ccf() << std::endl;	
	
	/* Create a vector of PIVPoints, give constructor to instantiate CCF at correct size, 
	* for now set coordinate to (-1, -1) to indicate that the piv has not yet been done */
	std::vector<PIVPoint> vp(grid->get_totalGridPoints(), PIVPoint(-1, -1, analysisOptions));

	/* Loop through each grid point:
	 * 1) Set the correct coordinates
	 * 2) Do the PIV? */
	int iNow, jNow;
	for (int j = 0; j < grid->get_noPointsY(); j++) {
		for (int i = 0; i < grid->get_noPointsX(); i++) {
			iNow = grid->get_xCoord(i);
			jNow = grid->get_yCoord(j);

			vp[i*j].set_xCoord(iNow);			
			vp[i*j].set_yCoord(jNow);			

			XCorr2::xCorr2n(vp[i*j].get_ccf(), i1, i2, iNow, jNow,
						analysisOptions->get_windowWidth(), analysisOptions->get_windowHeight()
					);
		}
	}

	std::cout << "Length of points vector: " << vp.size() << std::endl;

	// // Initialize a CCF to dump the correlation data in
	// XCorr2::xCorr2n(vp[0].get_ccf(), i1, i2, 255, 255,
	// 			analysisOptions->get_windowWidth(), analysisOptions->get_windowHeight()
	// 		);

	// std::cout << "vp[0] ccf at (0,0): " << vp[0].get_ccf()->get_elementAt(16,16) << std::endl; 
	// std::cout << *p.get_ccf() << std::endl;

	/* ToDo:
	 * 1) Calculate grid
	 * 2) Initialize storage for PIV info
	 * 3) Do cross-correlation
	 * 4) Find peaks in correlation function
	 * 5) Calculate sub-pixel displacements for peaks
	 * 6) Export results */

	return 0;
}
