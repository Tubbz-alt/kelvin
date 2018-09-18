/**----------------------------------------------------------------------------
 Copyright  2018-, UT-Battelle, LLC
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 * Neither the name of fern nor the names of its
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
#ifndef SRC_GRID_H_
#define SRC_GRID_H_

#include <Point.h>
#include <mfem.hpp>
#include <set>
#include <MassMatrix.h>
#include <MeshContainer.h>
#include <KelvinBaseTypes.h>
#include <functional>
#include <map>

namespace Kelvin {

/**
* This is the background Eulerian grid. It computes and stores the raw mesh
* (through the meshContainer), velocity, acceleration, mass, stress,
* strain, gradients of these quantities, etc.
*
* Nodes are stored as Points, which have position, velocity, and
* acceleration. Accessing these quantities should be done by pulling and
* indexing the list of nodes through the nodes() operation.
* @code
* auto & nodes = grid.nodes();
* auto & pos5 = nodes[4].pos;
* auto & vel5 = nodes[4].vel;
* auto & acc5 = nodes[4].acc;
* ...
* @endcode
*/
class Grid {

protected:

	/**
	 * Nodal positions
	 */
	std::vector<Point> _nodes;

	/**
	 * The list of massive nodes
	 */
	std::set<int> nodeSet;

	/**
	 * The mesh container that holds the original finite element mesh.
	 */
	MeshContainer & _meshContainer;

	/**
	 * The shape/mapping matrix
	 */
	std::unique_ptr<mfem::SparseMatrix> _shapeMatrix;

	/**
	 * A map representing a sparse matrix holding the shape function gradients.
	 */
	std::map<int,std::vector<Gradient>> _gradientMap;

	/**
	 * The mass matrix that shows the amount of mass shared between nodes due
	 * to the particles.
	 */
	std::unique_ptr<MassMatrix> _massMatrix;

	/**
	 * This private operation updates the state of the shape matrix.
	 */
	void updateShapeMatrix();

	/**
	 * This private operation updates the state of the mass matrix.
	 */
	void updateMassMatrix();

	/**
	 * The list of forces applied on the grid. The function prototype for
	 * forces is f(Point,double) where the first argument is the coordinates
	 * where the force is currently applied and the second argument is the
	 * mass.
	 */
    std::vector<std::function<void(const Point&,double)>> forces;

public:

	/**
	 * Constructor. Requires a mesh container as input to extract nodal and
	 * gradient information.
	 * @param meshContainer the finite element mesh used to create the grid
	 */
	Grid(MeshContainer & meshContainer);

	/**
	 * Destructor
	 */
	virtual ~Grid();

	/**
	 * Assemble all matrices. Rock and roll.
	 * @param particles the particle list that represent a mass distributed
	 * across the grid. Used to create shape and mass matrices.
	 */
	void assemble(const std::vector<Kelvin::Point> & particles);

	/// Update kinematics, fields, etc.
	void update();

	/**
	 * Get the mass matrix associated with the grid.
	 * @return the mass matrix
	 */
	const MassMatrix & massMatrix() const;

	/**
	 * This operation returns the gradients of the nodal shape functions for
	 * the nodes that are near particles.
	 * @return a map representing the matrix of gradients (technically a rank
	 * 3 tensor). The map key represents the row id which is equal to the
	 * particle id in the particles list provided to assemble(), and the value
	 * set contains the Gradients and represents the columns assigned to that
	 * row.
	 */
	const std::map<int,std::vector<Gradient>> & gradients() const;

	/**
	 * This operation computes and returns the internal forces at the grid
	 * nodes.
	 * @return the internal forces
	 */
	const std::vector<ForceVector> & internalForces();

	/**
	 * This operation returns the present kinematic information at the nodes
	 * as Points with positions, velocities, and accelerations. This includes
	 * the whole grid, not just the massive nodes.
	 * @return the kinematic information of the nodes
	 */
	const std::vector<Point> nodes() const;

	/**
	 * This function adds a force to the list of forces at are applied at the
	 * grid points.
	 * @param force the function to add to the list of applied forces. It must
	 * take a point and a mass as input arguments.
	 */
	void addForce(std::function<void(const Point &,double)> force);

};

} /* namespace Kelvin */

#endif /* SRC_GRID_H_ */
