//std
#include <cmath>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Engine.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Joints/Type.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

#include "FEA/inc/Analysis/Analysis.hpp"
#include "FEA/inc/Analysis/Solvers/StaticNonlinear.hpp"

//Scissors
#include "Scissors/inc/scissors.hpp"

void scissors::unit(void)
{
	//data
	fea::Model model;
	typedef fea::mesh::nodes::DOF dof;
	typedef fea::mesh::joints::Type joint;
	typedef math::solvers::Convergence::Type convergence;
	//nodes
	const double q = M_PI_2;
	model.mesh()->create_node(0, 0, 0);
	model.mesh()->create_node(cos(q), sin(q), 0);
	model.mesh()->create_node(2 * cos(q), 2 * sin(q), 0);

	model.mesh()->create_node(0, 2 * sin(q), 0);
	model.mesh()->create_node(cos(q), sin(q), 0);
	model.mesh()->create_node(2 * cos(q), 0, 0);
	//joints
	model.mesh()->create_joint(joint::Rigid2D, {0, 1});
	model.mesh()->create_joint(joint::Rigid2D, {1, 2});
	model.mesh()->create_joint(joint::Rigid2D, {3, 4});
	model.mesh()->create_joint(joint::Rigid2D, {4, 5});
	model.mesh()->create_joint(joint::Revolute2D, {1, 4});
	//supports
	model.boundary()->create_support(0, dof::Translation_1);
	model.boundary()->create_support(0, dof::Translation_2);
	model.boundary()->create_support(5, dof::Translation_1);
	model.boundary()->create_support(5, dof::Translation_2);
	//actuators
	model.boundary()->support(2)->state([q] (double t) { return 2 * (1 - cos(q)) * t; });
	//solver
	model.analysis()->solver_static_nonlinear()->enable();
	model.analysis()->solver_static_nonlinear()->watch_dof().node(5);
	model.analysis()->solver_static_nonlinear()->watch_dof().dof(dof::Translation_1);
	model.analysis()->solver_static_nonlinear()->convergence().type(convergence::Fixed);
	//solve
	model.solve();
	//draw
	fea::draw::Engine(&model).start();
}