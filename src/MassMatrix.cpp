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
#include <MassMatrix.h>
#include <utility>

using namespace mfem;
using namespace std;

namespace Kelvin {

MassMatrix::MassMatrix(const std::vector<MaterialPoint> & particleList) :
		nodes(nodesDummy),
		particles(particleList), shapes(NULL) {
	// TODO Auto-generated constructor stub

}

MassMatrix::~MassMatrix() {
	// TODO Auto-generated destructor stub
}

void MassMatrix::assemble(mfem::SparseMatrix & shapeMatrix,
		std::set<int> & nodeSet) {
	shapes = &shapeMatrix;
	nodes = nodeSet;
}

double MassMatrix::operator()(int i, int j) const {

	// Mass element m_ij
	double m_ij = 0.0;
	double particleMass = 0.0;

	// Construct the mass matrix associated with the grid nodes

	// FIXME! Set particle mass on Particle subclass of Point, next to coords.
	Vector rowI;
	Array<int> colsI;

	// Get the i-th and j-th rows of the shape matrix for the p-th
	// particle. The i-th row is technically transposed in the dot
	// product that follows.
	int numParticles = particles.size();
	for (int k = 0; k < numParticles; k++) {
		auto & matPoint = particles[k];
		particleMass = matPoint.mass;
		shapes->GetRow(k, colsI, rowI);
		int colI = colsI.Find(i);
		int colJ = colsI.Find(j);
		if (colI >= 0 && colJ >= 0) {
			m_ij += particleMass * rowI[colI] * rowI[colJ];
		}
	}

	return m_ij;
}

std::vector<double> MassMatrix::lump() {

	int i = 0; // Row counter
	set<int>::iterator outerIt;
	set<int>::iterator innerIt;
	std::vector<double> diagonal(nodes.size());

	// Loop over all the non-zero rows and compute the mass
	int numPoints = particles.size();
	double dValue = 0.0;
	double index = 0;
	for (outerIt = nodes.begin(); outerIt != nodes.end(); outerIt++) {
		const auto & constShapes = (*shapes);
		for (int i = 0; i < numPoints; i++) {
			diagonal[index] += particles[i].mass * constShapes(i, *outerIt);
		}
		index++;
	}

	return diagonal;
}

} /* namespace Kelvin */
