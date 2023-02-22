#define _HAS_STD_BYTE 0

#include <mesh_def.h>
#include <isotropic_rem.h>

int main(int argc, char *argv[]) {

	MyMesh m;

	m.openFromDisk(argv[1]);

	const int iter = std::atoi(argv[2]);
	const double deg = std::atof(argv[3]);
	const double len  = std::atof(argv[4]);
	const double surfDist = std::atof(argv[5]);

	vcg::tri::IsotropicRemeshingFast<MyMesh>::Params pars;
	pars.cleanFlag = false;
	pars.surfDistCheck = true;
	pars.smoothFlag = true;
	pars.SetFeatureAngleDeg(deg);
	pars.SetTargetLen(len);
	pars.maxSurfDist = surfDist;
	pars.iter = iter;

	vcg::tri::IsotropicRemeshingFast<MyMesh>::Do(m, pars);

	m.saveToDisk("remeshed.ply");

	return 0;
}