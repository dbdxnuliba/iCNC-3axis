#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <vcclr.h>
#include <windows.h>
#include ".\DRV\INC\switch.h"
#include ".\DRV\INC\stepMotorDRV.h"

#include "CNC\INC\cnc.h"

CNC_DATA	cncData;		//entity declare
CNC_DATA	*cncDataPtr = &cncData;	//pointer declare & point to entity

bool	uvwValid = false;

int realKnl(CNC_DATA *cncDataPtr)
{
	//******************************************
	// intp
	//******************************************
	cncDataPtr->intpDataPtr->uvwEnd[0] += 0.5 * cncDataPtr->mmiDataPtr->uvwButton[0];
	cncDataPtr->intpDataPtr->uvwEnd[1] += 0.5 * cncDataPtr->mmiDataPtr->uvwButton[1];
	cncDataPtr->intpDataPtr->uvwEnd[2] += 0.15 * cncDataPtr->mmiDataPtr->uvwButton[2];

	//******************************************
	// position control 
	//******************************************
	int i;

	for( i = 0 ; i<3 ; ++i)
	{
		// relInc = (set - oldSet) / resolution
		double relDistance = cncDataPtr->intpDataPtr->uvwEnd[i] - cncDataPtr->intpDataPtr->uvwOld[i];
		double uvwRemain[3];
		// 	calculate relative increment
		if (relDistance != 0)
		{
			cncDataPtr->intpDataPtr->uvwIncrement[i] = (int)(relDistance / cncDataPtr->intpDataPtr->uvwResolution[i]);

			// ================== remain value ================== //
			uvwRemain[i] += ( relDistance - cncDataPtr->intpDataPtr->uvwIncrement[i] * cncDataPtr->intpDataPtr->uvwResolution[i]);
			// remainValue > 0 && resolution <= remainValue
			if( !( cncDataPtr->intpDataPtr->uvwResolution[i] > uvwRemain[i] ) )
			{
				cncDataPtr->intpDataPtr->uvwIncrement[i] ++;
				uvwRemain[i] -= cncDataPtr->intpDataPtr->uvwResolution[i];
			}
			// remainValue < 0 && resolution <= (-1)*remainValue
			else if(  !( cncDataPtr->intpDataPtr->uvwResolution[i] > (-1)*uvwRemain[i] ) )
			{
				cncDataPtr->intpDataPtr->uvwIncrement[i] --;
				uvwRemain[i] += cncDataPtr->intpDataPtr->uvwResolution[i];
			}
			// ================================================== //

		}
		else 
			cncDataPtr->intpDataPtr->uvwIncrement[i] = 0;

	}//end for

	// output
	for (i = 0 ; i < 3 ; ++i)
	{
		// send to motion card when we use HD_TIMER
#if HD_TIMER
		putAxisValue(i, cncDataPtr->intpDataPtr->uvwIncrement[i]);
#endif

		cncDataPtr->intpDataPtr->uvwNew[i] = cncDataPtr->intpDataPtr->uvwEnd[i];
		cncDataPtr->intpDataPtr->uvwOld[i] = cncDataPtr->intpDataPtr->uvwEnd[i];

	}//end for


	cncDataPtr->intpDataPtr->xyzNew[0] = cncDataPtr->intpDataPtr->l1 * cos(angleCycle(degToRad(cncDataPtr->intpDataPtr->uvwNew[0])) )
										+ cncDataPtr->intpDataPtr->l2 * cos(angleCycle(degToRad(cncDataPtr->intpDataPtr->uvwNew[1])) );
	cncDataPtr->intpDataPtr->xyzNew[1] = cncDataPtr->intpDataPtr->l1 * sin(angleCycle(degToRad(cncDataPtr->intpDataPtr->uvwNew[0])) ) 
										+ cncDataPtr->intpDataPtr->l2 * sin(angleCycle(degToRad(cncDataPtr->intpDataPtr->uvwNew[1])) );
	cncDataPtr->intpDataPtr->xyzNew[2] = cncDataPtr->intpDataPtr->uvwNew[2];



		return 0;
}

//**********************************************
// Real timer function
//**********************************************
#if HD_TIMER
void _stdcall Timer_ISR_Function(LIOINT *pstINTSource)
{
	cncRtCtl(cncDataPtr);
	if(cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode && uvwValid == true)
		realKnl(cncDataPtr);
}
#endif

namespace iCNC_V1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Form1 的摘要
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::RichTextBox^  richTextBox_ncFile;
	protected: 

	protected: 

	private: System::Windows::Forms::Button^  button_OPEN;
	private: System::Windows::Forms::RichTextBox^  richTextBox_filePath;
	protected: 



	protected: 

	protected: 



	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::Label^  label_NZ;

	private: System::Windows::Forms::Label^  label_NY;

	private: System::Windows::Forms::Label^  label_NX;

	private: System::Windows::Forms::Label^  label_INZ;

	private: System::Windows::Forms::Label^  label_INY;

	private: System::Windows::Forms::Label^  label_INX;

	private: System::Windows::Forms::Label^  label_ERR;

	private: System::Windows::Forms::Label^  label_M30;

	private: System::Windows::Forms::Label^  label_PZ;
	private: System::Windows::Forms::Label^  label_PY;
	private: System::Windows::Forms::Label^  label_PX;
	private: System::Windows::Forms::Label^  label_HZ;
	private: System::Windows::Forms::Label^  label_HY;
	private: System::Windows::Forms::Label^  label_HX;
	private: System::Windows::Forms::Label^  label_PGM;
	private: System::Windows::Forms::Label^  label_M00;
	private: System::Windows::Forms::Timer^  RT_timer;
	private: System::Windows::Forms::Timer^  NRT_timer;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog_openFile;



	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel2;
	private: System::Windows::Forms::Label^  label_hour;
	private: System::Windows::Forms::Label^  label_year;
	private: System::Windows::Forms::Button^  button_X1;
	private: System::Windows::Forms::Button^  button_X10;
	private: System::Windows::Forms::Button^  button_X100;
	private: System::Windows::Forms::Button^  button_TEACHSTART;


	private: System::Windows::Forms::Button^  button_TEACHEND;
	private: System::Windows::Forms::Button^  button_SB;
	private: System::Windows::Forms::Button^  button_FH;
	private: System::Windows::Forms::Button^  button_M01;
	private: System::Windows::Forms::TrackBar^  trackBar_feedOverride;


	private: System::Windows::Forms::Button^  button_START;
	private: System::Windows::Forms::Button^  button_RESET;

	private: System::Windows::Forms::Button^  button_EMERGENCYSTOP;
	private: System::Windows::Forms::GroupBox^  groupBox_XYZcoordinate;



















	private: System::Windows::Forms::Button^  button_Xplus;
	private: System::Windows::Forms::Button^  button_Yplus;
	private: System::Windows::Forms::Button^  button_Zplus;
	private: System::Windows::Forms::Button^  button_Xminus;
	private: System::Windows::Forms::Button^  button_Yminus;
	private: System::Windows::Forms::Button^  button_Zminus;
private: System::Windows::Forms::Button^  button_RECORD;
private: System::Windows::Forms::Button^  button_MEM;
private: System::Windows::Forms::Button^  button_MDI;
private: System::Windows::Forms::Button^  button_JOG;
private: System::Windows::Forms::Button^  button_WHEEL;
private: System::Windows::Forms::Button^  button_TEACH;
private: System::Windows::Forms::Button^  button_HOME;
private: System::Windows::Forms::Button^  button_diagnosis;
private: System::Windows::Forms::Button^  button_simulation;
private: System::Windows::Forms::Button^  button_parameter;
private: System::Windows::Forms::Label^  label_FO;


private: System::Windows::Forms::Label^  label_feedOverride;
private: System::Windows::Forms::Label^  label_coordZ;
private: System::Windows::Forms::Label^  label_coordY;
private: System::Windows::Forms::Label^  label_coordX;



private: System::Windows::Forms::Label^  label_X;
private: System::Windows::Forms::Label^  label_Z;

private: System::Windows::Forms::Label^  label_Y;
private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart_linkage;








private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;
private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel3;
private: System::Windows::Forms::Label^  label_decLineNumber;

private: System::Windows::Forms::Label^  label_Increment;






private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel4;
private: System::Windows::Forms::Label^  label_vIncre;

private: System::Windows::Forms::Label^  label_uIncre;
private: System::Windows::Forms::Label^  label_totalLength;
private: System::Windows::Forms::Label^  label_residualLength;



private: System::Windows::Forms::Label^  label_sumLength;

private: System::Windows::Forms::Label^  label_wIncre;

private: System::Windows::Forms::GroupBox^  groupBox_UVWcoordinate;
private: System::Windows::Forms::Label^  label_W;
private: System::Windows::Forms::Label^  label_V;
private: System::Windows::Forms::Label^  label_coordV;
private: System::Windows::Forms::Label^  label_coordW;
private: System::Windows::Forms::Label^  label_U;
private: System::Windows::Forms::Label^  label_coordU;

private: System::Windows::Forms::Label^  label3;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::Label^  label_intpLineNumber;

