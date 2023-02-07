#pragma once

#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/update/flag.h>
#include <vcg/complex/algorithms/update/topology.h>
#include <vcg/complex/algorithms/update/normal.h>
#include <vcg/complex/algorithms/update/quality.h>
#include <vcg/complex/algorithms/update/color.h>
#include <vcg/complex/algorithms/update/curvature.h>
#include <vcg/complex/algorithms/clean.h>
#include <vcg/complex/algorithms/create/platonic.h>

#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export.h>


class MyVertex; class MyFace;

struct MyUsedTypes :
        public vcg::UsedTypes<vcg::Use<MyVertex>::AsVertexType, vcg::Use<MyFace>::AsFaceType>{};

class MyVertex : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3d, vcg::vertex::Normal3d, vcg::vertex::Color4b,
        vcg::vertex::VFAdj, vcg::vertex::Mark, vcg::vertex::BitFlags, vcg::vertex::Qualityd, vcg::vertex::CurvatureDird> {};

class MyFace : public vcg::Face<MyUsedTypes, vcg::face::FFAdj, vcg::face::VFAdj, vcg::face::Qualityd, vcg::face::Color4b,
        vcg::face::Normal3d,  vcg::face::VertexRef, vcg::face::Mark, vcg::face::BitFlags, vcg::face::CurvatureDird > {};

class MyMesh : public vcg::tri::TriMesh< std::vector<MyVertex>, std::vector<MyFace>> {
public:
	typedef MyMesh                    ThisType;
	typedef std::shared_ptr<ThisType> Ptr;

	vcg::Matrix44<double> tr = vcg::Matrix44<double>::Identity();
	std::string meshName;

	void updateBoxAndNormals()
	{
		(*this).updateBox();
		if(fn>0) {
			vcg::tri::UpdateNormal<MyMesh>::PerFaceNormalized(*this);
			vcg::tri::UpdateNormal<MyMesh>::PerVertexAngleWeighted(*this);
		}
	}

	void updateBox()
	{
		vcg::tri::Allocator<MyMesh>::CompactEveryVector(*this);
		vcg::tri::UpdateBounding<MyMesh>::Box(*this);
	}

	void centerAndScaleToUnitBBox()
	{
		vcg::tri::UpdateBounding<MyMesh>::Box(*this);
		auto maxDim = std::max((*this).bbox.DimX(), std::max((*this).bbox.DimY(), (*this).bbox.DimZ()));
		vcg::tri::UpdatePosition<MyMesh>::Scale(*this, 1.f/maxDim);
		vcg::tri::UpdateBounding<MyMesh>::Box(*this);
		vcg::tri::UpdatePosition<MyMesh>::Translate(*this, -(*this).bbox.Center());
		vcg::tri::UpdateBounding<MyMesh>::Box(*this);
	}

	void deleteSelected()
	{
		vcg::tri::UpdateSelection<MyMesh>::VertexClear(*this);
		vcg::tri::UpdateSelection<MyMesh>::VertexFromFaceStrict(*this);

		for(auto fi = (*this).face.begin(); fi != (*this).face.end(); ++fi)
			if(fi->IsS())
				vcg::tri::Allocator<MyMesh>::DeleteFace(*this, *fi);

		for(auto vi = (*this).vert.begin(); vi != (*this).vert.end(); ++vi)
			if(vi->IsS())
				vcg::tri::Allocator<MyMesh>::DeleteVertex(*this, *vi);

		vcg::tri::Allocator<MyMesh>::CompactEveryVector(*this);
	}

	inline int saveToDisk(const char* name)
	{
		std::cout << name << ":";
		int ret = vcg::tri::io::Exporter<MyMesh>::Save(
		              (*this),
		              name
		              );
		std::cout << "saving done " << ret << std::endl;
		return ret;
	}

	inline int openFromDisk(const char *name)
	{
		this->Clear();
		int err = vcg::tri::io::Importer<MeshType>::Open(*this, name);
		if (err)
		{
			if (vcg::tri::io::Importer<MeshType>::ErrorCritical(err))
				exit(-1);
		}
		this->updateBox();

		return err;
	}
};

typedef	typename MyMesh::ScalarType ScalarType;
typedef	typename MyMesh::CoordType  CoordType;
typedef	typename MyMesh::VertexPointer  VertexPointer;