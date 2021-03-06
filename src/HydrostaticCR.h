/**----------------------------------------------------------------------------
 Copyright (c) 2019-, UT-Battelle, LLC
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
#ifndef SRC_HYDROSTATICCR_H_
#define SRC_HYDROSTATICCR_H_

#include <ConstitutiveRelationship.h>

namespace Kelvin {

class HydrostaticCR: public ConstitutiveRelationship {
public:
	HydrostaticCR();
	virtual ~HydrostaticCR();

	/**
	 * This function computes the strain rate for a hydrostatic configuration.
	 * @param grid the computational grid on which nodal quantities are defined.
	 * @param the material point at which the strains should be updated.
	 */
	void updateStrainRate(const Kelvin::Grid & grid,
			Kelvin::MaterialPoint & matPoints);

	/**
	 * This operation computes the hydrostatic stress.
	 * @param grid the computational grid on which nodal quantities are defined.
	 * @param the material point at which the strains should be updated.
	 */
	void updateStress(const Kelvin::Grid & grid,
			Kelvin::MaterialPoint & matPoints);
};

} /* namespace Kelvin */

#endif /* SRC_HYDROSTATICCR_H_ */