private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::TabControl^  tabControl_axisButton;
private: System::Windows::Forms::TabPage^  tabPage_XYZ;
private: System::Windows::Forms::TabPage^  tabPage_UVW;
private: System::Windows::Forms::Button^  button_Uminus;
private: System::Windows::Forms::Button^  button_Uplus;
private: System::Windows::Forms::Button^  button_Wminus;
private: System::Windows::Forms::Button^  button_Vplus;
private: System::Windows::Forms::Button^  button_Vminus;
private: System::Windows::Forms::Button^  button_Wplus;
private: System::Windows::Forms::Button^  button_CONTINUE;
private: System::Windows::Forms::Button^  button1;








	private: System::ComponentModel::IContainer^  components;


	protected: 


	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器
		/// 修改這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea2 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::Series^  series5 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->richTextBox_ncFile = (gcnew System::Windows::Forms::RichTextBox());
			this->button_OPEN = (gcnew System::Windows::Forms::Button());
			this->richTextBox_filePath = (gcnew System::Windows::Forms::RichTextBox());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->label_NZ = (gcnew System::Windows::Forms::Label());
			this->label_NY = (gcnew System::Windows::Forms::Label());
			this->label_NX = (gcnew System::Windows::Forms::Label());
			this->label_INZ = (gcnew System::Windows::Forms::Label());
			this->label_INY = (gcnew System::Windows::Forms::Label());
			this->label_INX = (gcnew System::Windows::Forms::Label());
			this->label_ERR = (gcnew System::Windows::Forms::Label());
			this->label_M30 = (gcnew System::Windows::Forms::Label());
			this->label_PY = (gcnew System::Windows::Forms::Label());
			this->label_PX = (gcnew System::Windows::Forms::Label());
			this->label_HZ = (gcnew System::Windows::Forms::Label());
			this->label_HY = (gcnew System::Windows::Forms::Label());
			this->label_HX = (gcnew System::Windows::Forms::Label());
			this->label_M00 = (gcnew System::Windows::Forms::Label());
			this->label_PZ = (gcnew System::Windows::Forms::Label());
			this->label_PGM = (gcnew System::Windows::Forms::Label());
			this->RT_timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->NRT_timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->openFileDialog_openFile = (gcnew System::Windows::Forms::OpenFileDialog());
			this->tableLayoutPanel2 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->label_hour = (gcnew System::Windows::Forms::Label());
			this->label_year = (gcnew System::Windows::Forms::Label());
			this->button_X1 = (gcnew System::Windows::Forms::Button());
			this->button_X10 = (gcnew System::Windows::Forms::Button());
			this->button_X100 = (gcnew System::Windows::Forms::Button());
			this->button_TEACHSTART = (gcnew System::Windows::Forms::Button());
			this->button_TEACHEND = (gcnew System::Windows::Forms::Button());
			this->button_SB = (gcnew System::Windows::Forms::Button());
			this->button_FH = (gcnew System::Windows::Forms::Button());
			this->button_M01 = (gcnew System::Windows::Forms::Button());
			this->trackBar_feedOverride = (gcnew System::Windows::Forms::TrackBar());
			this->button_START = (gcnew System::Windows::Forms::Button());
			this->button_RESET = (gcnew System::Windows::Forms::Button());
			this->button_EMERGENCYSTOP = (gcnew System::Windows::Forms::Button());
			this->groupBox_XYZcoordinate = (gcnew System::Windows::Forms::GroupBox());
			this->label_Z = (gcnew System::Windows::Forms::Label());
			this->label_X = (gcnew System::Windows::Forms::Label());
			this->label_Y = (gcnew System::Windows::Forms::Label());
			this->label_coordZ = (gcnew System::Windows::Forms::Label());
			this->label_coordY = (gcnew System::Windows::Forms::Label());
			this->label_coordX = (gcnew System::Windows::Forms::Label());
			this->button_Xplus = (gcnew System::Windows::Forms::Button());
			this->button_Yplus = (gcnew System::Windows::Forms::Button());
			this->button_Zplus = (gcnew System::Windows::Forms::Button());
			this->button_Xminus = (gcnew System::Windows::Forms::Button());
			this->button_Yminus = (gcnew System::Windows::Forms::Button());
			this->button_Zminus = (gcnew System::Windows::Forms::Button());
			this->button_RECORD = (gcnew System::Windows::Forms::Button());
			this->button_MEM = (gcnew System::Windows::Forms::Button());
			this->button_MDI = (gcnew System::Windows::Forms::Button());
			this->button_JOG = (gcnew System::Windows::Forms::Button());
			this->button_WHEEL = (gcnew System::Windows::Forms::Button());
			this->button_TEACH = (gcnew System::Windows::Forms::Button());
			this->button_HOME = (gcnew System::Windows::Forms::Button());
			this->button_diagnosis = (gcnew System::Windows::Forms::Button());
			this->button_simulation = (gcnew System::Windows::Forms::Button());
			this->button_parameter = (gcnew System::Windows::Forms::Button());
			this->label_FO = (gcnew System::Windows::Forms::Label());
			this->label_feedOverride = (gcnew System::Windows::Forms::Label());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->chart_linkage = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->tableLayoutPanel3 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->label_vIncre = (gcnew System::Windows::Forms::Label());
			this->label_wIncre = (gcnew System::Windows::Forms::Label());
			this->label_uIncre = (gcnew System::Windows::Forms::Label());
			this->label_Increment = (gcnew System::Windows::Forms::Label());
			this->label_decLineNumber = (gcnew System::Windows::Forms::Label());
			this->tableLayoutPanel4 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->label_totalLength = (gcnew System::Windows::Forms::Label());
			this->label_residualLength = (gcnew System::Windows::Forms::Label());
			this->label_sumLength = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->groupBox_UVWcoordinate = (gcnew System::Windows::Forms::GroupBox());
			this->label_W = (gcnew System::Windows::Forms::Label());
			this->label_V = (gcnew System::Windows::Forms::Label());
			this->label_coordV = (gcnew System::Windows::Forms::Label());
			this->label_coordW = (gcnew System::Windows::Forms::Label());
			this->label_U = (gcnew System::Windows::Forms::Label());
			this->label_coordU = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label_intpLineNumber = (gcnew System::Windows::Forms::Label());
			this->tabControl_axisButton = (gcnew System::Windows::Forms::TabControl());
			this->tabPage_XYZ = (gcnew System::Windows::Forms::TabPage());
			this->tabPage_UVW = (gcnew System::Windows::Forms::TabPage());
			this->button_Uminus = (gcnew System::Windows::Forms::Button());
			this->button_Uplus = (gcnew System::Windows::Forms::Button());
			this->button_Wminus = (gcnew System::Windows::Forms::Button());
			this->button_Vplus = (gcnew System::Windows::Forms::Button());
			this->button_Vminus = (gcnew System::Windows::Forms::Button());
			this->button_Wplus = (gcnew System::Windows::Forms::Button());
			this->button_CONTINUE = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->tableLayoutPanel1->SuspendLayout();
			this->tableLayoutPanel2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar_feedOverride))->BeginInit();
			this->groupBox_XYZcoordinate->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart_linkage))->BeginInit();
			this->tableLayoutPanel3->SuspendLayout();
			this->tableLayoutPanel4->SuspendLayout();
			this->groupBox_UVWcoordinate->SuspendLayout();
			this->tabControl_axisButton->SuspendLayout();
			this->tabPage_XYZ->SuspendLayout();
			this->tabPage_UVW->SuspendLayout();
			this->SuspendLayout();
			// 
			// richTextBox_ncFile
			// 
			this->richTextBox_ncFile->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->richTextBox_ncFile->Location = System::Drawing::Point(243, 112);
			this->richTextBox_ncFile->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->richTextBox_ncFile->Name = L"richTextBox_ncFile";
			this->richTextBox_ncFile->Size = System::Drawing::Size(487, 173);
			this->richTextBox_ncFile->TabIndex = 0;
			this->richTextBox_ncFile->Text = L"";
			// 
			// button_OPEN
			// 
			this->button_OPEN->BackColor = System::Drawing::Color::Gainsboro;
			this->button_OPEN->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_OPEN->Location = System::Drawing::Point(243, 289);
			this->button_OPEN->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_OPEN->Name = L"button_OPEN";
			this->button_OPEN->Size = System::Drawing::Size(67, 32);
			this->button_OPEN->TabIndex = 1;
			this->button_OPEN->Text = L"OPEN";
			this->button_OPEN->UseVisualStyleBackColor = false;
			this->button_OPEN->Click += gcnew System::EventHandler(this, &Form1::button_OPEN_Click);
			// 
			// richTextBox_filePath
			// 
			this->richTextBox_filePath->Enabled = false;
			this->richTextBox_filePath->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei UI", 7.8F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(136)));
			this->richTextBox_filePath->Location = System::Drawing::Point(316, 289);
			this->richTextBox_filePath->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->richTextBox_filePath->Name = L"richTextBox_filePath";
			this->richTextBox_filePath->Size = System::Drawing::Size(414, 32);
			this->richTextBox_filePath->TabIndex = 2;
			this->richTextBox_filePath->Text = L"";
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel1->BackColor = System::Drawing::Color::Gainsboro;
			this->tableLayoutPanel1->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Outset;
			this->tableLayoutPanel1->ColumnCount = 8;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				12.5F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				12.5F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				12.5F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				12.5F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				12.5F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				12.5F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				12.5F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				12.5F)));
			this->tableLayoutPanel1->Controls->Add(this->label_NZ, 7, 1);
			this->tableLayoutPanel1->Controls->Add(this->label_NY, 6, 1);
			this->tableLayoutPanel1->Controls->Add(this->label_NX, 5, 1);
			this->tableLayoutPanel1->Controls->Add(this->label_INZ, 4, 1);
			this->tableLayoutPanel1->Controls->Add(this->label_INY, 3, 1);
			this->tableLayoutPanel1->Controls->Add(this->label_INX, 2, 1);
			this->tableLayoutPanel1->Controls->Add(this->label_ERR, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->label_M30, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->label_PY, 6, 0);
			this->tableLayoutPanel1->Controls->Add(this->label_PX, 5, 0);
			this->tableLayoutPanel1->Controls->Add(this->label_HZ, 4, 0);
			this->tableLayoutPanel1->Controls->Add(this->label_HY, 3, 0);
			this->tableLayoutPanel1->Controls->Add(this->label_HX, 2, 0);
			this->tableLayoutPanel1->Controls->Add(this->label_M00, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->label_PZ, 7, 0);
			this->tableLayoutPanel1->Controls->Add(this->label_PGM, 1, 0);
			this->tableLayoutPanel1->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->tableLayoutPanel1->Location = System::Drawing::Point(48, 5);
			this->tableLayoutPanel1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tableLayoutPanel1->MaximumSize = System::Drawing::Size(682, 70);
			this->tableLayoutPanel1->MinimumSize = System::Drawing::Size(682, 70);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 2;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(682, 70);
			this->tableLayoutPanel1->TabIndex = 3;
			// 
			// label_NZ
			// 
			this->label_NZ->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_NZ->AutoSize = true;
			this->label_NZ->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_NZ->Location = System::Drawing::Point(600, 36);
			this->label_NZ->Name = L"label_NZ";
			this->label_NZ->Size = System::Drawing::Size(77, 32);
			this->label_NZ->TabIndex = 15;
			this->label_NZ->Text = L"NZ";
			this->label_NZ->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_NY
			// 
			this->label_NY->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_NY->AutoSize = true;
			this->label_NY->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_NY->Location = System::Drawing::Point(515, 36);
			this->label_NY->Name = L"label_NY";
			this->label_NY->Size = System::Drawing::Size(77, 32);
			this->label_NY->TabIndex = 14;
			this->label_NY->Text = L"NY";
			this->label_NY->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_NX
			// 
			this->label_NX->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_NX->AutoSize = true;
			this->label_NX->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_NX->Location = System::Drawing::Point(430, 36);
			this->label_NX->Name = L"label_NX";
			this->label_NX->Size = System::Drawing::Size(77, 32);
			this->label_NX->TabIndex = 13;
			this->label_NX->Text = L"NX";
			this->label_NX->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_INZ
			// 
			this->label_INZ->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_INZ->AutoSize = true;
			this->label_INZ->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_INZ->Location = System::Drawing::Point(345, 36);
			this->label_INZ->Name = L"label_INZ";
			this->label_INZ->Size = System::Drawing::Size(77, 32);
			this->label_INZ->TabIndex = 12;
			this->label_INZ->Text = L"INZ";
			this->label_INZ->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_INY
			// 
			this->label_INY->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_INY->AutoSize = true;
			this->label_INY->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_INY->Location = System::Drawing::Point(260, 36);
			this->label_INY->Name = L"label_INY";
			this->label_INY->Size = System::Drawing::Size(77, 32);
			this->label_INY->TabIndex = 11;
			this->label_INY->Text = L"INY";
			this->label_INY->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_INX
			// 
			this->label_INX->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_INX->AutoSize = true;
			this->label_INX->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_INX->Location = System::Drawing::Point(175, 36);
			this->label_INX->Name = L"label_INX";
			this->label_INX->Size = System::Drawing::Size(77, 32);
			this->label_INX->TabIndex = 10;
			this->label_INX->Text = L"INX";
			this->label_INX->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_ERR
			// 
			this->label_ERR->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_ERR->AutoSize = true;
			this->label_ERR->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_ERR->Location = System::Drawing::Point(90, 36);
			this->label_ERR->Name = L"label_ERR";
			this->label_ERR->Size = System::Drawing::Size(77, 32);
			this->label_ERR->TabIndex = 9;
			this->label_ERR->Text = L"ERR";
			this->label_ERR->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_M30
			// 
			this->label_M30->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_M30->AutoSize = true;
			this->label_M30->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_M30->Location = System::Drawing::Point(5, 36);
			this->label_M30->Name = L"label_M30";
			this->label_M30->Size = System::Drawing::Size(77, 32);
			this->label_M30->TabIndex = 8;
			this->label_M30->Text = L"M30";
			this->label_M30->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_PY
			// 
			this->label_PY->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_PY->AutoSize = true;
			this->label_PY->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_PY->Location = System::Drawing::Point(515, 2);
			this->label_PY->Name = L"label_PY";
			this->label_PY->Size = System::Drawing::Size(77, 32);
			this->label_PY->TabIndex = 6;
			this->label_PY->Text = L"PY";
			this->label_PY->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_PX
			// 
			this->label_PX->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_PX->AutoSize = true;
			this->label_PX->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_PX->Location = System::Drawing::Point(430, 2);
			this->label_PX->Name = L"label_PX";
			this->label_PX->Size = System::Drawing::Size(77, 32);
			this->label_PX->TabIndex = 5;
			this->label_PX->Text = L"PX";
			this->label_PX->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_HZ
			// 
			this->label_HZ->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_HZ->AutoSize = true;
			this->label_HZ->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_HZ->Location = System::Drawing::Point(345, 2);
			this->label_HZ->Name = L"label_HZ";
			this->label_HZ->Size = System::Drawing::Size(77, 32);
			this->label_HZ->TabIndex = 4;
			this->label_HZ->Text = L"HZ";
			this->label_HZ->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_HY
			// 
			this->label_HY->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_HY->AutoSize = true;
			this->label_HY->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_HY->Location = System::Drawing::Point(260, 2);
			this->label_HY->Name = L"label_HY";
			this->label_HY->Size = System::Drawing::Size(77, 32);
			this->label_HY->TabIndex = 3;
			this->label_HY->Text = L"HY";
			this->label_HY->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_HX
			// 
			this->label_HX->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_HX->AutoSize = true;
			this->label_HX->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_HX->Location = System::Drawing::Point(175, 2);
			this->label_HX->Name = L"label_HX";
			this->label_HX->Size = System::Drawing::Size(77, 32);
			this->label_HX->TabIndex = 2;
			this->label_HX->Text = L"HX";
			this->label_HX->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_M00
			// 
			this->label_M00->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_M00->AutoSize = true;
			this->label_M00->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_M00->Location = System::Drawing::Point(5, 2);
			this->label_M00->Name = L"label_M00";
			this->label_M00->Size = System::Drawing::Size(77, 32);
			this->label_M00->TabIndex = 0;
			this->label_M00->Text = L"M00";
			this->label_M00->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_PZ
			// 
			this->label_PZ->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_PZ->AutoSize = true;
			this->label_PZ->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_PZ->Location = System::Drawing::Point(600, 2);
			this->label_PZ->Name = L"label_PZ";
			this->label_PZ->Size = System::Drawing::Size(77, 32);
			this->label_PZ->TabIndex = 7;
			this->label_PZ->Text = L"PZ";
			this->label_PZ->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_PGM
			// 
			this->label_PGM->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_PGM->AutoSize = true;
			this->label_PGM->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_PGM->Location = System::Drawing::Point(90, 2);
			this->label_PGM->Name = L"label_PGM";
			this->label_PGM->Size = System::Drawing::Size(77, 32);
			this->label_PGM->TabIndex = 1;
			this->label_PGM->Text = L"PGM";
			this->label_PGM->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// RT_timer
			// 
			this->RT_timer->Enabled = true;
			this->RT_timer->Interval = 50;
			this->RT_timer->Tick += gcnew System::EventHandler(this, &Form1::RT_timer_Tick);
			// 
			// NRT_timer
			// 
			this->NRT_timer->Enabled = true;
			this->NRT_timer->Tick += gcnew System::EventHandler(this, &Form1::NRT_timer_Tick);
			// 
			// openFileDialog_openFile
			// 
			this->openFileDialog_openFile->FileName = L"openFileDialog_openFile";
			// 
			// tableLayoutPanel2
			// 
			this->tableLayoutPanel2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel2->BackColor = System::Drawing::Color::Gainsboro;
			this->tableLayoutPanel2->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Outset;
			this->tableLayoutPanel2->ColumnCount = 1;
			this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				50)));
			this->tableLayoutPanel2->Controls->Add(this->label_hour, 0, 1);
			this->tableLayoutPanel2->Controls->Add(this->label_year, 0, 0);
			this->tableLayoutPanel2->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->tableLayoutPanel2->Location = System::Drawing::Point(736, 5);
			this->tableLayoutPanel2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tableLayoutPanel2->MaximumSize = System::Drawing::Size(100, 70);
			this->tableLayoutPanel2->MinimumSize = System::Drawing::Size(100, 70);
			this->tableLayoutPanel2->Name = L"tableLayoutPanel2";
			this->tableLayoutPanel2->RowCount = 2;
			this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel2->Size = System::Drawing::Size(100, 70);
			this->tableLayoutPanel2->TabIndex = 4;
			// 
			// label_hour
			// 
			this->label_hour->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_hour->AutoSize = true;
			this->label_hour->Font = (gcnew System::Drawing::Font(L"Calibri", 10));
			this->label_hour->Location = System::Drawing::Point(5, 36);
			this->label_hour->Name = L"label_hour";
			this->label_hour->Size = System::Drawing::Size(90, 32);
			this->label_hour->TabIndex = 1;
			this->label_hour->Text = L"timer";
			this->label_hour->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_year
			// 
			this->label_year->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_year->AutoSize = true;
			this->label_year->Font = (gcnew System::Drawing::Font(L"Calibri", 10));
			this->label_year->Location = System::Drawing::Point(5, 2);
			this->label_year->Name = L"label_year";
			this->label_year->Size = System::Drawing::Size(90, 32);
			this->label_year->TabIndex = 0;
			this->label_year->Text = L"RESET";
			this->label_year->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// button_X1
			// 
			this->button_X1->BackColor = System::Drawing::Color::Gainsboro;
			this->button_X1->Font = (gcnew System::Drawing::Font(L"Calibri", 10.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_X1->Location = System::Drawing::Point(339, 325);
			this->button_X1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_X1->Name = L"button_X1";
			this->button_X1->Size = System::Drawing::Size(60, 50);
			this->button_X1->TabIndex = 5;
			this->button_X1->Text = L"X1";
			this->button_X1->UseVisualStyleBackColor = false;
			this->button_X1->Click += gcnew System::EventHandler(this, &Form1::button_X1_Click);
			// 
			// button_X10
			// 
			this->button_X10->BackColor = System::Drawing::Color::Gainsboro;
			this->button_X10->Font = (gcnew System::Drawing::Font(L"Calibri", 10.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_X10->Location = System::Drawing::Point(339, 379);
			this->button_X10->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_X10->Name = L"button_X10";
			this->button_X10->Size = System::Drawing::Size(60, 50);
			this->button_X10->TabIndex = 6;
			this->button_X10->Text = L"X10";
			this->button_X10->UseVisualStyleBackColor = false;
			this->button_X10->Click += gcnew System::EventHandler(this, &Form1::button_X10_Click);
			// 
			// button_X100
			// 
			this->button_X100->BackColor = System::Drawing::Color::Gainsboro;
			this->button_X100->Font = (gcnew System::Drawing::Font(L"Calibri", 10.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_X100->Location = System::Drawing::Point(339, 433);
			this->button_X100->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_X100->Name = L"button_X100";
			this->button_X100->Size = System::Drawing::Size(60, 50);
			this->button_X100->TabIndex = 7;
			this->button_X100->Text = L"X100";
			this->button_X100->UseVisualStyleBackColor = false;
			this->button_X100->Click += gcnew System::EventHandler(this, &Form1::button_X100_Click);
			// 
			// button_TEACHSTART
			// 
			this->button_TEACHSTART->BackColor = System::Drawing::Color::Gainsboro;
			this->button_TEACHSTART->Enabled = false;
			this->button_TEACHSTART->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_TEACHSTART->Location = System::Drawing::Point(405, 325);
			this->button_TEACHSTART->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_TEACHSTART->Name = L"button_TEACHSTART";
			this->button_TEACHSTART->Size = System::Drawing::Size(79, 50);
			this->button_TEACHSTART->TabIndex = 8;
			this->button_TEACHSTART->Text = L"TEACH START";
			this->button_TEACHSTART->UseVisualStyleBackColor = false;
			// 
			// button_TEACHEND
			// 
			this->button_TEACHEND->BackColor = System::Drawing::Color::Gainsboro;
			this->button_TEACHEND->Enabled = false;
			this->button_TEACHEND->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_TEACHEND->Location = System::Drawing::Point(405, 433);
			this->button_TEACHEND->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_TEACHEND->Name = L"button_TEACHEND";
			this->button_TEACHEND->Size = System::Drawing::Size(79, 50);
			this->button_TEACHEND->TabIndex = 10;
			this->button_TEACHEND->Text = L"TEACH  END";
			this->button_TEACHEND->UseVisualStyleBackColor = false;
			// 
			// button_SB
			// 
			this->button_SB->BackColor = System::Drawing::Color::Gainsboro;
			this->button_SB->Font = (gcnew System::Drawing::Font(L"Calibri", 10.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_SB->Location = System::Drawing::Point(490, 379);
			this->button_SB->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_SB->Name = L"button_SB";
			this->button_SB->Size = System::Drawing::Size(60, 50);
			this->button_SB->TabIndex = 13;
			this->button_SB->Text = L"SB";
			this->button_SB->UseVisualStyleBackColor = false;
			this->button_SB->Click += gcnew System::EventHandler(this, &Form1::button_SB_Click);
			// 
			// button_FH
			// 
			this->button_FH->BackColor = System::Drawing::Color::Gainsboro;
			this->button_FH->Font = (gcnew System::Drawing::Font(L"Calibri", 10.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_FH->Location = System::Drawing::Point(490, 433);
			this->button_FH->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_FH->Name = L"button_FH";
			this->button_FH->Size = System::Drawing::Size(60, 50);
			this->button_FH->TabIndex = 12;
			this->button_FH->Text = L"FH";
			this->button_FH->UseVisualStyleBackColor = false;
			this->button_FH->Click += gcnew System::EventHandler(this, &Form1::button_FH_Click);
			// 
			// button_M01
			// 
			this->button_M01->BackColor = System::Drawing::Color::Gainsboro;
			this->button_M01->Font = (gcnew System::Drawing::Font(L"Calibri", 10.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_M01->Location = System::Drawing::Point(490, 325);
			this->button_M01->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_M01->Name = L"button_M01";
			this->button_M01->Size = System::Drawing::Size(60, 50);
			this->button_M01->TabIndex = 11;
			this->button_M01->Text = L"M01";
			this->button_M01->UseVisualStyleBackColor = false;
			this->button_M01->Click += gcnew System::EventHandler(this, &Form1::button_M01_Click);
			// 
			// trackBar_feedOverride
			// 
			this->trackBar_feedOverride->AutoSize = false;
			this->trackBar_feedOverride->Location = System::Drawing::Point(549, 345);
			this->trackBar_feedOverride->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->trackBar_feedOverride->Maximum = 200;
			this->trackBar_feedOverride->Name = L"trackBar_feedOverride";
			this->trackBar_feedOverride->Size = System::Drawing::Size(189, 42);
			this->trackBar_feedOverride->TabIndex = 14;
			this->trackBar_feedOverride->TickFrequency = 10;
			this->trackBar_feedOverride->Value = 100;
			this->trackBar_feedOverride->Scroll += gcnew System::EventHandler(this, &Form1::trackBar_feedOverride_Scroll);
			this->trackBar_feedOverride->ValueChanged += gcnew System::EventHandler(this, &Form1::trackBar_feedOverride_ValueChanged);
			// 
			// button_START
			// 
			this->button_START->BackColor = System::Drawing::Color::Lime;
			this->button_START->Font = (gcnew System::Drawing::Font(L"Calibri", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_START->ForeColor = System::Drawing::SystemColors::ControlText;
			this->button_START->Location = System::Drawing::Point(556, 383);
			this->button_START->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_START->Name = L"button_START";
			this->button_START->Size = System::Drawing::Size(94, 57);
			this->button_START->TabIndex = 15;
			this->button_START->Text = L"START";
			this->button_START->UseVisualStyleBackColor = false;
			this->button_START->Click += gcnew System::EventHandler(this, &Form1::button_START_Click);
			// 
			// button_RESET
			// 
			this->button_RESET->BackColor = System::Drawing::Color::Yellow;
			this->button_RESET->Font = (gcnew System::Drawing::Font(L"Calibri", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_RESET->Location = System::Drawing::Point(652, 383);
			this->button_RESET->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_RESET->Name = L"button_RESET";
			this->button_RESET->Size = System::Drawing::Size(78, 57);
			this->button_RESET->TabIndex = 16;
			this->button_RESET->Text = L"RESET";
			this->button_RESET->UseVisualStyleBackColor = false;
			this->button_RESET->Click += gcnew System::EventHandler(this, &Form1::button_RESET_Click);
			// 
			// button_EMERGENCYSTOP
			// 
			this->button_EMERGENCYSTOP->BackColor = System::Drawing::Color::Red;
			this->button_EMERGENCYSTOP->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_EMERGENCYSTOP->Location = System::Drawing::Point(600, 487);
			this->button_EMERGENCYSTOP->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_EMERGENCYSTOP->Name = L"button_EMERGENCYSTOP";
			this->button_EMERGENCYSTOP->Size = System::Drawing::Size(130, 61);
			this->button_EMERGENCYSTOP->TabIndex = 17;
			this->button_EMERGENCYSTOP->Text = L"EMERGENCY STOP";
			this->button_EMERGENCYSTOP->UseVisualStyleBackColor = false;
			this->button_EMERGENCYSTOP->Click += gcnew System::EventHandler(this, &Form1::button_EMERGENCYSTOP_Click);
			// 
			// groupBox_XYZcoordinate
			// 
			this->groupBox_XYZcoordinate->Controls->Add(this->label_Z);
			this->groupBox_XYZcoordinate->Controls->Add(this->label_X);
			this->groupBox_XYZcoordinate->Controls->Add(this->label_Y);
			this->groupBox_XYZcoordinate->Controls->Add(this->label_coordZ);
			this->groupBox_XYZcoordinate->Controls->Add(this->label_coordY);
			this->groupBox_XYZcoordinate->Controls->Add(this->label_coordX);
			this->groupBox_XYZcoordinate->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->groupBox_XYZcoordinate->Location = System::Drawing::Point(6, 79);
			this->groupBox_XYZcoordinate->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox_XYZcoordinate->Name = L"groupBox_XYZcoordinate";
			this->groupBox_XYZcoordinate->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox_XYZcoordinate->Size = System::Drawing::Size(231, 191);
			this->groupBox_XYZcoordinate->TabIndex = 18;
			this->groupBox_XYZcoordinate->TabStop = false;
			this->groupBox_XYZcoordinate->Text = L"XYZ Coordinate";
			// 
			// label_Z
			// 
			this->label_Z->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_Z->BackColor = System::Drawing::Color::Turquoise;
			this->label_Z->Font = (gcnew System::Drawing::Font(L"Consolas", 24, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_Z->Location = System::Drawing::Point(1, 130);
			this->label_Z->MaximumSize = System::Drawing::Size(50, 50);
			this->label_Z->MinimumSize = System::Drawing::Size(50, 50);
			this->label_Z->Name = L"label_Z";
			this->label_Z->Size = System::Drawing::Size(50, 50);
			this->label_Z->TabIndex = 50;
			this->label_Z->Text = L"Z";
			this->label_Z->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_X
			// 
			this->label_X->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_X->BackColor = System::Drawing::Color::Turquoise;
			this->label_X->Font = (gcnew System::Drawing::Font(L"Consolas", 24, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_X->Location = System::Drawing::Point(1, 21);
			this->label_X->MaximumSize = System::Drawing::Size(50, 50);
			this->label_X->MinimumSize = System::Drawing::Size(50, 50);
			this->label_X->Name = L"label_X";
			this->label_X->Size = System::Drawing::Size(50, 50);
			this->label_X->TabIndex = 48;
			this->label_X->Text = L"X";
			this->label_X->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_Y
			// 
			this->label_Y->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_Y->BackColor = System::Drawing::Color::Turquoise;
			this->label_Y->Font = (gcnew System::Drawing::Font(L"Consolas", 24, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_Y->Location = System::Drawing::Point(1, 76);
			this->label_Y->MaximumSize = System::Drawing::Size(50, 50);
			this->label_Y->MinimumSize = System::Drawing::Size(50, 50);
			this->label_Y->Name = L"label_Y";
			this->label_Y->Size = System::Drawing::Size(50, 50);
			this->label_Y->TabIndex = 51;
			this->label_Y->Text = L"Y";
			this->label_Y->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_coordZ
			// 
			this->label_coordZ->BackColor = System::Drawing::Color::Gainsboro;
			this->label_coordZ->Font = (gcnew System::Drawing::Font(L"Calibri", 24, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_coordZ->Location = System::Drawing::Point(49, 130);
			this->label_coordZ->MaximumSize = System::Drawing::Size(182, 50);
			this->label_coordZ->MinimumSize = System::Drawing::Size(182, 50);
			this->label_coordZ->Name = L"label_coordZ";
			this->label_coordZ->Size = System::Drawing::Size(182, 50);
			this->label_coordZ->TabIndex = 50;
			this->label_coordZ->Text = L"100.000";
			this->label_coordZ->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label_coordY
			// 
			this->label_coordY->BackColor = System::Drawing::Color::Gainsboro;
			this->label_coordY->Font = (gcnew System::Drawing::Font(L"Calibri", 24, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_coordY->Location = System::Drawing::Point(49, 76);
			this->label_coordY->MaximumSize = System::Drawing::Size(182, 50);
			this->label_coordY->MinimumSize = System::Drawing::Size(182, 50);
			this->label_coordY->Name = L"label_coordY";
			this->label_coordY->Size = System::Drawing::Size(182, 50);
			this->label_coordY->TabIndex = 49;
			this->label_coordY->Text = L"075.000";
			this->label_coordY->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label_coordX
			// 
			this->label_coordX->BackColor = System::Drawing::Color::Gainsboro;
			this->label_coordX->Font = (gcnew System::Drawing::Font(L"Calibri", 24, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_coordX->Location = System::Drawing::Point(49, 21);
			this->label_coordX->MaximumSize = System::Drawing::Size(182, 50);
			this->label_coordX->MinimumSize = System::Drawing::Size(182, 50);
			this->label_coordX->Name = L"label_coordX";
			this->label_coordX->Size = System::Drawing::Size(182, 50);
			this->label_coordX->TabIndex = 48;
			this->label_coordX->Text = L"075.000";
			this->label_coordX->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// button_Xplus
			// 
			this->button_Xplus->BackColor = System::Drawing::Color::Gainsboro;
			this->button_Xplus->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_Xplus->Location = System::Drawing::Point(6, 7);
			this->button_Xplus->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Xplus->Name = L"button_Xplus";
			this->button_Xplus->Size = System::Drawing::Size(57, 36);
			this->button_Xplus->TabIndex = 25;
			this->button_Xplus->Text = L"X+";
			this->button_Xplus->UseVisualStyleBackColor = false;
			this->button_Xplus->Click += gcnew System::EventHandler(this, &Form1::button_Xplus_Click);
			this->button_Xplus->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Xplus_MouseDown);
			this->button_Xplus->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Xplus_MouseUp);
			// 
			// button_Yplus
			// 
			this->button_Yplus->BackColor = System::Drawing::Color::Gainsboro;
			this->button_Yplus->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_Yplus->Location = System::Drawing::Point(6, 47);
			this->button_Yplus->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Yplus->Name = L"button_Yplus";
			this->button_Yplus->Size = System::Drawing::Size(57, 36);
			this->button_Yplus->TabIndex = 26;
			this->button_Yplus->Text = L"Y+";
			this->button_Yplus->UseVisualStyleBackColor = false;
			this->button_Yplus->Click += gcnew System::EventHandler(this, &Form1::button_Yplus_Click);
			this->button_Yplus->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Yplus_MouseDown);
			this->button_Yplus->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Yplus_MouseUp);
			// 
			// button_Zplus
			// 
			this->button_Zplus->BackColor = System::Drawing::Color::Gainsboro;
			this->button_Zplus->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_Zplus->Location = System::Drawing::Point(6, 87);
			this->button_Zplus->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Zplus->Name = L"button_Zplus";
			this->button_Zplus->Size = System::Drawing::Size(57, 36);
			this->button_Zplus->TabIndex = 27;
			this->button_Zplus->Text = L"Z+";
			this->button_Zplus->UseVisualStyleBackColor = false;
			this->button_Zplus->Click += gcnew System::EventHandler(this, &Form1::button_Zplus_Click);
			this->button_Zplus->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Zplus_MouseDown);
			this->button_Zplus->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Zplus_MouseUp);
			// 
			// button_Xminus
			// 
			this->button_Xminus->BackColor = System::Drawing::Color::Gainsboro;
			this->button_Xminus->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_Xminus->Location = System::Drawing::Point(67, 7);
			this->button_Xminus->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Xminus->Name = L"button_Xminus";
			this->button_Xminus->Size = System::Drawing::Size(57, 36);
			this->button_Xminus->TabIndex = 28;
			this->button_Xminus->Text = L"X-";
			this->button_Xminus->UseVisualStyleBackColor = false;
			this->button_Xminus->Click += gcnew System::EventHandler(this, &Form1::button_Xminus_Click);
			this->button_Xminus->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Xminus_MouseDown);
			this->button_Xminus->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Xminus_MouseUp);
			// 
			// button_Yminus
			// 
			this->button_Yminus->BackColor = System::Drawing::Color::Gainsboro;
			this->button_Yminus->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_Yminus->Location = System::Drawing::Point(67, 47);
			this->button_Yminus->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Yminus->Name = L"button_Yminus";
			this->button_Yminus->Size = System::Drawing::Size(57, 36);
			this->button_Yminus->TabIndex = 29;
			this->button_Yminus->Text = L"Y-";
			this->button_Yminus->UseVisualStyleBackColor = false;
			this->button_Yminus->Click += gcnew System::EventHandler(this, &Form1::button_Yminus_Click);
			this->button_Yminus->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Yminus_MouseDown);
			this->button_Yminus->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Yminus_MouseUp);
			// 
			// button_Zminus
			// 
			this->button_Zminus->BackColor = System::Drawing::Color::Gainsboro;
			this->button_Zminus->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_Zminus->Location = System::Drawing::Point(67, 87);
			this->button_Zminus->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Zminus->Name = L"button_Zminus";
			this->button_Zminus->Size = System::Drawing::Size(57, 36);
			this->button_Zminus->TabIndex = 30;
			this->button_Zminus->Text = L"Z-";
			this->button_Zminus->UseVisualStyleBackColor = false;
			this->button_Zminus->Click += gcnew System::EventHandler(this, &Form1::button_Zminus_Click);
			this->button_Zminus->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Zminus_MouseDown);
			this->button_Zminus->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Zminus_MouseUp);
			// 
			// button_RECORD
			// 
			this->button_RECORD->BackColor = System::Drawing::Color::Gainsboro;
			this->button_RECORD->Enabled = false;
			this->button_RECORD->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_RECORD->Location = System::Drawing::Point(405, 379);
			this->button_RECORD->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_RECORD->Name = L"button_RECORD";
			this->button_RECORD->Size = System::Drawing::Size(79, 50);
			this->button_RECORD->TabIndex = 31;
			this->button_RECORD->Text = L"RECORD";
			this->button_RECORD->UseVisualStyleBackColor = false;
			// 
			// button_MEM
			// 
			this->button_MEM->BackColor = System::Drawing::Color::Violet;
			this->button_MEM->Font = (gcnew System::Drawing::Font(L"Verdana", 10));
			this->button_MEM->ForeColor = System::Drawing::Color::Black;
			this->button_MEM->Location = System::Drawing::Point(6, 487);
			this->button_MEM->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_MEM->Name = L"button_MEM";
			this->button_MEM->Size = System::Drawing::Size(93, 61);
			this->button_MEM->TabIndex = 32;
			this->button_MEM->Text = L"MEM";
			this->button_MEM->UseVisualStyleBackColor = false;
			this->button_MEM->Click += gcnew System::EventHandler(this, &Form1::button_MEM_Click);
			// 
			// button_MDI
			// 
			this->button_MDI->BackColor = System::Drawing::Color::DeepSkyBlue;
			this->button_MDI->Font = (gcnew System::Drawing::Font(L"Verdana", 10));
			this->button_MDI->ForeColor = System::Drawing::Color::Black;
			this->button_MDI->Location = System::Drawing::Point(105, 487);
			this->button_MDI->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_MDI->Name = L"button_MDI";
			this->button_MDI->Size = System::Drawing::Size(93, 61);
			this->button_MDI->TabIndex = 34;
			this->button_MDI->Text = L"MDI";
			this->button_MDI->UseVisualStyleBackColor = false;
			this->button_MDI->Click += gcnew System::EventHandler(this, &Form1::button_MDI_Click);
			// 
			// button_JOG
			// 
			this->button_JOG->BackColor = System::Drawing::Color::DeepSkyBlue;
			this->button_JOG->Font = (gcnew System::Drawing::Font(L"Verdana", 10));
			this->button_JOG->ForeColor = System::Drawing::Color::Black;
			this->button_JOG->Location = System::Drawing::Point(204, 487);
			this->button_JOG->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_JOG->Name = L"button_JOG";
			this->button_JOG->Size = System::Drawing::Size(93, 61);
			this->button_JOG->TabIndex = 36;
			this->button_JOG->Text = L"JOG";
			this->button_JOG->UseVisualStyleBackColor = false;
			this->button_JOG->Click += gcnew System::EventHandler(this, &Form1::button_JOG_Click);
			// 
			// button_WHEEL
			// 
			this->button_WHEEL->BackColor = System::Drawing::Color::DeepSkyBlue;
			this->button_WHEEL->Font = (gcnew System::Drawing::Font(L"Verdana", 10));
			this->button_WHEEL->ForeColor = System::Drawing::Color::Black;
			this->button_WHEEL->Location = System::Drawing::Point(303, 487);
			this->button_WHEEL->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_WHEEL->Name = L"button_WHEEL";
			this->button_WHEEL->Size = System::Drawing::Size(93, 61);
			this->button_WHEEL->TabIndex = 38;
			this->button_WHEEL->Text = L"WHEEL";
			this->button_WHEEL->UseVisualStyleBackColor = false;
			this->button_WHEEL->Click += gcnew System::EventHandler(this, &Form1::button_WHEEL_Click);
			// 
			// button_TEACH
			// 
			this->button_TEACH->BackColor = System::Drawing::Color::DeepSkyBlue;
			this->button_TEACH->Enabled = false;
			this->button_TEACH->Font = (gcnew System::Drawing::Font(L"Verdana", 10));
			this->button_TEACH->ForeColor = System::Drawing::Color::Black;
			this->button_TEACH->Location = System::Drawing::Point(402, 487);
			this->button_TEACH->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_TEACH->Name = L"button_TEACH";
			this->button_TEACH->Size = System::Drawing::Size(93, 61);
			this->button_TEACH->TabIndex = 40;
			this->button_TEACH->Text = L"TEACH";
			this->button_TEACH->UseVisualStyleBackColor = false;
			this->button_TEACH->Click += gcnew System::EventHandler(this, &Form1::button_TEACH_Click);
			// 
			// button_HOME
			// 
			this->button_HOME->BackColor = System::Drawing::Color::DeepSkyBlue;
			this->button_HOME->Font = (gcnew System::Drawing::Font(L"Verdana", 10));
			this->button_HOME->ForeColor = System::Drawing::Color::Black;
			this->button_HOME->Location = System::Drawing::Point(501, 487);
			this->button_HOME->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_HOME->Name = L"button_HOME";
			this->button_HOME->Size = System::Drawing::Size(93, 61);
			this->button_HOME->TabIndex = 41;
			this->button_HOME->Text = L"HOME";
			this->button_HOME->UseVisualStyleBackColor = false;
			this->button_HOME->Click += gcnew System::EventHandler(this, &Form1::button_HOME_Click);
			// 
			// button_diagnosis
			// 
			this->button_diagnosis->BackColor = System::Drawing::Color::Wheat;
			this->button_diagnosis->Enabled = false;
			this->button_diagnosis->Font = (gcnew System::Drawing::Font(L"Calibri", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_diagnosis->Location = System::Drawing::Point(741, 164);
			this->button_diagnosis->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_diagnosis->Name = L"button_diagnosis";
			this->button_diagnosis->Size = System::Drawing::Size(90, 36);
			this->button_diagnosis->TabIndex = 42;
			this->button_diagnosis->Text = L"Diagnose";
			this->button_diagnosis->UseVisualStyleBackColor = false;
			// 
			// button_simulation
			// 
			this->button_simulation->BackColor = System::Drawing::Color::Wheat;
			this->button_simulation->Enabled = false;
			this->button_simulation->Font = (gcnew System::Drawing::Font(L"Calibri", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_simulation->Location = System::Drawing::Point(741, 84);
			this->button_simulation->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_simulation->Name = L"button_simulation";
			this->button_simulation->Size = System::Drawing::Size(90, 36);
			this->button_simulation->TabIndex = 43;
			this->button_simulation->Text = L"Simulation";
			this->button_simulation->UseVisualStyleBackColor = false;
			// 
			// button_parameter
			// 
			this->button_parameter->BackColor = System::Drawing::Color::Wheat;
			this->button_parameter->Enabled = false;
			this->button_parameter->Font = (gcnew System::Drawing::Font(L"Calibri", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_parameter->Location = System::Drawing::Point(741, 124);
			this->button_parameter->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_parameter->Name = L"button_parameter";
			this->button_parameter->Size = System::Drawing::Size(90, 36);
			this->button_parameter->TabIndex = 44;
			this->button_parameter->Text = L"Parameter";
			this->button_parameter->UseVisualStyleBackColor = false;
			this->button_parameter->Click += gcnew System::EventHandler(this, &Form1::button_parameter_Click);
			// 
			// label_FO
			// 
			this->label_FO->AutoSize = true;
			this->label_FO->Font = (gcnew System::Drawing::Font(L"Calibri", 10));
			this->label_FO->Location = System::Drawing::Point(563, 325);
			this->label_FO->Name = L"label_FO";
			this->label_FO->Size = System::Drawing::Size(107, 21);
			this->label_FO->TabIndex = 45;
			this->label_FO->Text = L"Feed Override";
			// 
			// label_feedOverride
			// 
			this->label_feedOverride->AutoSize = true;
			this->label_feedOverride->Font = (gcnew System::Drawing::Font(L"Calibri", 10));
			this->label_feedOverride->Location = System::Drawing::Point(676, 326);
			this->label_feedOverride->Name = L"label_feedOverride";
			this->label_feedOverride->Size = System::Drawing::Size(49, 21);
			this->label_feedOverride->TabIndex = 47;
			this->label_feedOverride->Text = L"100%";
			// 
			// chart1
			// 
			this->chart1->BorderlineColor = System::Drawing::Color::Black;
			chartArea1->AxisX->Enabled = System::Windows::Forms::DataVisualization::Charting::AxisEnabled::False;
			chartArea1->AxisX->Maximum = 500;
			chartArea1->AxisY->Enabled = System::Windows::Forms::DataVisualization::Charting::AxisEnabled::True;
			chartArea1->AxisY->Interval = 10;
			chartArea1->AxisY->Maximum = 50;
			chartArea1->AxisY->Minimum = 0;
			chartArea1->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea1);
			this->chart1->ImeMode = System::Windows::Forms::ImeMode::NoControl;
			legend1->Enabled = false;
			legend1->Name = L"Legend1";
			this->chart1->Legends->Add(legend1);
			this->chart1->Location = System::Drawing::Point(736, 325);
			this->chart1->Name = L"chart1";
			series1->BorderWidth = 3;
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::StepLine;
			series1->Color = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)), 
				static_cast<System::Int32>(static_cast<System::Byte>(192)));
			series1->Legend = L"Legend1";
			series1->MarkerBorderWidth = 3;
			series1->MarkerSize = 3;
			series1->Name = L"momtVb";
			this->chart1->Series->Add(series1);
			this->chart1->Size = System::Drawing::Size(428, 223);
			this->chart1->TabIndex = 48;
			this->chart1->Text = L"chart1";
			// 
			// chart_linkage
			// 
			this->chart_linkage->BorderlineColor = System::Drawing::Color::Black;
			chartArea2->AxisX->Enabled = System::Windows::Forms::DataVisualization::Charting::AxisEnabled::True;
			chartArea2->AxisX->Interval = 50;
			chartArea2->AxisX->LabelAutoFitMaxFontSize = 6;
			chartArea2->AxisX->Maximum = 150;
			chartArea2->AxisX->Minimum = -150;
			chartArea2->AxisY->Enabled = System::Windows::Forms::DataVisualization::Charting::AxisEnabled::True;
			chartArea2->AxisY->Interval = 50;
			chartArea2->AxisY->LabelAutoFitMaxFontSize = 6;
			chartArea2->AxisY->Maximum = 150;
			chartArea2->AxisY->MaximumAutoSize = 50;
			chartArea2->AxisY->Minimum = -150;
			chartArea2->Name = L"ChartArea1";
			this->chart_linkage->ChartAreas->Add(chartArea2);
			legend2->Enabled = false;
			legend2->Name = L"Legend1";
			this->chart_linkage->Legends->Add(legend2);
			this->chart_linkage->Location = System::Drawing::Point(842, 5);
			this->chart_linkage->Name = L"chart_linkage";
			series2->BorderWidth = 4;
			series2->ChartArea = L"ChartArea1";
			series2->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series2->Legend = L"Legend1";
			series2->MarkerSize = 3;
			series2->Name = L"Series1";
			series3->BorderWidth = 4;
			series3->ChartArea = L"ChartArea1";
			series3->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series3->Legend = L"Legend1";
			series3->MarkerSize = 10;
			series3->Name = L"Series2";
			series4->ChartArea = L"ChartArea1";
			series4->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Point;
			series4->Legend = L"Legend1";
			series4->MarkerSize = 1;
			series4->MarkerStyle = System::Windows::Forms::DataVisualization::Charting::MarkerStyle::Circle;
			series4->Name = L"Series3";
			series5->ChartArea = L"ChartArea1";
			series5->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Point;
			series5->Legend = L"Legend1";
			series5->MarkerColor = System::Drawing::Color::LightSeaGreen;
			series5->MarkerSize = 3;
			series5->Name = L"Series4";
			this->chart_linkage->Series->Add(series2);
			this->chart_linkage->Series->Add(series3);
			this->chart_linkage->Series->Add(series4);
			this->chart_linkage->Series->Add(series5);
			this->chart_linkage->Size = System::Drawing::Size(322, 315);
			this->chart_linkage->TabIndex = 49;
			this->chart_linkage->Text = L"chart_linkage";
			// 
			// tableLayoutPanel3
			// 
			this->tableLayoutPanel3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel3->BackColor = System::Drawing::Color::Gainsboro;
			this->tableLayoutPanel3->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Single;
			this->tableLayoutPanel3->ColumnCount = 1;
			this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				100)));
			this->tableLayoutPanel3->Controls->Add(this->label_vIncre, 0, 1);
			this->tableLayoutPanel3->Controls->Add(this->label_wIncre, 0, 2);
			this->tableLayoutPanel3->Controls->Add(this->label_uIncre, 0, 0);
			this->tableLayoutPanel3->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->tableLayoutPanel3->Location = System::Drawing::Point(741, 229);
			this->tableLayoutPanel3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tableLayoutPanel3->MaximumSize = System::Drawing::Size(90, 91);
			this->tableLayoutPanel3->MinimumSize = System::Drawing::Size(90, 91);
			this->tableLayoutPanel3->Name = L"tableLayoutPanel3";
			this->tableLayoutPanel3->RowCount = 3;
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33333F)));
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33333F)));
			this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33333F)));
			this->tableLayoutPanel3->Size = System::Drawing::Size(90, 91);
			this->tableLayoutPanel3->TabIndex = 50;
			// 
			// label_vIncre
			// 
			this->label_vIncre->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_vIncre->AutoSize = true;
			this->label_vIncre->BackColor = System::Drawing::Color::Gainsboro;
			this->label_vIncre->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_vIncre->Location = System::Drawing::Point(4, 31);
			this->label_vIncre->Name = L"label_vIncre";
			this->label_vIncre->Size = System::Drawing::Size(82, 29);
			this->label_vIncre->TabIndex = 5;
			this->label_vIncre->Text = L"Incr_V";
			this->label_vIncre->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_wIncre
			// 
			this->label_wIncre->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_wIncre->AutoSize = true;
			this->label_wIncre->BackColor = System::Drawing::Color::Gainsboro;
			this->label_wIncre->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_wIncre->Location = System::Drawing::Point(4, 61);
			this->label_wIncre->Name = L"label_wIncre";
			this->label_wIncre->Size = System::Drawing::Size(82, 29);
			this->label_wIncre->TabIndex = 3;
			this->label_wIncre->Text = L"Incr_W";
			this->label_wIncre->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_uIncre
			// 
			this->label_uIncre->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_uIncre->AutoSize = true;
			this->label_uIncre->BackColor = System::Drawing::Color::Gainsboro;
			this->label_uIncre->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_uIncre->Location = System::Drawing::Point(4, 1);
			this->label_uIncre->Name = L"label_uIncre";
			this->label_uIncre->Size = System::Drawing::Size(82, 29);
			this->label_uIncre->TabIndex = 4;
			this->label_uIncre->Text = L"Incr_U";
			this->label_uIncre->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_Increment
			// 
			this->label_Increment->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_Increment->BackColor = System::Drawing::Color::PaleTurquoise;
			this->label_Increment->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_Increment->Location = System::Drawing::Point(741, 208);
			this->label_Increment->MaximumSize = System::Drawing::Size(90, 21);
			this->label_Increment->MinimumSize = System::Drawing::Size(90, 21);
			this->label_Increment->Name = L"label_Increment";
			this->label_Increment->Size = System::Drawing::Size(90, 21);
			this->label_Increment->TabIndex = 0;
			this->label_Increment->Text = L"Increment";
			this->label_Increment->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_decLineNumber
			// 
			this->label_decLineNumber->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_decLineNumber->BackColor = System::Drawing::Color::Gainsboro;
			this->label_decLineNumber->Font = (gcnew System::Drawing::Font(L"Calibri", 10));
			this->label_decLineNumber->Location = System::Drawing::Point(338, 79);
			this->label_decLineNumber->MaximumSize = System::Drawing::Size(59, 29);
			this->label_decLineNumber->MinimumSize = System::Drawing::Size(59, 29);
			this->label_decLineNumber->Name = L"label_decLineNumber";
			this->label_decLineNumber->Size = System::Drawing::Size(59, 29);
			this->label_decLineNumber->TabIndex = 10;
			this->label_decLineNumber->Text = L"N000";
			this->label_decLineNumber->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// tableLayoutPanel4
			// 
			this->tableLayoutPanel4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->tableLayoutPanel4->BackColor = System::Drawing::Color::Gainsboro;
			this->tableLayoutPanel4->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Single;
			this->tableLayoutPanel4->ColumnCount = 3;
			this->tableLayoutPanel4->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				33.33332F)));
			this->tableLayoutPanel4->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				33.33334F)));
			this->tableLayoutPanel4->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				33.33334F)));
			this->tableLayoutPanel4->Controls->Add(this->label_totalLength, 0, 0);
			this->tableLayoutPanel4->Controls->Add(this->label_residualLength, 2, 0);
			this->tableLayoutPanel4->Controls->Add(this->label_sumLength, 1, 0);
			this->tableLayoutPanel4->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->tableLayoutPanel4->Location = System::Drawing::Point(15, 290);
			this->tableLayoutPanel4->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->tableLayoutPanel4->MaximumSize = System::Drawing::Size(215, 31);
			this->tableLayoutPanel4->MinimumSize = System::Drawing::Size(215, 31);
			this->tableLayoutPanel4->Name = L"tableLayoutPanel4";
			this->tableLayoutPanel4->RowCount = 1;
			this->tableLayoutPanel4->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tableLayoutPanel4->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
			this->tableLayoutPanel4->Size = System::Drawing::Size(215, 31);
			this->tableLayoutPanel4->TabIndex = 51;
			// 
			// label_totalLength
			// 
			this->label_totalLength->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_totalLength->AutoSize = true;
			this->label_totalLength->BackColor = System::Drawing::Color::Gainsboro;
			this->label_totalLength->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_totalLength->Location = System::Drawing::Point(4, 1);
			this->label_totalLength->Name = L"label_totalLength";
			this->label_totalLength->Size = System::Drawing::Size(64, 29);
			this->label_totalLength->TabIndex = 0;
			this->label_totalLength->Text = L"00.00";
			this->label_totalLength->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_residualLength
			// 
			this->label_residualLength->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_residualLength->AutoSize = true;
			this->label_residualLength->BackColor = System::Drawing::Color::Gainsboro;
			this->label_residualLength->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_residualLength->Location = System::Drawing::Point(146, 1);
			this->label_residualLength->Name = L"label_residualLength";
			this->label_residualLength->Size = System::Drawing::Size(65, 29);
			this->label_residualLength->TabIndex = 2;
			this->label_residualLength->Text = L"00.00";
			this->label_residualLength->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_sumLength
			// 
			this->label_sumLength->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_sumLength->AutoSize = true;
			this->label_sumLength->BackColor = System::Drawing::Color::Gainsboro;
			this->label_sumLength->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_sumLength->Location = System::Drawing::Point(75, 1);
			this->label_sumLength->Name = L"label_sumLength";
			this->label_sumLength->Size = System::Drawing::Size(64, 29);
			this->label_sumLength->TabIndex = 1;
			this->label_sumLength->Text = L"00.00";
			this->label_sumLength->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label6
			// 
			this->label6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label6->BackColor = System::Drawing::Color::PaleTurquoise;
			this->label6->Font = (gcnew System::Drawing::Font(L"Calibri", 9));
			this->label6->Location = System::Drawing::Point(22, 272);
			this->label6->MaximumSize = System::Drawing::Size(61, 18);
			this->label6->MinimumSize = System::Drawing::Size(61, 18);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(61, 18);
			this->label6->TabIndex = 53;
			this->label6->Text = L"total";
			this->label6->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label5
			// 
			this->label5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label5->BackColor = System::Drawing::Color::PaleTurquoise;
			this->label5->Font = (gcnew System::Drawing::Font(L"Calibri", 9));
			this->label5->Location = System::Drawing::Point(163, 272);
			this->label5->MaximumSize = System::Drawing::Size(61, 18);
			this->label5->MinimumSize = System::Drawing::Size(61, 18);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(61, 18);
			this->label5->TabIndex = 53;
			this->label5->Text = L"resid";
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label4
			// 
			this->label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label4->BackColor = System::Drawing::Color::PaleTurquoise;
			this->label4->Font = (gcnew System::Drawing::Font(L"Calibri", 9));
			this->label4->Location = System::Drawing::Point(93, 272);
			this->label4->MaximumSize = System::Drawing::Size(61, 18);
			this->label4->MinimumSize = System::Drawing::Size(61, 18);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(61, 18);
			this->label4->TabIndex = 53;
			this->label4->Text = L"sum";
			this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// groupBox_UVWcoordinate
			// 
			this->groupBox_UVWcoordinate->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->groupBox_UVWcoordinate->Controls->Add(this->label_W);
			this->groupBox_UVWcoordinate->Controls->Add(this->label_V);
			this->groupBox_UVWcoordinate->Controls->Add(this->label_coordV);
			this->groupBox_UVWcoordinate->Controls->Add(this->label_coordW);
			this->groupBox_UVWcoordinate->Controls->Add(this->label_U);
			this->groupBox_UVWcoordinate->Controls->Add(this->label_coordU);
			this->groupBox_UVWcoordinate->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->groupBox_UVWcoordinate->Location = System::Drawing::Point(7, 331);
			this->groupBox_UVWcoordinate->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox_UVWcoordinate->MaximumSize = System::Drawing::Size(190, 152);
			this->groupBox_UVWcoordinate->MinimumSize = System::Drawing::Size(190, 152);
			this->groupBox_UVWcoordinate->Name = L"groupBox_UVWcoordinate";
			this->groupBox_UVWcoordinate->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->groupBox_UVWcoordinate->Size = System::Drawing::Size(190, 152);
			this->groupBox_UVWcoordinate->TabIndex = 46;
			this->groupBox_UVWcoordinate->TabStop = false;
			this->groupBox_UVWcoordinate->Text = L"UVW Coordinate";
			// 
			// label_W
			// 
			this->label_W->BackColor = System::Drawing::Color::Turquoise;
			this->label_W->Font = (gcnew System::Drawing::Font(L"Consolas", 20));
			this->label_W->Location = System::Drawing::Point(4, 107);
			this->label_W->Name = L"label_W";
			this->label_W->Size = System::Drawing::Size(36, 40);
			this->label_W->TabIndex = 50;
			this->label_W->Text = L"W";
			this->label_W->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_V
			// 
			this->label_V->BackColor = System::Drawing::Color::Turquoise;
			this->label_V->Font = (gcnew System::Drawing::Font(L"Consolas", 20));
			this->label_V->Location = System::Drawing::Point(4, 64);
			this->label_V->Name = L"label_V";
			this->label_V->Size = System::Drawing::Size(36, 40);
			this->label_V->TabIndex = 51;
			this->label_V->Text = L"V";
			this->label_V->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_coordV
			// 
			this->label_coordV->BackColor = System::Drawing::Color::Gainsboro;
			this->label_coordV->Font = (gcnew System::Drawing::Font(L"Calibri", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_coordV->Location = System::Drawing::Point(40, 64);
			this->label_coordV->Name = L"label_coordV";
			this->label_coordV->Size = System::Drawing::Size(147, 40);
			this->label_coordV->TabIndex = 52;
			this->label_coordV->Text = L"090.000";
			this->label_coordV->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label_coordW
			// 
			this->label_coordW->BackColor = System::Drawing::Color::Gainsboro;
			this->label_coordW->Font = (gcnew System::Drawing::Font(L"Calibri", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_coordW->Location = System::Drawing::Point(40, 107);
			this->label_coordW->Name = L"label_coordW";
			this->label_coordW->Size = System::Drawing::Size(147, 40);
			this->label_coordW->TabIndex = 53;
			this->label_coordW->Text = L"100.000";
			this->label_coordW->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label_U
			// 
			this->label_U->BackColor = System::Drawing::Color::Turquoise;
			this->label_U->Font = (gcnew System::Drawing::Font(L"Consolas", 20));
			this->label_U->Location = System::Drawing::Point(4, 21);
			this->label_U->Name = L"label_U";
			this->label_U->Size = System::Drawing::Size(36, 40);
			this->label_U->TabIndex = 49;
			this->label_U->Text = L"U";
			this->label_U->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_coordU
			// 
			this->label_coordU->BackColor = System::Drawing::Color::Gainsboro;
			this->label_coordU->Font = (gcnew System::Drawing::Font(L"Calibri", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label_coordU->Location = System::Drawing::Point(40, 21);
			this->label_coordU->Name = L"label_coordU";
			this->label_coordU->Size = System::Drawing::Size(147, 40);
			this->label_coordU->TabIndex = 51;
			this->label_coordU->Text = L"000.000";
			this->label_coordU->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// label3
			// 
			this->label3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label3->BackColor = System::Drawing::Color::PaleTurquoise;
			this->label3->Font = (gcnew System::Drawing::Font(L"Calibri", 9));
			this->label3->Location = System::Drawing::Point(415, 80);
			this->label3->MaximumSize = System::Drawing::Size(80, 29);
			this->label3->MinimumSize = System::Drawing::Size(80, 29);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(80, 29);
			this->label3->TabIndex = 53;
			this->label3->Text = L"Running";
			this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label1
			// 
			this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label1->BackColor = System::Drawing::Color::PaleTurquoise;
			this->label1->Font = (gcnew System::Drawing::Font(L"Calibri", 9));
			this->label1->Location = System::Drawing::Point(258, 79);
			this->label1->MaximumSize = System::Drawing::Size(80, 29);
			this->label1->MinimumSize = System::Drawing::Size(80, 29);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(80, 29);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Decoded";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label_intpLineNumber
			// 
			this->label_intpLineNumber->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label_intpLineNumber->BackColor = System::Drawing::Color::Gainsboro;
			this->label_intpLineNumber->Font = (gcnew System::Drawing::Font(L"Calibri", 10));
			this->label_intpLineNumber->Location = System::Drawing::Point(495, 80);
			this->label_intpLineNumber->MaximumSize = System::Drawing::Size(59, 29);
			this->label_intpLineNumber->MinimumSize = System::Drawing::Size(59, 29);
			this->label_intpLineNumber->Name = L"label_intpLineNumber";
			this->label_intpLineNumber->Size = System::Drawing::Size(59, 29);
			this->label_intpLineNumber->TabIndex = 9;
			this->label_intpLineNumber->Text = L"N000";
			this->label_intpLineNumber->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// tabControl_axisButton
			// 
			this->tabControl_axisButton->Controls->Add(this->tabPage_XYZ);
			this->tabControl_axisButton->Controls->Add(this->tabPage_UVW);
			this->tabControl_axisButton->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->tabControl_axisButton->Location = System::Drawing::Point(200, 325);
			this->tabControl_axisButton->Name = L"tabControl_axisButton";
			this->tabControl_axisButton->SelectedIndex = 0;
			this->tabControl_axisButton->Size = System::Drawing::Size(138, 158);
			this->tabControl_axisButton->SizeMode = System::Windows::Forms::TabSizeMode::FillToRight;
			this->tabControl_axisButton->TabIndex = 54;
			// 
			// tabPage_XYZ
			// 
			this->tabPage_XYZ->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->tabPage_XYZ->Controls->Add(this->button_Xminus);
			this->tabPage_XYZ->Controls->Add(this->button_Xplus);
			this->tabPage_XYZ->Controls->Add(this->button_Yplus);
			this->tabPage_XYZ->Controls->Add(this->button_Zplus);
			this->tabPage_XYZ->Controls->Add(this->button_Yminus);
			this->tabPage_XYZ->Controls->Add(this->button_Zminus);
			this->tabPage_XYZ->Font = (gcnew System::Drawing::Font(L"Calibri", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->tabPage_XYZ->ForeColor = System::Drawing::SystemColors::ControlText;
			this->tabPage_XYZ->Location = System::Drawing::Point(4, 27);
			this->tabPage_XYZ->Name = L"tabPage_XYZ";
			this->tabPage_XYZ->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_XYZ->Size = System::Drawing::Size(130, 127);
			this->tabPage_XYZ->TabIndex = 0;
			this->tabPage_XYZ->Text = L"XYZ";
			this->tabPage_XYZ->UseVisualStyleBackColor = true;
			// 
			// tabPage_UVW
			// 
			this->tabPage_UVW->Controls->Add(this->button_Uminus);
			this->tabPage_UVW->Controls->Add(this->button_Uplus);
			this->tabPage_UVW->Controls->Add(this->button_Wminus);
			this->tabPage_UVW->Controls->Add(this->button_Vplus);
			this->tabPage_UVW->Controls->Add(this->button_Vminus);
			this->tabPage_UVW->Controls->Add(this->button_Wplus);
			this->tabPage_UVW->Font = (gcnew System::Drawing::Font(L"Calibri", 7.8F));
			this->tabPage_UVW->Location = System::Drawing::Point(4, 27);
			this->tabPage_UVW->Name = L"tabPage_UVW";
			this->tabPage_UVW->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_UVW->Size = System::Drawing::Size(130, 127);
			this->tabPage_UVW->TabIndex = 1;
			this->tabPage_UVW->Text = L"UVW";
			this->tabPage_UVW->UseVisualStyleBackColor = true;
			// 
			// button_Uminus
			// 
			this->button_Uminus->BackColor = System::Drawing::Color::Gainsboro;
			this->button_Uminus->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_Uminus->Location = System::Drawing::Point(67, 7);
			this->button_Uminus->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Uminus->Name = L"button_Uminus";
			this->button_Uminus->Size = System::Drawing::Size(57, 36);
			this->button_Uminus->TabIndex = 58;
			this->button_Uminus->Text = L"U-";
			this->button_Uminus->UseVisualStyleBackColor = false;
			this->button_Uminus->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Uminus_MouseDown);
			this->button_Uminus->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Uminus_MouseUp);
			// 
			// button_Uplus
			// 
			this->button_Uplus->BackColor = System::Drawing::Color::Gainsboro;
			this->button_Uplus->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_Uplus->Location = System::Drawing::Point(6, 7);
			this->button_Uplus->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Uplus->Name = L"button_Uplus";
			this->button_Uplus->Size = System::Drawing::Size(57, 36);
			this->button_Uplus->TabIndex = 55;
			this->button_Uplus->Text = L"U+";
			this->button_Uplus->UseVisualStyleBackColor = false;
			this->button_Uplus->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Uplus_MouseDown);
			this->button_Uplus->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Uplus_MouseUp);
			// 
			// button_Wminus
			// 
			this->button_Wminus->BackColor = System::Drawing::Color::Gainsboro;
			this->button_Wminus->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_Wminus->Location = System::Drawing::Point(67, 87);
			this->button_Wminus->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Wminus->Name = L"button_Wminus";
			this->button_Wminus->Size = System::Drawing::Size(57, 36);
			this->button_Wminus->TabIndex = 60;
			this->button_Wminus->Text = L"W-";
			this->button_Wminus->UseVisualStyleBackColor = false;
			this->button_Wminus->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Wminus_MouseDown);
			this->button_Wminus->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Wminus_MouseUp);
			// 
			// button_Vplus
			// 
			this->button_Vplus->BackColor = System::Drawing::Color::Gainsboro;
			this->button_Vplus->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_Vplus->Location = System::Drawing::Point(6, 47);
			this->button_Vplus->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Vplus->Name = L"button_Vplus";
			this->button_Vplus->Size = System::Drawing::Size(57, 36);
			this->button_Vplus->TabIndex = 56;
			this->button_Vplus->Text = L"V+";
			this->button_Vplus->UseVisualStyleBackColor = false;
			this->button_Vplus->Click += gcnew System::EventHandler(this, &Form1::button_Vplus_Click);
			this->button_Vplus->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Vplus_MouseDown);
			this->button_Vplus->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Vplus_MouseUp);
			// 
			// button_Vminus
			// 
			this->button_Vminus->BackColor = System::Drawing::Color::Gainsboro;
			this->button_Vminus->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_Vminus->Location = System::Drawing::Point(67, 47);
			this->button_Vminus->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Vminus->Name = L"button_Vminus";
			this->button_Vminus->Size = System::Drawing::Size(57, 36);
			this->button_Vminus->TabIndex = 59;
			this->button_Vminus->Text = L"V-";
			this->button_Vminus->UseVisualStyleBackColor = false;
			this->button_Vminus->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Vminus_MouseDown);
			this->button_Vminus->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Vminus_MouseUp);
			// 
			// button_Wplus
			// 
			this->button_Wplus->BackColor = System::Drawing::Color::Gainsboro;
			this->button_Wplus->Font = (gcnew System::Drawing::Font(L"Calibri", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button_Wplus->Location = System::Drawing::Point(6, 87);
			this->button_Wplus->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->button_Wplus->Name = L"button_Wplus";
			this->button_Wplus->Size = System::Drawing::Size(57, 36);
			this->button_Wplus->TabIndex = 57;
			this->button_Wplus->Text = L"W+";
			this->button_Wplus->UseVisualStyleBackColor = false;
			this->button_Wplus->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Wplus_MouseDown);
			this->button_Wplus->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::button_Wplus_MouseUp);
			// 
			// button_CONTINUE
			// 
			this->button_CONTINUE->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->button_CONTINUE->Font = (gcnew System::Drawing::Font(L"Calibri", 10));
			this->button_CONTINUE->Location = System::Drawing::Point(557, 445);
			this->button_CONTINUE->Name = L"button_CONTINUE";
			this->button_CONTINUE->Size = System::Drawing::Size(173, 37);
			this->button_CONTINUE->TabIndex = 55;
			this->button_CONTINUE->Text = L"CONTINUE";
			this->button_CONTINUE->UseVisualStyleBackColor = false;
			this->button_CONTINUE->Click += gcnew System::EventHandler(this, &Form1::button_CONTINUE_Click);
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Calibri", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button1->Location = System::Drawing::Point(650, 84);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 56;
			this->button1->Text = L"UVW";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1169, 559);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->button_CONTINUE);
			this->Controls->Add(this->tabControl_axisButton);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label_Increment);
			this->Controls->Add(this->label_decLineNumber);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label_intpLineNumber);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->button_parameter);
			this->Controls->Add(this->tableLayoutPanel4);
			this->Controls->Add(this->tableLayoutPanel3);
			this->Controls->Add(this->chart_linkage);
			this->Controls->Add(this->chart1);
			this->Controls->Add(this->label_feedOverride);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Controls->Add(this->label_FO);
			this->Controls->Add(this->button_simulation);
			this->Controls->Add(this->button_diagnosis);
			this->Controls->Add(this->button_HOME);
			this->Controls->Add(this->button_TEACH);
			this->Controls->Add(this->button_WHEEL);
			this->Controls->Add(this->button_JOG);
			this->Controls->Add(this->button_MDI);
			this->Controls->Add(this->button_MEM);
			this->Controls->Add(this->button_RECORD);
			this->Controls->Add(this->groupBox_XYZcoordinate);
			this->Controls->Add(this->button_EMERGENCYSTOP);
			this->Controls->Add(this->button_RESET);
			this->Controls->Add(this->button_START);
			this->Controls->Add(this->button_SB);
			this->Controls->Add(this->button_FH);
			this->Controls->Add(this->button_M01);
			this->Controls->Add(this->button_TEACHEND);
			this->Controls->Add(this->button_TEACHSTART);
			this->Controls->Add(this->button_X100);
			this->Controls->Add(this->button_X10);
			this->Controls->Add(this->button_X1);
			this->Controls->Add(this->tableLayoutPanel2);
			this->Controls->Add(this->richTextBox_filePath);
			this->Controls->Add(this->button_OPEN);
			this->Controls->Add(this->richTextBox_ncFile);
			this->Controls->Add(this->groupBox_UVWcoordinate);
			this->Controls->Add(this->trackBar_feedOverride);
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &Form1::Form1_FormClosed);
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			this->tableLayoutPanel2->ResumeLayout(false);
			this->tableLayoutPanel2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar_feedOverride))->EndInit();
			this->groupBox_XYZcoordinate->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart_linkage))->EndInit();
			this->tableLayoutPanel3->ResumeLayout(false);
			this->tableLayoutPanel3->PerformLayout();
			this->tableLayoutPanel4->ResumeLayout(false);
			this->tableLayoutPanel4->PerformLayout();
			this->groupBox_UVWcoordinate->ResumeLayout(false);
			this->tabControl_axisButton->ResumeLayout(false);
			this->tabPage_XYZ->ResumeLayout(false);
			this->tabPage_UVW->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) 
		 {
			 cncDataPtr = &cncData; //point the realbody to the pointer
			 cncInit(cncDataPtr);
#if HD_TIMER
			 // init motion card
			 initMotionCard(0, 90, INIT_Z_POSITION, REAL_SAMPLING_TIME,Timer_ISR_Function);
			 // enable timer
			 enableHDTimer(REAL_SAMPLING_TIME);	
			 RT_timer->Enabled	= false;
			 NRT_timer->Enabled	= true;
#else
			 RT_timer->Interval = REAL_SAMPLING_TIME;
			 RT_timer->Enabled		= true;
			 NRT_timer->Enabled	= true;
#endif

		 }
