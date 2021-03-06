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
#ifndef SRC_MFEMOLEVSKYLVCR_H_
#define SRC_MFEMOLEVSKYLVCR_H_

#include <ConstitutiveRelationship.h>
#include <MFEMData.h>
#include <mfem.hpp>

namespace Kelvin {

class MFEMOlevskyLVCR : public ConstitutiveRelationship {

	/**
	 * The data for the simulation. Primarily used to retrieve the MFEM mesh
	 * from the mesh container.
	 */
	MFEMData & _data;

	/**
	 * Spatial dimension
	 */
	int dim;

	/**
	 * The ratio of the volume of pores to the total volume.
	 */
	double porosity;

	/**
	 * The shear modulus of the material
	 */
	double shearModulus;

	/**
	 * (1.0-porosity)^2
	 */
	double phi;

	/**
	 * (2/3)(1-porosity)^3/porosity
	 */
	double psi;

	/**
	 * The material density
	 */
	double density;

	/**
	 * Finite element collection used to compute the velocity for the strain
	 * rate.
	 */
	mfem::H1_FECollection velCol;

	/**
	 * Finite element space used to compute the velocity for the strain rate.
	 */
	mfem::FiniteElementSpace velSpace;

	/**
	 * Finite element grid function that compute the velocity.
	 */
	mfem::GridFunction velGf;

public:

	/**
	 * Constructor
	 */
	MFEMOlevskyLVCR(MFEMData & data);

	/**
	 * Destructor
	 */
	virtual ~MFEMOlevskyLVCR();

	/**
	 * This operation updates the strain rate at the material points using
	 * infinitesimal strain theory, 1/2(\grad v + \grad v^T).
	 * @param grid the computational grid on which nodal quantities are defined.
	 * @param the material point at which the strains should be updated.
	 */
	virtual void updateStrainRate(const Kelvin::Grid & grid,
			Kelvin::MaterialPoint & matPoints);

	/**
	 * This operation updates the stress at the material points using the
	 * constitutive equation for linear viscous materials from Eugene Olevsky's
	 * Continuum Theory of Sintering.
	 * @param grid the computational grid on which nodal quantities are defined.
	 * @param the material point at which the strains should be updated.
	 */
	virtual void updateStress(const Kelvin::Grid & grid,
			Kelvin::MaterialPoint & matPoints);
};

} /* namespace Kelvin */

#endif /* SRC_MFEMOLEVSKYLVCR_H_ */
