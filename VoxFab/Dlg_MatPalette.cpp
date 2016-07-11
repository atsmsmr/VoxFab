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

#include "Dlg_MatPalette.h"
#include <QMessageBox>
#include <QFileDialog>

Dlg_MatPalette::Dlg_MatPalette(QVX_Object* pObjIn, QWidget *parent)
	: QWidget(parent)
{
	pObj = pObjIn;
	ui.setupUi(this);

    Palette_Model = new CMPalModel(pObj, this);
//	Palette_Model.pObj = pObj; //connect our model to our object
	ui.MaterialList->setModel(Palette_Model);
	selectionModel = ui.MaterialList->selectionModel();
    SetCurMat(0);

	const QValidator* DEval = new QDoubleValidator(this); //constrain input to numbers only
	ui.ElasticEdit->setValidator(DEval);
	ui.PlasticEdit->setValidator(DEval);
	ui.YieldEdit->setValidator(DEval);
	ui.FailEdit->setValidator(DEval);
	ui.FailStrainEdit->setValidator(DEval);

	ui.PoissoEdit->setValidator(DEval);
	ui.DensityEdit->setValidator(DEval);
	ui.CTEEdit->setValidator(DEval);
	ui.TempPhaseEdit->setValidator(DEval);
	ui.UsEdit->setValidator(DEval);
	ui.UdEdit->setValidator(DEval);

	//Change selection
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(UpdateFields()));

	//add/delete materials
	connect(ui.AddMatButton, SIGNAL(clicked(bool)), this, SLOT(AddMaterial()));
	connect(ui.DelMatButton, SIGNAL(clicked(bool)), this, SLOT(DeleteCurMaterial()));

	//Load or Save Palettes
	connect(ui.LoadPalButton, SIGNAL(clicked()), this, SLOT(LoadPalette()));
	connect(ui.SavePalButton, SIGNAL(clicked()), this, SLOT(SavePalette()));

	//Visible
//	connect(ui.VisibleCheck, SIGNAL(clicked(bool)), this, SLOT(ChangedVisible(bool)));

//    //Material mode Radios
//    connect(ui.MTBasicRadio, SIGNAL(toggled(bool)), this, SLOT(ChangedBasic(bool)));
//    connect(ui.MTDitherRadio, SIGNAL(toggled(bool)), this, SLOT(ChangedDither(bool)));
//    connect(ui.MTStructureRadio, SIGNAL(toggled(bool)), this, SLOT(ChangedStructure(bool)));

	//model properties
	connect(ui.MatModelCombo, SIGNAL(activated(int)), this, SLOT(MatModeChanged(int)));
	connect(ui.ElasticEdit, SIGNAL(textChanged(QString)), this, SLOT(ChangedElasticMod(QString)));
	connect(ui.PlasticEdit, SIGNAL(textChanged(QString)), this, SLOT(ChangedPlasticMod(QString)));
	connect(ui.YieldEdit, SIGNAL(textChanged(QString)), this, SLOT(ChangedYieldStress(QString)));

	connect(ui.StressFailRadio, SIGNAL(clicked()), this, SLOT(ClickedStressFail()));
	connect(ui.StrainFailRadio, SIGNAL(clicked()), this, SLOT(ClickedStrainFail()));
	connect(ui.FailEdit, SIGNAL(textChanged(QString)), this, SLOT(ChangedFailStress(QString)));
	connect(ui.FailStrainEdit, SIGNAL(textChanged(QString)), this, SLOT(ChangedFailStrain(QString)));

	connect(ui.LoadSSButton, SIGNAL(clicked()), this, SLOT(ClickedLoadSS()));

	

	//physical properties
	connect(ui.PoissoEdit, SIGNAL(textChanged(QString)), this, SLOT(ChangedPoisson(QString)));
	connect(ui.DensityEdit, SIGNAL(textChanged(QString)), this, SLOT(ChangedDens(QString)));
	connect(ui.CTEEdit, SIGNAL(textChanged(QString)), this, SLOT(ChangedCTE(QString)));
	connect(ui.TempPhaseEdit, SIGNAL(textChanged(QString)), this, SLOT(ChangedTempPhase(QString)));
	connect(ui.UsEdit, SIGNAL(textChanged(QString)), this, SLOT(ChangeduStatic(QString)));
	connect(ui.UdEdit, SIGNAL(textChanged(QString)), this, SLOT(ChangeduDynamic(QString)));


	//set up editor window!!
	TmpEditDlg = new Dlg_StructureEditor(&TmpEdit, NULL); //shouldn't ever casue me leaks...