private: System::Void button_OPEN_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			if(cncDataPtr->linkDataPtr->systemOpMode == MEM_Mode)
			{
				cncDataPtr->mmiDataPtr->ncFileName[200] = 0;

				openFileDialog_openFile->InitialDirectory= "";
				openFileDialog_openFile->Filter = "文字檔(*.txt)|*.txt|所有檔案(*.)|*.*";
				openFileDialog_openFile->FilterIndex= 2;
				openFileDialog_openFile->DefaultExt= "*.txt";
				openFileDialog_openFile->FileName= "";
				openFileDialog_openFile->RestoreDirectory= true;
				if(openFileDialog_openFile->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				{
					richTextBox_filePath->Text = openFileDialog_openFile->FileName;
					richTextBox_ncFile->LoadFile(openFileDialog_openFile->FileName, RichTextBoxStreamType::PlainText);
					strcpy( cncDataPtr->mmiDataPtr->ncFileName, (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi( openFileDialog_openFile->FileName) );
					cncDataPtr->mmiDataPtr->ncFileValid = true;
					cncDataPtr->mmiDataPtr->formInput = true;
				}
			}
			else
			{
				MessageBox::Show("請在MEM_MODE下使用" , "模式錯誤", MessageBoxButtons::OK , MessageBoxIcon::Warning );
			}
		}


private: System::Void RT_timer_Tick(System::Object^  sender, System::EventArgs^  e) 
		{
			cncRtCtl(cncDataPtr);
			if(cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode && uvwValid == true)
				realKnl(cncDataPtr);

			//UVW increment display
			label_uIncre->Text = cncDataPtr->intpDataPtr->uvwIncrement[0].ToString("000");
			label_vIncre->Text = cncDataPtr->intpDataPtr->uvwIncrement[1].ToString("000");
			label_wIncre->Text = cncDataPtr->intpDataPtr->uvwIncrement[2].ToString("000");

		}
private: System::Void NRT_timer_Tick(System::Object^  sender, System::EventArgs^  e) 
		 {
			 cncNrtCtl(cncDataPtr);
			 
			 //Form timer display
			 cncDataPtr->mmiDataPtr->timerCounter++;
			 label_hour->Text = cncDataPtr->mmiDataPtr->timerCounter.ToString("000");
			 if(cncDataPtr->mmiDataPtr->timerCounter > 100000)
				 cncDataPtr->mmiDataPtr->timerCounter = 0; //
			 
			 //Reset state display
			 if (cncDataPtr->linkDataPtr->systemReset == true)
			 {
				 label_year->BackColor = Color::Yellow; //label_RESET
				 richTextBox_ncFile->Text = nullptr;
				 richTextBox_filePath->Text = nullptr;
				 button_X1->BackColor = Color::Gainsboro;
				 button_X10->BackColor = Color::Gainsboro;
				 button_X100->BackColor = Color::Gainsboro;
				 button_SB->BackColor = Color::Gainsboro;
				 button_FH->BackColor = Color::Gainsboro;
				 button_M01->BackColor = Color::Gainsboro;

				 trackBar_feedOverride->Value = 100;
			 }
			 else
				 label_year->BackColor = Color::Gainsboro;
				 
			 //coordination display
			 label_coordX->Text = cncDataPtr->intpDataPtr->xyzNew[0].ToString("000.000");
			 label_coordY->Text = cncDataPtr->intpDataPtr->xyzNew[1].ToString("000.000");
			 label_coordZ->Text = cncDataPtr->intpDataPtr->xyzNew[2].ToString("000.000");
			 label_coordU->Text = cncDataPtr->intpDataPtr->uvwNew[0].ToString("000.000");
			 label_coordV->Text = cncDataPtr->intpDataPtr->uvwNew[1].ToString("000.000");
			 label_coordW->Text = cncDataPtr->intpDataPtr->uvwNew[2].ToString("000.000");
			 //length display
			 label_totalLength->Text = cncDataPtr->intpDataPtr->slopeData.totalLength.ToString("00.000");
			 label_sumLength->Text	 = cncDataPtr->intpDataPtr->slopeData.sumLength.ToString("00.00");
			 label_residualLength->Text = cncDataPtr->intpDataPtr->slopeData.residualLength.ToString("00.000");

			 //line number display
			 label_decLineNumber->Text = (cncDataPtr->decDataPtr->decState == Dec_M30_End)?
								"M30" : "N" + cncDataPtr->decDataPtr->linkDataPtr->decToIntpBlock.lineNumber.ToString("000");
			 label_decLineNumber->BackColor = (cncDataPtr->linkDataPtr->errorData.decLineNumber != 0? Color::Red:Color::Gainsboro);
			 label_intpLineNumber->Text = "N" + cncDataPtr->intpDataPtr->ncBlock.lineNumber.ToString("000");
			 //top lights color change
			 label_M30->BackColor = (cncDataPtr->linkDataPtr->M30ProgramEnd == true?Color::Yellow : Color::Gainsboro);
			 label_M00->BackColor = (cncDataPtr->intpDataPtr->intpState == Intp_RT_Function? Color::Yellow : Color::Gainsboro);
			 label_PGM->BackColor = (cncDataPtr->intpDataPtr->intpState == Intp_RT_Function ||cncDataPtr->intpDataPtr->intpState == Intp_Exe
									 || cncDataPtr->intpDataPtr->intpState == Intp_G04_Wait? Color::Yellow : Color::Gainsboro);
			 label_HX->BackColor = (cncDataPtr->intpDataPtr->xyzNew[0] == 75? Color::Yellow : Color::Gainsboro);
			 label_HY->BackColor = (cncDataPtr->intpDataPtr->xyzNew[1] == 75? Color::Yellow : Color::Gainsboro);
			 label_HZ->BackColor = (cncDataPtr->intpDataPtr->xyzNew[2] == 100? Color::Yellow : Color::Gainsboro);
			 label_INX->BackColor = (cncDataPtr->intpDataPtr->xyzNew[0] == cncDataPtr->intpDataPtr->xyzEnd[0]? Color::Yellow : Color::Gainsboro);
			 label_INY->BackColor = (cncDataPtr->intpDataPtr->xyzNew[1] == cncDataPtr->intpDataPtr->xyzEnd[1]? Color::Yellow : Color::Gainsboro);
			 label_INZ->BackColor = (cncDataPtr->intpDataPtr->xyzNew[2] == cncDataPtr->intpDataPtr->xyzEnd[2]? Color::Yellow : Color::Gainsboro);

			 //visualization of momtVb
			 this->chart1->Series["momtVb"]->Points->AddY(cncDataPtr->intpDataPtr->slopeData.momtVb);

			 bool removePointValid = false;
			 if (cncDataPtr->mmiDataPtr->timerCounter > 500)
			 {
				 removePointValid = true;
			 }
			 if (removePointValid == true)
			 {
				 this->chart1->Series["momtVb"]->Points->RemoveAt(0);
			 }//visualization of momtVb
			 
			 //visualization of linkage
			 this->chart_linkage->Series["Series1"]->Points->Clear();
			 this->chart_linkage->Series["Series2"]->Points->Clear();
			 this->chart_linkage->Series["Series4"]->Points->Clear();

			 this->chart_linkage->Series["Series1"]->Points->AddXY(0,0);
			 this->chart_linkage->Series["Series1"]->Points->
				 AddXY( 75*cos(degToRad(cncDataPtr->intpDataPtr->uvwNew[0])), 75*sin(degToRad(cncDataPtr->intpDataPtr->uvwNew[0])) );
			 this->chart_linkage->Series["Series2"]->Points->
				 AddXY( 75*cos(degToRad(cncDataPtr->intpDataPtr->uvwNew[0])), 75*sin(degToRad(cncDataPtr->intpDataPtr->uvwNew[0])) );
			 this->chart_linkage->Series["Series2"]->Points->
				 AddXY(cncDataPtr->intpDataPtr->xyzNew[0], cncDataPtr->intpDataPtr->xyzNew[1]);
			 this->chart_linkage->Series["Series3"]->Points->
				 AddXY(cncDataPtr->intpDataPtr->xyzNew[0], cncDataPtr->intpDataPtr->xyzNew[1]);
			 this->chart_linkage->Series["Series4"]->Points->
				 AddXY(cncDataPtr->intpDataPtr->xyzEnd[0], cncDataPtr->intpDataPtr->xyzEnd[1]);
			 //visualization of linkage


		 }
private: System::Void button_FH_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (cncDataPtr->linkDataPtr->systemOpMode == MEM_Mode || cncDataPtr->linkDataPtr->systemOpMode == MDI_Mode)
			 {
				cncDataPtr->mmiDataPtr->formInput = true;
				 if(cncDataPtr->mmiDataPtr->feedHold == true)
				 {
					 cncDataPtr->mmiDataPtr->feedHold = false;
					 button_FH->BackColor = Color::Gainsboro; 
				 }
				 else
				 {
					 cncDataPtr->mmiDataPtr->feedHold = true;
					 button_FH->BackColor = Color::Yellow;
				 }
			 }
		 }
