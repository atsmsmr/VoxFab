/*******************************************************************************
Copyright (c) 2016, Atsushi Masumori
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

// This software is developed based on VoxCad (https://svn.code.sf.net/p/voxcad/code/VoxCad/) developed by following author with following license.

/*******************************************************************************
Copyright (c) 2010, Jonathan Hiller
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * The name if its contributors may not be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include "Dlg_Workspace.h"
#include <math.h>


Dlg_Workspace::Dlg_Workspace(CVX_Object* pObjIn, QWidget *parent)
	: QWidget(parent)
{
	pObj = pObjIn;
	ui.setupUi(this);

//	ui.PresetWScombo->addItem("Cubes"); //sjhould be in same order as WorkspaceType enum
//	ui.PresetWScombo->addItem("FCC Spheres");
//	ui.PresetWScombo->addItem("HCP Spheres");
//	ui.PresetWScombo->addItem("DMU");
//	ui.PresetWScombo->addItem("Custom");

//	ui.VoxelCombo->addItem("Sphere"); //should be same order as #defines in CDM_Voxel
//	ui.VoxelCombo->addItem("Cube");
//	ui.VoxelCombo->addItem("Cylinder");

//	connect(ui.PresetWScombo, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangedScheme(int)));
//	connect(ui.LatDimSpin, SIGNAL(valueChanged(double)), this, SLOT(ChangedLatDim(double)));
//	connect(ui.XV_Spin, SIGNAL(valueChanged(int)), this, SLOT(ChangedVXDim(int)));
//	connect(ui.YV_Spin, SIGNAL(valueChanged(int)), this, SLOT(ChangedVYDim(int)));
//	connect(ui.ZV_Spin, SIGNAL(valueChanged(int)), this, SLOT(ChangedVZDim(int)));
	connect(ui.XV_Spin, SIGNAL(editingFinished()), this, SLOT(ChangedVXDim()));
	connect(ui.YV_Spin, SIGNAL(editingFinished()), this, SLOT(ChangedVYDim()));
	connect(ui.ZV_Spin, SIGNAL(editingFinished()), this, SLOT(ChangedVZDim()));

//    connect(ui.AdjustLattice, SIGNAL(clicked(bool)), this, SLOT(AdjustLattice()));
    connect(ui.CheckMesh, SIGNAL(clicked(bool)), this, SLOT(ChangeDrawMesh(bool)));
    connect(ui.CheckVoxel, SIGNAL(clicked(bool)), this, SLOT(ChangeDrawVoxel(bool)));
    connect(ui.ClearVoxelButton, SIGNAL(clicked(bool)), this, SLOT(ClearVoxel()));
    connect(ui.ClearMeshButton, SIGNAL(clicked(bool)), this, SLOT(ClearMesh()));

    // Voxelizer VoxelizeUnitRadio
    connect(ui.VoxelizeButton, SIGNAL(clicked(bool)), this, SLOT(ClickedVoxelize()));
    connect(ui.ImportMeshButton, SIGNAL(clicked(bool)), this, SLOT(ClickedImportSTL()));
    connect(ui.VoxelizeUnitCheck, SIGNAL(clicked(bool)), this, SLOT(ChangedUnitCheck(bool)));
//    connect(ui.VoxelizeDimRadio, SIGNAL(toggled(bool)), this, SLOT(ChangedDimRadio(bool)));

//    smooth mesh
//    connect(ui.Clear_Smesh, SIGNAL(clicked(bool)), this, SLOT(ClearSMesh()));
//    connect(ui.SmoothMesh, SIGNAL(clicked(bool)), this, SLOT(GetSmoothMesh()));

	//custom settings: Lattice
    connect(ui.LatticeDim_Spin, SIGNAL(valueChanged(double)), this, SLOT(ChangedLatticeDim(double)));
//	connect(ui.XDimAdj_Spin, SIGNAL(valueChanged(double)), this, SLOT(ChangedXDimAdj(double)));
//	connect(ui.YDimAdj_Spin, SIGNAL(valueChanged(double)), this, SLOT(ChangedYDimAdj(double)));
//	connect(ui.ZDimAdj_Spin, SIGNAL(valueChanged(double)), this, SLOT(ChangedZDimAdj(double)));
//	connect(ui.XLineOff_Spin, SIGNAL(valueChanged(double)), this, SLOT(ChangedXLiO(double)));
//	connect(ui.YLineOff_Spin, SIGNAL(valueChanged(double)), this, SLOT(ChangedYLiO(double)));
//	connect(ui.XLayerOff_Spin, SIGNAL(valueChanged(double)), this, SLOT(ChangedXLaO(double)));
//	connect(ui.YLayerOff_Spin, SIGNAL(valueChanged(double)), this, SLOT(ChangedYLaO(double)));

//	//custom settings: Voxel Display
//	connect(ui.VoxelCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangedVoxShape(int)));
//	connect(ui.XSqueeze_Spin, SIGNAL(valueChanged(double)), this, SLOT(ChangedXSqueeze(double)));
//	connect(ui.YSqueeze_Spin, SIGNAL(valueChanged(double)), this, SLOT(ChangedYSqueeze(double)));
//	connect(ui.ZSqueeze_Spin, SIGNAL(valueChanged(double)), this, SLOT(ChangedZSqueeze(double)));
}

Dlg_Workspace::~Dlg_Workspace()
{

}

void Dlg_Workspace::IniUpdateUI(void)
{
//	//special cases of presets:
//	if (pObj->Voxel.GetVoxName() == VS_BOX && pObj->Lattice.GetXDimAdj() == 1.0 && pObj->Lattice.GetYDimAdj() == 1.0 && pObj->Lattice.GetZDimAdj() == 1.0 && pObj->Lattice.GetXLiO() == 0.0 && pObj->Lattice.GetYLiO() == 0.0  && pObj->Lattice.GetXLaO() == 0.0 && pObj->Lattice.GetYLaO() == 0.0)
//		ui.PresetWScombo->setCurrentIndex(WSP_CUBES);
//	else if (pObj->Voxel.GetVoxName() == VS_SPHERE && pObj->Lattice.GetXDimAdj() == 1.0 && pObj->Lattice.GetYDimAdj() == 1.0 && abs(pObj->Lattice.GetZDimAdj() - 0.707107)<0.0001 && pObj->Lattice.GetXLiO() == 0.0 && pObj->Lattice.GetYLiO() == 0.0  && pObj->Lattice.GetXLaO() == 0.5 && pObj->Lattice.GetYLaO() == 0.5)
//		ui.PresetWScombo->setCurrentIndex(WSP_FCCSPHERES);
//	else if (pObj->Voxel.GetVoxName() == VS_SPHERE && pObj->Lattice.GetXDimAdj() == 1.0 && abs(pObj->Lattice.GetYDimAdj()-0.866025) < 0.0001 && abs(pObj->Lattice.GetZDimAdj() - 0.816496)<0.0001 && pObj->Lattice.GetXLiO() == 0.5 && pObj->Lattice.GetYLiO() == 0.0  && pObj->Lattice.GetXLaO() == 0.5 && abs(pObj->Lattice.GetYLaO()-0.288675)<0.0001 )
//		ui.PresetWScombo->setCurrentIndex(WSP_HCPSPHERES);
//	else
//		ui.PresetWScombo->setCurrentIndex(WSP_CUSTOM);
		
	UpdateUI();
}

void Dlg_Workspace::UpdateUI(void)
{
//    ui.LatDimSpin->setValue(pObj->GetLatticeDim()*1000); //1000 converts m to mm

	ui.XV_Spin->setValue(pObj->GetVXDim());
	ui.YV_Spin->setValue(pObj->GetVYDim());
	ui.ZV_Spin->setValue(pObj->GetVZDim());

//	switch (pObj->Voxel.GetVoxName()){
//		case VS_SPHERE:
//			ui.VoxelCombo->setCurrentIndex(VS_SPHERE); break;
//		case VS_BOX:
//			ui.VoxelCombo->setCurrentIndex(VS_BOX); break;
//		case VS_CYLINDER:
//			ui.VoxelCombo->setCurrentIndex(VS_CYLINDER); break;
//	}


	//Lattice Tab
    ui.LatticeDim_Spin->setValue((double)pObj->Lattice.GetLatticeDim());
//	ui.XDimAdj_Spin->setValue((double)pObj->Lattice.GetXDimAdj());
//	ui.YDimAdj_Spin->setValue((double)pObj->Lattice.GetYDimAdj());
//	ui.ZDimAdj_Spin->setValue((double)pObj->Lattice.GetZDimAdj());

    //Voxelizer Tab
    ui.CheckVoxel->setChecked(true);
    ui.CheckMesh->setChecked(false);
//    ui.VoxelizeDimRadio->setEnabled(false);
    ui.VoxelizeUnitCheck->setEnabled(true);

//	ui.XLineOff_Spin->setValue((double)pObj->Lattice.GetXLiO());
//	ui.YLineOff_Spin->setValue((double)pObj->Lattice.GetYLiO());

//	ui.XLayerOff_Spin->setValue((double)pObj->Lattice.GetXLaO());
//	ui.YLayerOff_Spin->setValue((double)pObj->Lattice.GetYLaO());

//	//Voxel Display tab
//	if (ui.PresetWScombo->currentIndex() == WSP_CUSTOM) ui.CustomSettings->setEnabled(true);
//	else ui.CustomSettings->setEnabled(false);

//	ui.XSqueeze_Spin->setValue((double)pObj->Voxel.GetXSqueeze());
//	ui.YSqueeze_Spin->setValue((double)pObj->Voxel.GetYSqueeze());
//	ui.ZSqueeze_Spin->setValue((double)pObj->Voxel.GetZSqueeze());

}

void Dlg_Workspace::ChangedScheme(int NewWSScheme)
{
	switch (NewWSScheme){
		case WSP_CUBES:
			pObj->Lattice.SetLattice(pObj->GetLatticeDim()); //function defaults to cubes!
			pObj->Voxel.SetVoxName(VS_BOX, 1.0, 1.0, 1.0);
			break;
		case WSP_FCCSPHERES:
			pObj->Lattice.SetLattice(pObj->GetLatticeDim(), 1.0, 1.0, sqrt(2.0)/2.0, 0, 0, 0.5, 0.5);
			pObj->Voxel.SetVoxName(VS_SPHERE, 1.0, 1.0, 1.0);
			break;
		case WSP_HCPSPHERES:
			pObj->Lattice.SetLattice(pObj->GetLatticeDim(), 1.0, sqrt(3.0)/2.0, sqrt(2.0/3.0), 0.5, 0, 0.5, 1.0/sqrt(12.0));
			pObj->Voxel.SetVoxName(VS_SPHERE, 1.0, 1.0, 1.0);
			break;
		case WSP_DMU:
			pObj->Lattice.SetLattice(0.0000846666, 0.5, 1.0, 0.3543); //function defaults to cubes!
			pObj->Voxel.SetVoxName(VS_BOX, 0.5, 1.0, 0.3543);
			break;


			break;
		//if switched to cutom, just leave everything where it is
	}

    UpdateUI();
	emit RequestUpdateGL();
}

void Dlg_Workspace::AdjustLattice(){

    int x_num = ceil( pObj->fileio.meshEntries[0]->mesh->bounding_box.length[0] / (pObj->Lattice.GetLatticeDim()*pObj->Lattice.GetXDimAdj()) );
    int y_num = ceil( pObj->fileio.meshEntries[0]->mesh->bounding_box.length[1] / (pObj->Lattice.GetLatticeDim()*pObj->Lattice.GetYDimAdj()) );
    int z_num = ceil( pObj->fileio.meshEntries[0]->mesh->bounding_box.length[2] / (pObj->Lattice.GetLatticeDim()*pObj->Lattice.GetZDimAdj()) );
    pObj->Structure.ClearStructure();
    pObj->Structure.CreateStructure(x_num,y_num,z_num);

    UpdateUI();

    emit RequestViewPerspective();
    emit RequestZoomExtAll();
    emit RequestUpdateGL();
}

void Dlg_Workspace::ClearVoxel(){
    pObj->Structure.CreateStructure(pObj->GetVXDim(),pObj->GetVYDim(),pObj->GetVZDim());
    pObj->is_draw_mesh = true;
    pObj->is_draw_voxel = false;
    emit RequestUpdateGL();
}

void Dlg_Workspace::ClearMesh(){
    if(pObj->fileio.is_loaded){
        pObj->fileio.clear();
        pObj->is_draw_mesh = false;
        pObj->fileio.is_loaded = false;
    }
    emit RequestUpdateGL();
}

void Dlg_Workspace::ClearSmoothMesh(){
    delete pObj->MeshUtil;
    pObj->is_draw_smooth = false;
    emit RequestUpdateGL();
}
