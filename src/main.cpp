//std
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>

//FEA
#include "FEA/inc/Model.hpp"

#include "FEA/inc/Draw/Engine.hpp"

#include "FEA/inc/Mesh/Mesh.hpp"
#include "FEA/inc/Mesh/Nodes/DOF.hpp"
#include "FEA/inc/Mesh/Joints/Type.hpp"

#include "FEA/inc/Boundary/Boundary.hpp"
#include "FEA/inc/Boundary/Supports/Support.hpp"

//Scissors
#include "Scissors/inc/scissors.hpp"

void translational(const double* x, uint32_t n, double a, double b, double h)
{
	//data
	fea::Model model;
	model.mesh()->create_node(x[0], x[1], 0);
	model.mesh()->create_node(x[0], x[1], 0);
	model.mesh()->create_joint(fea::mesh::joints::Type::Revolute2D, {0, 1});
	//scissors
	for(uint32_t i = 0; i < n; i++)
	{
		//data
		const double dx = x[2 * i + 2] - x[2 * i + 0];
		const double dy = x[2 * i + 3] - x[2 * i + 1];
		const double qh = acos((a * a + b * b - h * h) / (2 * a * b));
		const double qa = asin(a / h * sin(qh));
		const double qb = asin(b / h * sin(qh));
		const double s1 = a * sin(qb);
		const double h1 = a * cos(qb);
		const double h2 = b * cos(qa);
		const double a_new = sqrt(pow(dx - s1, 2) + pow(dy + h2, 2));
		const double b_new = sqrt(pow(dx - s1, 2) + pow(dy - h1, 2));
		//create nodes
		model.mesh()->create_node(x[2 * i + 0] + s1, x[2 * i + 1] - h2, 0);
		model.mesh()->create_node(x[2 * i + 0] + s1, x[2 * i + 1] - h2, 0);
		model.mesh()->create_node(x[2 * i + 0] + s1, x[2 * i + 1] + h1, 0);
		model.mesh()->create_node(x[2 * i + 0] + s1, x[2 * i + 1] + h1, 0);
		model.mesh()->create_node(x[2 * i + 0] + dx, x[2 * i + 1] + dy, 0);
		model.mesh()->create_node(x[2 * i + 0] + dx, x[2 * i + 1] + dy, 0);
		//create joints
		model.mesh()->create_joint(fea::mesh::joints::Type::Rigid2D, {6 * i + 0, 6 * i + 2});
		model.mesh()->create_joint(fea::mesh::joints::Type::Rigid2D, {6 * i + 1, 6 * i + 4});
		model.mesh()->create_joint(fea::mesh::joints::Type::Rigid2D, {6 * i + 3, 6 * i + 6});
		model.mesh()->create_joint(fea::mesh::joints::Type::Rigid2D, {6 * i + 5, 6 * i + 7});
		model.mesh()->create_joint(fea::mesh::joints::Type::Revolute2D, {6 * i + 2, 6 * i + 3});
		model.mesh()->create_joint(fea::mesh::joints::Type::Revolute2D, {6 * i + 4, 6 * i + 5});
		model.mesh()->create_joint(fea::mesh::joints::Type::Revolute2D, {6 * i + 6, 6 * i + 7});
		//update
		a = a_new;
		b = b_new;
	}
	//draw
	fea::draw::Engine(&model).start();
}

int main(void)
{
	try
	{
		const uint32_t n = 20;
		double* x = (double*) alloca(2 * (n + 1) * sizeof(double));
		for(uint32_t i = 0; i < n + 1; i++)
		{
			x[2 * i + 1] = 0;
			x[2 * i + 0] = double(i) / n;
		}
		translational(x, n, 0.1, 0.1, 0.05);
		// scissors::unit();
	}
	catch(const std::exception& exception)
	{
		printf("%s\n", exception.what());
	}
	//return
	return EXIT_SUCCESS;
}