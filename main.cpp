#include <mesh_def.h>
#include <isotropic_rem.h>

int main(int argc, char *argv[]) {

	MyMesh m;

	m.openFromDisk(argv[1]);

	const int iter = std::atoi(argv[2]);
	const float deg = std::atof(argv[3]);
	const float len  = std::atof(argv[4]);
	const float surf_dist = std::atof(argv[5]);

	vcg::tri::IsotropicRemeshingFast<MyMesh>::Params pars;
	pars.cleanFlag = false;
	pars.surfDistCheck = true;
	pars.SetFeatureAngleDeg(deg);
	pars.SetTargetLen(len);
	pars.maxSurfDist = surf_dist;
	pars.iter = iter;

	vcg::tri::IsotropicRemeshingFast<MyMesh>::Do(m, pars);
	std::cout << "done" << std::endl;
	m.saveToDisk("remeshed.ply");
	return 0;
}