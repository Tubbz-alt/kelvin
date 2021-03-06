/**----------------------------------------------------------------------------
 Copyright (c) 2018-, UT-Battelle, LLC
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 * Neither the name of the copyright holder nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 Author(s): Jay Jay Billings (billingsjj <at> ornl <dot> gov)
 -----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <MFEMManager.h>
#include <MFEMThermalSolver.h>
#include <MFEMMPMSolver.h>
#include <MFEMMPMData.h>
#include <memory>
#include <MFEMOlevskyLVCR.h>
#include <HydrostaticCR.h>
#include <ConstitutiveRelationshipService.h>

using namespace std;
using namespace Kelvin;

/**
 * This function registers user-defined constitutive relationships with the framework.
 * @param data the MPM data structure provided the by MFEMManager
 */
void configureConstitutiveRelationships(MFEMMPMData & data) {
	// Create the Olevsky sintering constitutive relationships
	unique_ptr<ConstitutiveRelationship> olevskyLVCR =
			make_unique<MFEMOlevskyLVCR>(data);
	int olveskyID = 1;
	// Create the hydrostatic constitutive relationships
	unique_ptr<ConstitutiveRelationship> hydrostaticCR =
			make_unique<HydrostaticCR>();
	int hsID = 2;

	// Add them to the service
	ConstitutiveRelationshipService::add(olveskyID,std::move(olevskyLVCR));
	ConstitutiveRelationshipService::add(hsID,std::move(hydrostaticCR));

	return;
}

/**
 * Main program
 * @param argc the number of input arguments
 * @param argv the input arguments array of argc elements
 * @return EXIT_SUCCESS if successful, otherwise another value.
 */
int main(int argc, char * argv[]) {

	// Input file name - default is input.ini in the present directory.
	string inputFile("input.ini");

	// Create the MFEM problem manager
//	MFEMManager<MFEMThermalSolver,MFEMMPMData> manager;
	MFEMManager<MFEMMPMSolver,MFEMMPMData> manager;
	manager.setup(inputFile,argc,argv);

	// Configure the full set of constitutive relationships
	configureConstitutiveRelationships(manager.data);

	// Do the thermal solve
	manager.solve();

	return EXIT_SUCCESS;
}
