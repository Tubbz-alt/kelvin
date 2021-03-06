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
#include <MaterialPoint.h>

namespace Kelvin {

MaterialPoint::MaterialPoint(int dim) : Point(dim), stress(dim), strain(dim),
		bodyForce(dim), mass(0.0), materialId(0) {
	// Resize the stress and strain tensor rows to constrain their memory
	// footprint.
	for (int i = 0; i < dim; i++) {
		auto & stressRow = stress[i];
		stressRow.resize(dim);
		auto & strainRow = strain[i];
		strainRow.resize(dim);
	}
}

MaterialPoint::MaterialPoint(const MaterialPoint & otherPoint) :
		MaterialPoint(otherPoint.dimension()) {

	for (int i = 0; i < nDim; i++) {
		pos[i] = otherPoint.pos[i];
		vel[i] = otherPoint.vel[i];
		acc[i] = otherPoint.acc[i];
		for (int j = 0; j < nDim; j++) {
			stress[i][j] = otherPoint.stress[i][j];
			strain[i][j] = otherPoint.strain[i][j];
		}
		bodyForce[i] = otherPoint.bodyForce[i];
	}
	mass = otherPoint.mass;
	materialId = otherPoint.materialId;

	// Do I actually need the above loop? Wouldn't copy assignment work?
	// Something for later.

	return;
}

MaterialPoint::MaterialPoint(const Point & otherPoint) :
		MaterialPoint(otherPoint.dimension()) {

	for (int i = 0; i < nDim; i++) {
		pos[i] = otherPoint.pos[i];
		vel[i] = otherPoint.vel[i];
		acc[i] = otherPoint.acc[i];
	}

	// All other attributes are 0.0.

	return;
}

} /* namespace Kelvin */