private: System::Void button_M01_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (cncDataPtr->linkDataPtr->systemOpMode == MEM_Mode || cncDataPtr->linkDataPtr->systemOpMode == MDI_Mode)
			 {
				 cncDataPtr->mmiDataPtr->formInput = true;
				 if(cncDataPtr->mmiDataPtr->M01 == true)
				 {
					 cncDataPtr->mmiDataPtr->M01 = false;
					 button_M01->BackColor = Color::Gainsboro; 
				 }
				 else
				 {
					 cncDataPtr->mmiDataPtr->M01 = true;
					 button_M01->BackColor = Color::Yellow;
				 }
			 }

		 }
private: System::Void button_SB_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (cncDataPtr->linkDataPtr->systemOpMode == MEM_Mode || cncDataPtr->linkDataPtr->systemOpMode == MDI_Mode)
			 {
				 cncDataPtr->mmiDataPtr->formInput = true;
				 if(cncDataPtr->mmiDataPtr->singleBlock == true)
				 {
					 cncDataPtr->mmiDataPtr->singleBlock = false;
					 button_SB->BackColor = Color::Gainsboro; 
				 }
				 else
				 {
					 cncDataPtr->mmiDataPtr->singleBlock = true;
					 button_SB->BackColor = Color::Yellow;
				 }
			 }
				 
		 }