//	TmpEditDlg->setWindowTitle("Structure Editor");
//	TmpEditDlg->setWindowModality(Qt::ApplicationModal);
//	connect(TmpEditDlg, SIGNAL(DoneEditing()), this, SLOT(DoneEditStructure()));
//	UpdateMaterialWindow(QItemSelection());
}

Dlg_MatPalette::~Dlg_MatPalette()
{

}

void Dlg_MatPalette::AddMaterial(void)
{
	Palette_Model->insertRows(0,0);
	UpdateFields();

}

void Dlg_MatPalette::DeleteCurMaterial(void)
{
	int CurMat = GetCurMat();
	if (CurMat > 0) Palette_Model->removeRows(CurMat,CurMat); //don't remove the null material, or try to remove -1
	emit RequestUpdateGL();
	UpdateFields();

	
}

void Dlg_MatPalette::LoadPalette(void)
{
	pObj->OpenPal();
	Palette_Model->UpdateList(); //to update the list...
	emit RequestUpdateGL();

}

void Dlg_MatPalette::SavePalette(void)
{
	pObj->SavePal();
}

void Dlg_MatPalette::GetCurMat(int* MatIndex)
{
	QModelIndexList indexes = selectionModel->selectedIndexes();
	if (!indexes.isEmpty())	*MatIndex = indexes.first().row();
	else *MatIndex = 0; //return erase if no material selected!
}

void Dlg_MatPalette::SetCurMat(int NewMatIndex)
{
	QModelIndex tmp = ui.MaterialList->model()->index(NewMatIndex, 0);
	ui.MaterialList->setCurrentIndex(tmp);
}

void Dlg_MatPalette::ChangedR(int RVal)
{
	int CurMat = GetCurMat();
	if (CurMat > 0) GetPCurMat()->SetRed(RVal/255.0);
	Palette_Model->UpdateList(); //to update the icon color
	emit RequestUpdateGL();
}

void Dlg_MatPalette::ChangedG(int GVal)
{
	int CurMat = GetCurMat();
	if (CurMat > 0) GetPCurMat()->SetGreen(GVal/255.0);
	Palette_Model->UpdateList(); //to update the icon color
	emit RequestUpdateGL();
}

void Dlg_MatPalette::ChangedB(int BVal)
{
	int CurMat = GetCurMat();
	if (CurMat > 0) GetPCurMat()->SetBlue(BVal/255.0);
	Palette_Model->UpdateList(); //to update the icon color
	emit RequestUpdateGL();
}

void Dlg_MatPalette::ChangedA(int AVal)
{
	int CurMat = GetCurMat();
	if (CurMat > 0) GetPCurMat()->SetAlpha(AVal/255.0);
	Palette_Model->UpdateList(); //to update the icon color
	emit RequestUpdateGL();
}

void Dlg_MatPalette::MatModeChanged(int NewMatMode)
{
	GetPCurMat()->SetMatModel(NewMatMode);
	if (NewMatMode == MDL_DATA) GetPCurMat()->ValidateSSData();
	UpdateFields();
}


void Dlg_MatPalette::Mat1IndexChanged(int NewMatIndex)
{
	if (NewMatIndex>=0 && NewMatIndex != GetCurMat() && MatIsEditable(NewMatIndex)){ //filter for selecting anything not allowed, since its a pain to disable in QComboBox
		GetPCurMat()->SetRandInd1(NewMatIndex);
		emit RequestUpdateGL();
	}
	UpdateFields();

}

void Dlg_MatPalette::Mat2IndexChanged(int NewMatIndex)
{
	if (NewMatIndex>=0 && NewMatIndex != GetCurMat() && MatIsEditable(NewMatIndex)){ //filter for selecting anything not allowed, since its a pain to disable in QComboBox
		GetPCurMat()->SetRandInd2(NewMatIndex);
		emit RequestUpdateGL();
	}
	UpdateFields();

}

void Dlg_MatPalette::ChangedPercRand(int RVal)
{
	int CurMat = GetCurMat();
	if (CurMat > 0) GetPCurMat()->SetRandPerc1(RVal/100.0);
	emit RequestUpdateGL();
}

//void Dlg_MatPalette::ChangedXDim(int XDimVal)
//{
//	GetPCurMat()->SetSubXSize(XDimVal);
//	if (GetPCurMat()->GetSubXOffset() >= GetPCurMat()->GetSubXSize()) ui.XOffSpin->setValue(XDimVal-1);
//	ui.XOffSpin->setRange(0, XDimVal-1); //keep offsets within range...
//	emit RequestUpdateGL();
//}

//void Dlg_MatPalette::ChangedXOff(int XOffVal)
//{
//	GetPCurMat()->SetSubXOffset(XOffVal);
//	emit RequestUpdateGL();
//}

