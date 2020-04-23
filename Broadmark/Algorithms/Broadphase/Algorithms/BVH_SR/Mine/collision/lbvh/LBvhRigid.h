#ifndef __LBVH_RIGID_H_
#define __LBVH_RIGID_H_

#include <thrust/device_vector.h>
#include "setting\BvhSettings.h"

#include "world\scene.h"
#include "Core\Scene.h"	  //broadmarkIntegration
#include "Core\Settings.h"//broadmarkIntegration

#include "BvhExtNode.h"
#include "BvhIntNode.h"
#include "LBvhKernels.cuh"
#include "utility\CudaThrustUtils.hpp"

namespace mn {

	/// for now, maintenance work is configured by this input alone, regardless of Bvh state
	enum class LBvhRigidMaintenance { BUILD, REFIT };

	/*
	 * \brief	LBvhRigid
	 * \note	RAII
	 */
	class LBvhRigid {
	public:
		LBvhRigid() = delete;
		LBvhRigid(LBvhBuildConfig&& config);
		~LBvhRigid();
		void	maintain(LBvhRigidMaintenance scheme, const SceneData& pdata);
		void	maintain_BroadMarkEdition(LBvhRigidMaintenance scheme, const SceneFrame& fdata, const InflatedSettings& settings); // broadmarkIntegration

#if MACRO_VERSION
		void	maintain(LBvhRigidMaintenance scheme, const ARCSimSceneData& pdata);
#endif

		uint				getPrimNodeSize() { return _primSize;				 }
		uint				getExtNodeSize()  { return _extSize;				 }
		uint				getIntNodeSize()  { return _intSize;				 }
		BvhPrimitiveArray   cprim()			  { return _lvs.getPrimitiveArray(); }
		BvhExtNodeArray		clvs()			  { return _lvs;					 }
		BvhIntNodeArray		ctks()			  { return _tks;					 }
	private:

		void	build();
		void	build_BroadMarkEdition(const Aabb& worldAabb);// broadmarkIntegration
		void	refit();
		void	refit_BroadMarkEdition();// broadmarkIntegration

		void	updatePrimData(const SceneData& pdata); 
		void	updatePrimData_BroadMarkEdition(const SceneFrame& fdata, const InflatedSettings& settings);// broadmarkIntegration
		void	reorderPrims();
		void	reorderIntNodes();

		/// pre-formated input data
		int3*								d_faces;
		PointType*							d_vertices;
		Aabb*								d_aabb;// broadmarkIntegration

#if MACRO_VERSION
		uint3*								d_facesARCSim;
		g_box*								d_bxsARCSim;
#endif
		/// bvh
		int									_primSize, _extSize, _intSize;
		BOX*								d_bv;
		BvhExtNodeArray						_lvs;
		BvhIntNodeArray						_tks;
		/// auxiliary structures during maintenance procedure
		BvhPrimitiveArray					_unsortedPrims;
		BvhIntNodeArray						_unsortedTks;
		thrust::device_vector<int>			d_primMap;				///< map from primitives to leaves
		thrust::device_vector<int>			d_tkMap;
		thrust::device_vector<uint>			d_offsetTable;
		// sort
		thrust::device_vector<uint>			d_count;
		thrust::device_vector<uint>			d_keys32;
		thrust::device_vector<int>			d_vals;
	};

}


#endif