private: System::Void button_X1_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (cncDataPtr->linkDataPtr->systemOpMode == WHEEL_Mode)
			 {
				 cncDataPtr->mmiDataPtr->formInput = true;
				 cncDataPtr->mmiDataPtr->wheelRatio = 1;

				 button_X1->BackColor = Color::Yellow;
				 button_X10->BackColor = Color::Gainsboro;
				 button_X100->BackColor = Color::Gainsboro;
			 }
		 }
private: System::Void button_X10_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (cncDataPtr->linkDataPtr->systemOpMode == WHEEL_Mode)
			 {
				 cncDataPtr->mmiDataPtr->formInput = true;
				 cncDataPtr->mmiDataPtr->wheelRatio = 10;

				 button_X1->BackColor = Color::Gainsboro;
				 button_X10->BackColor = Color::Yellow;
				 button_X100->BackColor = Color::Gainsboro;
			 }
		 }
private: System::Void button_X100_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (cncDataPtr->linkDataPtr->systemOpMode == WHEEL_Mode)
			 {
				 cncDataPtr->mmiDataPtr->formInput = true;
				 cncDataPtr->mmiDataPtr->wheelRatio = 100;

				 button_X1->BackColor = Color::Gainsboro;
				 button_X10->BackColor = Color::Gainsboro;
				 button_X100->BackColor = Color::Yellow;
			 }
		 }