//void Dlg_MatPalette::ChangedYDim(int YDimVal)
//{
//	GetPCurMat()->SetSubYSize(YDimVal);
//	if (GetPCurMat()->GetSubYOffset() >= GetPCurMat()->GetSubYSize()) ui.YOffSpin->setValue(YDimVal-1);
//	ui.YOffSpin->setRange(0, YDimVal-1); //keep offsets within range...
//	emit RequestUpdateGL();
//}

//void Dlg_MatPalette::ChangedYOff(int YOffVal)
//{
//	GetPCurMat()->SetSubYOffset(YOffVal);
//	emit RequestUpdateGL();
//}

//void Dlg_MatPalette::ChangedZDim(int ZDimVal)
//{
//	GetPCurMat()->SetSubZSize(ZDimVal);
//	if (GetPCurMat()->GetSubZOffset() >= GetPCurMat()->GetSubZSize()) ui.ZOffSpin->setValue(ZDimVal-1);
//	ui.ZOffSpin->setRange(0, ZDimVal-1); //keep offsets within range...
//	emit RequestUpdateGL();
//}

//void Dlg_MatPalette::ChangedZOff(int ZOffVal)
//{
//	GetPCurMat()->SetSubZOffset(ZOffVal);
//	emit RequestUpdateGL();
//}

void Dlg_MatPalette::ClickedEditStructure(void)
{
	if (GetPCurMat()->GetMatType() != INTERNAL) return;

	TmpEdit.Structure = GetPCurMat()->GetStructure();
	TmpEdit.Lattice = pObj->Lattice;
	TmpEdit.Palette = pObj->Palette; //needs work to avoid recusion!!
	std::string tmpName;
	for (int i=0; i<TmpEdit.GetNumMaterials(); i++){
		MatIsEditable(i, &tmpName);
		TmpEdit.Palette[i].SetName(tmpName);
//		if (i == GetCurMat()) TmpEdit.Palette[i].SetName(TmpEdit.Palette[i].GetName() + " (Editing)");
//		else if (TmpEdit.IsInRecursivePath(i, GetCurMat())) TmpEdit.Palette[i].SetName(TmpEdit.Palette[i].GetName() + " (Recursive)");
	}
	TmpEdit.Voxel = pObj->Voxel;

	TmpEditDlg->IniUpdateUI();
	TmpEditDlg->show();

}

bool Dlg_MatPalette::MatIsEditable(int MatIndex, std::string* ModName) //check for self reference, recursion...
{
	if (MatIndex == GetCurMat()){
		if (ModName) *ModName = pObj->Palette[MatIndex].GetName() + " (Editing)";
		return false;
	}
	else if (pObj->IsInRecursivePath(MatIndex, GetCurMat())){
		if (ModName) *ModName = pObj->Palette[MatIndex].GetName() + " (Recursive)";
		return false;
	}
	else {
		if (ModName) *ModName = pObj->Palette[MatIndex].GetName();
		return true;
	}
}

void Dlg_MatPalette::DoneEditStructure(void)
{
	GetPCurMat()->GetStructure() = TmpEdit.Structure;
	emit RequestUpdateGL();
}

void Dlg_MatPalette::ClickedImport(void) //does not currently support material recursion within the imported VXC (flattens it)
{
	QVX_Object tmpObj;
	std::string RetMsg;
	tmpObj.Open();
	int MatShiftNum = pObj->GetNumMaterials()-1; //amound to shift materials indices from imported VXC
	for (int i=1; i<tmpObj.GetNumMaterials(); i++){
		if (pObj->AddMat(tmpObj.Palette[i], true, &RetMsg) == -1){
			QMessageBox::warning(NULL, "Warning", "Could not add material. Aborting import.");
			return;
		}
	}

	for (int i=tmpObj.GetNumMaterials()-1; i>0; i--) //go backwards to avoid doubling up indices
		tmpObj.Structure.ReplaceMaterial(i, MatShiftNum+i); //temporarily creates bad indices, but they'll drop right in when we put this in our material structure

	GetPCurMat()->GetStructure() = tmpObj.Structure; //set the structure!
	UpdateFields(); //update the dimension sizes
	emit RequestUpdateGL();
}

void Dlg_MatPalette::ClickedExport(void) //does not currently support material recursion within the exported VXC (flattens it)
{
	QVX_Object tmp;
	tmp.Voxel = pObj->Voxel;
	tmp.Lattice = pObj->Lattice;
	tmp.Structure = GetPCurMat()->GetStructure();
	for (int i=0; i<pObj->GetNumMaterials(); i++){ //check each material if its used in this microstrucutre, add to palette if so...
		if (tmp.Structure.ContainsMatIndex(i)){ //if this material is in the substructure
			int ThisIndex = tmp.AddMat(pObj->Palette[i], true);
			tmp.Structure.ReplaceMaterial(i, ThisIndex);
		}
	}
	tmp.Save();
}