private: System::Void button_START_Click(System::Object^  sender, System::EventArgs^  e) 
		 {

			 if(cncDataPtr->mmiDataPtr->startValid == false)
			 {
				 cncDataPtr->mmiDataPtr->start = true;
				 cncDataPtr->mmiDataPtr->startValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;

				 if (cncDataPtr->linkDataPtr->systemOpMode == MDI_Mode && cncDataPtr->decDataPtr->decState == Dec_Idle)
				 {
					 char temp[1000];
					 for (int i = 0; i < 1000; i++)
						 temp[i] = 0;
					 FILE *mdiFile;
					 fopen_s(&mdiFile,"mdi.txt","w");

					 for (int i = 0; i < richTextBox_ncFile->Text->Length; i++)
						temp[i] = static_cast<char>(richTextBox_ncFile->Text[i]);
					 fprintf_s(mdiFile, temp);
					 fclose(mdiFile);

					 strcpy_s(cncDataPtr->mmiDataPtr->ncFileName, "mdi.txt");
					 cncDataPtr->mmiDataPtr->ncFileValid = true;

				 }
				 if (cncDataPtr->linkDataPtr->systemOpMode == HOME_Mode)
				 {
					 FILE *homeFile;
					 fopen_s(&homeFile,"home.txt","w");
					 fprintf_s(homeFile, "N100 G00 G90 X75 Y75 Z%d\nM30", INIT_Z_POSITION);
					 fclose(homeFile);
					 strcpy_s(cncDataPtr->mmiDataPtr->ncFileName, "home.txt");
					 cncDataPtr->mmiDataPtr->ncFileValid = true;

				 }
			 }
		 }
private: System::Void button_RESET_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(cncDataPtr->mmiDataPtr->resetValid == false)
			 {
				 cncDataPtr->mmiDataPtr->reset = true;
				 cncDataPtr->mmiDataPtr->resetValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;


			 }
		 }
private: System::Void button_EMERGENCYSTOP_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(cncDataPtr->mmiDataPtr->emergencyStopValid == false)
			 {
				 cncDataPtr->mmiDataPtr->emergencyStop = true;
				 cncDataPtr->mmiDataPtr->emergencyStopValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;
			 }
			 cncInit(cncDataPtr);
		 }

private: System::Void button_MEM_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 cncDataPtr->mmiDataPtr->opMode = MEM_Mode;
			 button_MEM->BackColor = Color::Violet; //change button color

			 button_JOG->BackColor = Color::DeepSkyBlue; 
			 button_MDI->BackColor = Color::DeepSkyBlue; 
			 button_WHEEL->BackColor = Color::DeepSkyBlue; 
			 button_TEACH->BackColor = Color::DeepSkyBlue; 
			 button_HOME->BackColor = Color::DeepSkyBlue; 
			 
			 cncDataPtr->mmiDataPtr->formInput = true;

		 }
private: System::Void button_MDI_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 cncDataPtr->mmiDataPtr->opMode = MDI_Mode;
			 button_MDI->BackColor = Color::Violet; //change button color

			 button_JOG->BackColor = Color::DeepSkyBlue; 
			 button_MEM->BackColor = Color::DeepSkyBlue; 
			 button_WHEEL->BackColor = Color::DeepSkyBlue; 
			 button_TEACH->BackColor = Color::DeepSkyBlue; 
			 button_HOME->BackColor = Color::DeepSkyBlue; 

			 cncDataPtr->mmiDataPtr->formInput = true;

		 }
private: System::Void button_JOG_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 cncDataPtr->mmiDataPtr->opMode = JOG_Mode;
			 button_JOG->BackColor = Color::Violet; //change button color 

			 button_MEM->BackColor = Color::DeepSkyBlue; 
			 button_MDI->BackColor = Color::DeepSkyBlue; 
			 button_WHEEL->BackColor = Color::DeepSkyBlue; 
			 button_TEACH->BackColor = Color::DeepSkyBlue; 
			 button_HOME->BackColor = Color::DeepSkyBlue; 

			 cncDataPtr->mmiDataPtr->formInput = true;

		 }
private: System::Void button_WHEEL_Click(System::Object^  sender, System::EventArgs^  e) 
				  {
					  cncDataPtr->mmiDataPtr->opMode = WHEEL_Mode;
					  button_WHEEL->BackColor = Color::Violet; //change button color 

					  button_MEM->BackColor = Color::DeepSkyBlue; 
					  button_MDI->BackColor = Color::DeepSkyBlue; 
					  button_JOG->BackColor = Color::DeepSkyBlue; 
					  button_TEACH->BackColor = Color::DeepSkyBlue; 
					  button_HOME->BackColor = Color::DeepSkyBlue; 

					  cncDataPtr->mmiDataPtr->formInput = true;
				  }
private: System::Void button_TEACH_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 cncDataPtr->mmiDataPtr->opMode = TEACH_Mode;
			 button_TEACH->BackColor = Color::Violet; //change button color 

			 button_MEM->BackColor = Color::DeepSkyBlue; 
			 button_MDI->BackColor = Color::DeepSkyBlue; 
			 button_WHEEL->BackColor = Color::DeepSkyBlue; 
			 button_JOG->BackColor = Color::DeepSkyBlue; 
			 button_HOME->BackColor = Color::DeepSkyBlue; 

			 cncDataPtr->mmiDataPtr->formInput = true;
		 }