void Dlg_MatPalette::UpdateFields(void) //updates window based on current material selection, etc.
{
	int CurMat = GetCurMat();
//	ui.VisibleCheck->setChecked(GetPCurMat()->IsVisible());
	
	if (CurMat == 0){ //if erase.nothing selected
//		ui.VisibleCheck->setEnabled(false);
//        ui.MatTypeGroup->setEnabled(false);
        ui.DelMatButton->setEnabled(false);

    } else { //if a valid material selected
//        ui.VisibleCheck->setEnabled(false);
//        ui.MatTypeGroup->setEnabled(false);
        ui.DelMatButton->setEnabled(false);

		switch (GetPCurMat()->GetMatType()){
		case SINGLE:
//            ui.MTBasicRadio->setChecked(true);
            //Model Tab:
			ui.ModelTab->setEnabled(true);

			ui.MatModelCombo->clear();
			ui.MatModelCombo->addItem("Linear");
			ui.MatModelCombo->addItem("Linear + Failure");
			ui.MatModelCombo->addItem("BiLinear");
			ui.MatModelCombo->addItem("Data");
			ui.MatModelCombo->setCurrentIndex(GetPCurMat()->GetMatModel());

			ui.ElasticEdit->setText(QString::number(GetPCurMat()->GetElasticMod()/1000000.0, 'g', 4));
			ui.PlasticEdit->setText(QString::number(GetPCurMat()->GetPlasticMod()/1000000.0, 'g', 4));
			ui.YieldEdit->setText(QString::number(GetPCurMat()->GetYieldStress()/1000000.0, 'g', 4));
			ui.FailEdit->setText(QString::number(GetPCurMat()->GetFailStress()/1000000.0, 'g', 4));
			ui.FailStrainEdit->setText(QString::number(GetPCurMat()->GetFailStrain(), 'g', 4));

			ui.ElasticEdit->setEnabled(false);
			ui.PlasticEdit->setEnabled(false);
			ui.YieldEdit->setEnabled(false);
			ui.FailEdit->setEnabled(false);
			ui.FailStrainEdit->setEnabled(false);
			ui.LoadSSButton->setEnabled(false);
			ui.StressFailRadio->setChecked(false);
			ui.StrainFailRadio->setChecked(false);
			ui.StressFailRadio->setEnabled(false);
			ui.StrainFailRadio->setEnabled(false);

			switch(GetPCurMat()->GetMatModel()){
			case MDL_LINEAR:
				ui.ElasticEdit->setEnabled(true);
				break;
			case MDL_LINEAR_FAIL:
				ui.ElasticEdit->setEnabled(true);
				break;
			case MDL_BILINEAR:
				ui.ElasticEdit->setEnabled(true);
				ui.PlasticEdit->setEnabled(true);
				ui.YieldEdit->setEnabled(true);
				break;
			case MDL_DATA:
				ui.LoadSSButton->setEnabled(true);
				break;
			}

			if (GetPCurMat()->GetMatModel() == MDL_LINEAR_FAIL || GetPCurMat()->GetMatModel() == MDL_BILINEAR){ 
				ui.StressFailRadio->setEnabled(true);
				ui.StrainFailRadio->setEnabled(true);
				if (GetPCurMat()->GetFailModel() == FM_MAXSTRAIN){
					ui.StrainFailRadio->setChecked(true);
					ui.FailStrainEdit->setEnabled(true);
				}
				else { //FailModel() == FM_MAXSTRESS
					ui.StressFailRadio->setChecked(true);
					ui.FailEdit->setEnabled(true);
				}
			}

			//Physical Tab:
			ui.PhysicalTab->setEnabled(true);
			ui.PoissoEdit->setText(QString::number(GetPCurMat()->GetPoissonsRatio(), 'g', 3));
			ui.DensityEdit->setText(QString::number(GetPCurMat()->GetDensity(), 'g', 4));
			ui.CTEEdit->setText(QString::number(GetPCurMat()->GetCTE(), 'g', 3));
			ui.TempPhaseEdit->setText(QString::number(GetPCurMat()->GetMatTempPhase(), 'g', 4));
			ui.UsEdit->setText(QString::number(GetPCurMat()->GetuStatic(), 'g', 3));
			ui.UdEdit->setText(QString::number(GetPCurMat()->GetuDynamic(), 'g', 3));


			break;
		}
	}
	Palette_Model->UpdateList(); //probably already emitted, because setting color values calls this, but apart from performance concerns, this won't hurt anything
}