private: System::Void button_HOME_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 cncDataPtr->mmiDataPtr->opMode = HOME_Mode;
			 button_HOME->BackColor = Color::Violet; //change button color 

			 button_MEM->BackColor = Color::DeepSkyBlue; 
			 button_MDI->BackColor = Color::DeepSkyBlue; 
			 button_WHEEL->BackColor = Color::DeepSkyBlue; 
			 button_TEACH->BackColor = Color::DeepSkyBlue; 
			 button_JOG->BackColor = Color::DeepSkyBlue; 

			 cncDataPtr->mmiDataPtr->formInput = true;
		 }
private: System::Void button_Xplus_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode && cncDataPtr->mmiDataPtr->axisButtonValid == false)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 1; // x+
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;

				 FILE *jogFile;
				 double edgeValue = sqrt(150*150 - pow(cncDataPtr->intpDataPtr->xyzNew[1], 2));
				 fopen_s(&jogFile,"jog.txt","w");
				 fprintf_s(jogFile, "N100 G00 G90 X%f\nM30",edgeValue);
				 fclose(jogFile);
				 strcpy_s(cncDataPtr->mmiDataPtr->ncFileName, "jog.txt");
				 cncDataPtr->mmiDataPtr->ncFileValid = true;

			 }
		 }
private: System::Void button_Xplus_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode && cncDataPtr->mmiDataPtr->axisButtonValid == false)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 0; // x+
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;
				 cncDataPtr->mmiDataPtr->mouseUp = true;
				 cncDataPtr->mmiDataPtr->mouseUpValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;
				 

			 }
		 }
private: System::Void button_Xminus_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode && cncDataPtr->mmiDataPtr->axisButtonValid == false)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 2; // x-
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;

				 FILE *jogFile;
				 double edgeValue = - sqrt(150*150 - pow(cncDataPtr->intpDataPtr->xyzNew[1], 2));
				 fopen_s(&jogFile,"jog.txt","w");
				 fprintf_s(jogFile, "N100 G00 G90 X%f\nM30",edgeValue);
				 fclose(jogFile);
				 strcpy_s(cncDataPtr->mmiDataPtr->ncFileName, "jog.txt");
				 cncDataPtr->mmiDataPtr->ncFileValid = true;
			 }
		 }
private: System::Void button_Xminus_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode && cncDataPtr->mmiDataPtr->axisButtonValid == false)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 0; // x-
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;
				 cncDataPtr->mmiDataPtr->mouseUp = true;
				 cncDataPtr->mmiDataPtr->mouseUpValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;
			 }
		 }
private: System::Void button_Yplus_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode && cncDataPtr->mmiDataPtr->axisButtonValid == false)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 3; // y+
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;

				 FILE *jogFile;
				 double edgeValue = sqrt(150*150 - pow(cncDataPtr->intpDataPtr->xyzNew[0], 2));
				 fopen_s(&jogFile,"jog.txt","w");
				 fprintf_s(jogFile, "N100 G00 G90 Y%f\nM30",edgeValue);
				 fclose(jogFile);
				 strcpy_s(cncDataPtr->mmiDataPtr->ncFileName, "jog.txt");
				 cncDataPtr->mmiDataPtr->ncFileValid = true;
			 }
		 }
private: System::Void button_Yplus_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode && cncDataPtr->mmiDataPtr->axisButtonValid == false)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 0; // y+
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;
				 cncDataPtr->mmiDataPtr->mouseUp = true;
				 cncDataPtr->mmiDataPtr->mouseUpValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;
			 }
		 }
private: System::Void button_Yminus_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode && cncDataPtr->mmiDataPtr->axisButtonValid == false)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 4; // y-
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;

				 FILE *jogFile;
				 double edgeValue = - sqrt(150*150 - pow(cncDataPtr->intpDataPtr->xyzNew[0], 2));
				 fopen_s(&jogFile,"jog.txt","w");
				 fprintf_s(jogFile, "N100 G00 G90 Y%f\nM30",edgeValue);
				 fclose(jogFile);
				 strcpy_s(cncDataPtr->mmiDataPtr->ncFileName, "jog.txt");
				 cncDataPtr->mmiDataPtr->ncFileValid = true;
			 }
		 }

private: System::Void button_Yminus_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode && cncDataPtr->mmiDataPtr->axisButtonValid == false)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 0; // y-
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;
				 cncDataPtr->mmiDataPtr->mouseUp = true;
				 cncDataPtr->mmiDataPtr->mouseUpValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;
			 }
		 }
private: System::Void button_Zplus_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode && cncDataPtr->mmiDataPtr->axisButtonValid == false)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 5; // z+
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;

				 FILE *jogFile;
				 fopen_s(&jogFile,"jog.txt","w");
				 fprintf_s(jogFile, "N100 G00 G90 Z200\nM30");
				 fclose(jogFile);
				 strcpy_s(cncDataPtr->mmiDataPtr->ncFileName, "jog.txt");
				 cncDataPtr->mmiDataPtr->ncFileValid = true;
			 }
		 }
private: System::Void button_Zplus_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 0; // z+
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;
				 cncDataPtr->mmiDataPtr->mouseUp = true;
				 cncDataPtr->mmiDataPtr->mouseUpValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;
			 }
		 }
private: System::Void button_Zminus_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 6; // z-
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;

				 FILE *jogFile;
				 fopen_s(&jogFile,"jog.txt","w");
				 fprintf_s(jogFile, "N100 G00 G90 Z0\nM30");
				 fclose(jogFile);
				 strcpy_s(cncDataPtr->mmiDataPtr->ncFileName, "jog.txt");
				 cncDataPtr->mmiDataPtr->ncFileValid = true;
			 }
		 }
private: System::Void button_Zminus_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 0; // z-
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;
				 cncDataPtr->mmiDataPtr->mouseUp = true;
				 cncDataPtr->mmiDataPtr->mouseUpValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;
			 }
		 }

private: System::Void trackBar_feedOverride_Scroll(System::Object^  sender, System::EventArgs^  e) 
		 {
			 cncDataPtr->mmiDataPtr->feedOverride = trackBar_feedOverride->Value;
			 cncDataPtr->mmiDataPtr->formInput = true;
		 }
private: System::Void trackBar_feedOverride_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 cncDataPtr->mmiDataPtr->feedOverride = trackBar_feedOverride->Value;
			 label_feedOverride->Text = trackBar_feedOverride->Value.ToString() + "%";
		 }
private: System::Void button_parameter_Click(System::Object^  sender, System::EventArgs^  e) {
		 }

private: System::Void button_Xplus_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == WHEEL_Mode && cncDataPtr->mmiDataPtr->axisButtonValid == false 
				 && cncDataPtr->mmiDataPtr->wheelRatio != 0 && cncDataPtr->decDataPtr->decState == Dec_Idle)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 1; // x+
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;

				 FILE *wheelFile;
				 double wheelCoord = cncDataPtr->mmiDataPtr->wheelRatio / 100;
				 fopen_s(&wheelFile,"wheel.txt","w");
				 fprintf_s(wheelFile, "N100 G00 G91 X%lf\nM30",wheelCoord);
				 fclose(wheelFile);
				 strcpy_s(cncDataPtr->mmiDataPtr->ncFileName, "wheel.txt");
				 cncDataPtr->mmiDataPtr->ncFileValid = true;
			 }
		 }
private: System::Void button_Xminus_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == WHEEL_Mode && cncDataPtr->mmiDataPtr->axisButtonValid == false 
				 && cncDataPtr->mmiDataPtr->wheelRatio != 0 && cncDataPtr->decDataPtr->decState == Dec_Idle)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 2; // x-
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;

				 FILE *wheelFile;
				 double wheelCoord = cncDataPtr->mmiDataPtr->wheelRatio / 100;
				 fopen_s(&wheelFile,"wheel.txt","w");
				 fprintf_s(wheelFile, "N100 G00 G91 X-%lf\nM30",wheelCoord);
				 fclose(wheelFile);
				 strcpy_s(cncDataPtr->mmiDataPtr->ncFileName, "wheel.txt");
				 cncDataPtr->mmiDataPtr->ncFileValid = true;
			 }
		 }
private: System::Void button_Yplus_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == WHEEL_Mode && cncDataPtr->mmiDataPtr->axisButtonValid == false 
				 && cncDataPtr->mmiDataPtr->wheelRatio != 0 && cncDataPtr->decDataPtr->decState == Dec_Idle)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 3; // y+
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;

				 FILE *wheelFile;
				 double wheelCoord = cncDataPtr->mmiDataPtr->wheelRatio / 100;
				 fopen_s(&wheelFile,"wheel.txt","w");
				 fprintf_s(wheelFile, "N100 G00 G91 Y%lf\nM30",wheelCoord);
				 fclose(wheelFile);
				 strcpy_s(cncDataPtr->mmiDataPtr->ncFileName, "wheel.txt");
				 cncDataPtr->mmiDataPtr->ncFileValid = true;
			 }
		 }
private: System::Void button_Yminus_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == WHEEL_Mode && cncDataPtr->mmiDataPtr->axisButtonValid == false 
				 && cncDataPtr->mmiDataPtr->wheelRatio != 0 && cncDataPtr->decDataPtr->decState == Dec_Idle)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 4; // y-
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;

				 FILE *wheelFile;
				 double wheelCoord = cncDataPtr->mmiDataPtr->wheelRatio / 100;
				 fopen_s(&wheelFile,"wheel.txt","w");
				 fprintf_s(wheelFile, "N100 G00 G91 Y-%lf\nM30",wheelCoord);
				 fclose(wheelFile);
				 strcpy_s(cncDataPtr->mmiDataPtr->ncFileName, "wheel.txt");
				 cncDataPtr->mmiDataPtr->ncFileValid = true;
			 }
		 }
private: System::Void button_Zplus_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == WHEEL_Mode && cncDataPtr->mmiDataPtr->axisButtonValid == false 
				 && cncDataPtr->mmiDataPtr->wheelRatio != 0 && cncDataPtr->decDataPtr->decState == Dec_Idle)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 5; // z+
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;

				 FILE *wheelFile;
				 double wheelCoord = cncDataPtr->mmiDataPtr->wheelRatio / 100;
				 fopen_s(&wheelFile,"wheel.txt","w");
				 fprintf_s(wheelFile, "N100 G00 G91 Z%lf\nM30",wheelCoord);
				 fclose(wheelFile);
				 strcpy_s(cncDataPtr->mmiDataPtr->ncFileName, "wheel.txt");
				 cncDataPtr->mmiDataPtr->ncFileValid = true;
			 }
		 }
private: System::Void button_Zminus_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(cncDataPtr->linkDataPtr->systemOpMode == WHEEL_Mode && cncDataPtr->mmiDataPtr->axisButtonValid == false 
				 && cncDataPtr->mmiDataPtr->wheelRatio != 0 && cncDataPtr->decDataPtr->decState == Dec_Idle)
			 {
				 cncDataPtr->mmiDataPtr->axisButton = 6; // z-
				 cncDataPtr->mmiDataPtr->axisButtonValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;

				 FILE *wheelFile;
				 double wheelCoord = cncDataPtr->mmiDataPtr->wheelRatio / 100;
				 fopen_s(&wheelFile,"wheel.txt","w");
				 fprintf_s(wheelFile, "N100 G00 G91 Z-%lf\nM30",wheelCoord);
				 fclose(wheelFile);
				 strcpy_s(cncDataPtr->mmiDataPtr->ncFileName, "wheel.txt");
				 cncDataPtr->mmiDataPtr->ncFileValid = true;
			 }
		 }
private: System::Void Form1_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
				  {

#if HD_TIMER
					  disableHDTimer();
					  closeMotionCard();
#endif

				  }
private: System::Void button_CONTINUE_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(cncDataPtr->mmiDataPtr->continueValid == false)
			 {
				 cncDataPtr->mmiDataPtr->continuee = true;
				 cncDataPtr->mmiDataPtr->continueValid = true;

				 cncDataPtr->mmiDataPtr->formInput = true;
			 }
		 }
private: System::Void button_Uplus_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode)
			 {
				 cncDataPtr->mmiDataPtr->uvwButton[0] = 1;
			 }
		 }
private: System::Void button_Uminus_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode)
			 {
				 cncDataPtr->mmiDataPtr->uvwButton[0] = -1;
			 }
		 }
private: System::Void button_Vplus_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode)
			 {
				 cncDataPtr->mmiDataPtr->uvwButton[1] = 1;
			 }
		 }
private: System::Void button_Vminus_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode)
			 {
				 cncDataPtr->mmiDataPtr->uvwButton[1] = -1;
			 }
		 }
private: System::Void button_Wplus_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode)
			 {
				 cncDataPtr->mmiDataPtr->uvwButton[2] = 1;
			 }
		 }
private: System::Void button_Wminus_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode)
			 {
				 cncDataPtr->mmiDataPtr->uvwButton[2] = -1;
			 }
		 }
private: System::Void button_Uplus_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode)
			 {
				 cncDataPtr->mmiDataPtr->uvwButton[0] = 0;
			 }
		 }
private: System::Void button_Uminus_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode)
			 {
				 cncDataPtr->mmiDataPtr->uvwButton[0] = 0;
			 }
		 }
private: System::Void button_Vplus_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button_Vplus_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode)
			 {
				 cncDataPtr->mmiDataPtr->uvwButton[1] = 0;
			 }
		 }
private: System::Void button_Vminus_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode)
			 {
				 cncDataPtr->mmiDataPtr->uvwButton[1] = 0;
			 }
		 }
private: System::Void button_Wplus_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode)
			 {
				 cncDataPtr->mmiDataPtr->uvwButton[2] = 0;
			 }
		 }
private: System::Void button_Wminus_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (cncDataPtr->linkDataPtr->systemOpMode == JOG_Mode)
			 {
				 cncDataPtr->mmiDataPtr->uvwButton[2] = 0;
			 }
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(uvwValid == true)
			 {
				uvwValid = false;
				button1->BackColor = Color::Gainsboro;
			 }
			 else{
				 uvwValid = true;
				 button1->BackColor = Color::Yellow;
			 }
		 }
};
}