void Dlg_MatPalette::UpdateVisibleTabs(void)
{
	while (ui.tabProperties->count() != 0) //remove all the tabs (we'll add back the ones we need based on the material mode
		ui.tabProperties->removeTab(0);

	switch (GetPCurMat()->GetMatType()){
		case SINGLE:
			ui.tabProperties->insertTab(0, ui.PhysicalTab, "Physical");
			ui.tabProperties->insertTab(0, ui.ModelTab, "Model");
			ui.tabProperties->setCurrentIndex(0);
			break;
//		case DITHER:
//			ui.tabProperties->insertTab(0, ui.DitherTab, "Dither");
//			ui.tabProperties->insertTab(0, ui.AppearanceTab, "Appearance");
//			break;
//		case INTERNAL:
//			ui.tabProperties->insertTab(0, ui.StructureTab, "Structure");
//			ui.tabProperties->insertTab(0, ui.AppearanceTab, "Appearance");
//			break;
	}
}

void Dlg_MatPalette::ImportSSData(void)
{
	QString tmpPath = QFileDialog::getOpenFileName(NULL, "Open Data", "", "Text (Tab delimited) (*.txt)");
	QString StrainInStr, StressInStr;
	std::vector<vfloat> StrainIn;
	std::vector<vfloat> StressIn;

	if (!tmpPath.isNull()){
		QFile File(tmpPath);
		if (!File.open(QIODevice::ReadOnly | QIODevice::Text)) return;
		QTextStream in(&File); 

		while(true){
			QString line = in.readLine();
			if (line == QString()) break;
			StrainInStr = line.section('\t', 0, 0);
			StressInStr = line.section('\t', 1, 1);
			StrainIn.push_back(StrainInStr.toDouble());
			StressIn.push_back(StressInStr.toDouble());

		}
	}

	std::string RetMsg;
	if (!GetPCurMat()->SetSSData(&StrainIn, &StressIn, &RetMsg)) QMessageBox::warning(NULL, "Import Error", RetMsg.c_str());
}



//MODEL

int CMPalModel::rowCount(const QModelIndex &parent) const
{
     return pObj->GetNumMaterials();
}

QVariant CMPalModel::data(const QModelIndex &index, int role) const
 {
	 if (!index.isValid())
         return QVariant();

	if (index.row() >= pObj->GetNumMaterials())
         return QVariant();

	if (role == Qt::DisplayRole){
		std::string tmp = (pObj->Palette[index.row()].GetName());
		if (pObj->Palette[index.row()].GetMatType()== DITHER) tmp += " (Dither)";
		else if (pObj->Palette[index.row()].GetMatType()== INTERNAL) tmp += " (Structure)";
		return QVariant(tmp.c_str());
	}
	if (role ==  Qt::EditRole){
		return QVariant((pObj->Palette[index.row()].GetName()).c_str());
	}
	else if (role == Qt::DecorationRole){
		int R, G, B;
//		pObj->Palette[index.row()].GetColori(&R, &G, &B);
        return QColor(255,255,255);
	}
	else
		return QVariant();

 }

Qt::ItemFlags CMPalModel::flags(const QModelIndex &index) const
{
	if (!index.isValid()) return Qt::ItemIsEnabled;

	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool CMPalModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.isValid() && role == Qt::EditRole) {

        pObj->Palette[index.row()].SetName(value.toString().toStdString());
        emit dataChanged(index, index);
		return true;
	}
	return false;
}

bool CMPalModel::insertRows(int row, int count, const QModelIndex & parent)
{
	beginInsertRows(parent, rowCount(), rowCount()); //always only add one to the end
	pObj->AddMat("Default");
	endInsertRows();
	return true;
}

bool CMPalModel::removeRows(int row, int count, const QModelIndex & parent)
{
	beginRemoveRows(parent, row, row); //removes just the one material
	if (pObj->GetBaseMat(row)->GetMatType() != SINGLE){ //if this is a compound material
		if (QMessageBox::question(NULL, "Warning", "Do you want to flatten (preserve) sub-voxels before delete?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Yes) == QMessageBox::Yes){
			pObj->DeleteMat(row, true);
		}
		else pObj->DeleteMat(row);
	}	
	else pObj->DeleteMat(row);
	endRemoveRows();
	return true;
}



void CMPalModel::UpdateList(void)
{
	emit dataChanged(index(0,0), index(pObj->GetNumMaterials(), 0));
}
