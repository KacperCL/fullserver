#include "main.h"
#include "mainWindow.h"
#pragma once

namespace fsLauncher {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Xml;
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Summary for updaterForm
	/// </summary>
	public ref class updaterForm : public System::Windows::Forms::Form
	{
	public:
		updaterForm(void)
		{
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~updaterForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  labelUpdate;
	private: System::Windows::Forms::ProgressBar^  updaterBar;
	private: System::Windows::Forms::Label^  updateStateLabel;
	private: System::Windows::Forms::Label^  updateStateChange;
	private: System::ComponentModel::BackgroundWorker^  updaterWorker;

	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(updaterForm::typeid));
			this->labelUpdate = (gcnew System::Windows::Forms::Label());
			this->updaterBar = (gcnew System::Windows::Forms::ProgressBar());
			this->updateStateLabel = (gcnew System::Windows::Forms::Label());
			this->updateStateChange = (gcnew System::Windows::Forms::Label());
			this->updaterWorker = (gcnew System::ComponentModel::BackgroundWorker());
			this->SuspendLayout();
			// 
			// labelUpdate
			// 
			this->labelUpdate->AutoSize = true;
			this->labelUpdate->BackColor = System::Drawing::Color::Transparent;
			this->labelUpdate->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->labelUpdate->ForeColor = System::Drawing::Color::White;
			this->labelUpdate->Location = System::Drawing::Point(9, 21);
			this->labelUpdate->Name = L"labelUpdate";
			this->labelUpdate->Size = System::Drawing::Size(348, 20);
			this->labelUpdate->TabIndex = 0;
			this->labelUpdate->Text = L"Trwa sprawdzanie aktualizacji... Proszê czekaæ...";
			this->labelUpdate->UseWaitCursor = true;
			// 
			// updaterBar
			// 
			this->updaterBar->Location = System::Drawing::Point(12, 93);
			this->updaterBar->Name = L"updaterBar";
			this->updaterBar->Size = System::Drawing::Size(344, 23);
			this->updaterBar->TabIndex = 1;
			this->updaterBar->UseWaitCursor = true;
			// 
			// updateStateLabel
			// 
			this->updateStateLabel->AutoSize = true;
			this->updateStateLabel->BackColor = System::Drawing::Color::Transparent;
			this->updateStateLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->updateStateLabel->ForeColor = System::Drawing::Color::White;
			this->updateStateLabel->Location = System::Drawing::Point(12, 77);
			this->updateStateLabel->Name = L"updateStateLabel";
			this->updateStateLabel->Size = System::Drawing::Size(47, 13);
			this->updateStateLabel->TabIndex = 2;
			this->updateStateLabel->Text = L"Status:";
			this->updateStateLabel->UseWaitCursor = true;
			// 
			// updateStateChange
			// 
			this->updateStateChange->AutoSize = true;
			this->updateStateChange->BackColor = System::Drawing::Color::Transparent;
			this->updateStateChange->ForeColor = System::Drawing::Color::White;
			this->updateStateChange->Location = System::Drawing::Point(65, 77);
			this->updateStateChange->Name = L"updateStateChange";
			this->updateStateChange->Size = System::Drawing::Size(77, 13);
			this->updateStateChange->TabIndex = 3;
			this->updateStateChange->Text = L"Oczekiwanie...";
			this->updateStateChange->UseWaitCursor = true;
			// 
			// updaterWorker
			// 
			this->updaterWorker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &updaterForm::updaterWorker_DoWork);
			this->updaterWorker->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &updaterForm::updaterWorker_RunWorkerCompleted);
			// 
			// updaterForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(368, 128);
			this->Controls->Add(this->updateStateChange);
			this->Controls->Add(this->updateStateLabel);
			this->Controls->Add(this->updaterBar);
			this->Controls->Add(this->labelUpdate);
			this->DoubleBuffered = true;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"updaterForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"FullServer.eu - Launcher";
			this->UseWaitCursor = true;
			this->Load += gcnew System::EventHandler(this, &updaterForm::updaterForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void updaterForm_Load(System::Object^  sender, System::EventArgs^  e) {
		this->updaterWorker->RunWorkerAsync(1);
	}
private: System::Void updaterWorker_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
	printf("[DEBUG:UPDATER] Worker started\n");

	this->updateStateChange->Text = L"Sprawdzanie lokalnego folderu...";
	//this->updaterBar->Value = 5;

	printf("[DEBUG:UPDATER] Reading md5sum from samp.dll\n");

	md5wrapper md5;
	std::string sampDllSum = md5.getHashFromFile("samp.dll");

	std::cout << "[DEBUG:UPDATER] Current md5sum for samp.dll is " << sampDllSum << "\n";

	this->updateStateChange->Text = L"Próba weryfikacji wersji...";

	// update checking
	try {
		this->updateStateChange->Text = L"Pobieranie pliku wersji...";

		XmlTextReader^ reader = gcnew XmlTextReader("https://s3-eu-west-1.amazonaws.com/i32/fslauncher/checkin.xml");

		this->updateStateChange->Text = L"Rozpakowywanie pliku wersji...";

		std::string sampVerUpd = "ERROR";
		const char* launchVerUpd = "-1";

		while (reader->Read())
		{
			if (reader->Name == "samp")
			{
				reader->Read();
				if (reader->NodeType == XmlNodeType::Text)
				{
					sampVerUpd = (const char*)(void*)Marshal::StringToHGlobalAnsi(reader->Value);
					reader->Read();
				}
			}
			else if (reader->Name == "updater")
			{
				reader->Read();
				if (reader->NodeType == XmlNodeType::Text)
				{
					launchVerUpd = (const char*)(void*)Marshal::StringToHGlobalAnsi(reader->Value);
					reader->Read();
				}
			}
		}
		std::cout << "[DEBUG:UPDATER] Got <samp>" << sampVerUpd << "</samp>\n";
		printf("[DEBUG:UPDATER] Got <updater>%s</updater>\n", launchVerUpd);

		// convert it to INT to compare!!!!!!!!!!!!!!
		std::string ssampVerUpd(sampVerUpd);
		std::string slaunchVerUpd(launchVerUpd);

		int iLaunchVerUpd = -1;
		toInt(slaunchVerUpd, iLaunchVerUpd);

		this->updateStateChange->Text = L"Sprawdzanie wersji...";

		// version checking!!!!!!!!!!!!!!!!!!
		if (sampVerUpd.compare(sampDllSum) != 0)
		{
			this->updateStateChange->Text = L"Pobieranie aktualizacji...";

			std::cout << "[DEBUG:UPDATER] SA-MP version md5sum(" << sampVerUpd << ") doesn't match current installed md5sum(" << sampDllSum << ")! Force reinstall!\n";
			System::Net::WebClient ^Client = gcnew System::Net::WebClient();
			printf("[DEBUG:UPDATER:DOWNLOAD] Download started from https://s3-eu-west-1.amazonaws.com/i32/fslauncher/update/samp_installer.exe\n");
			Client->DownloadFileAsync(gcnew Uri("https://s3-eu-west-1.amazonaws.com/i32/fslauncher/update/samp_installer.exe"), "doUpdate.exe");
			Client->DownloadFileCompleted += gcnew AsyncCompletedEventHandler(this, &updaterForm::DownloadFileCompleted);
			Client->DownloadProgressChanged += gcnew System::Net::DownloadProgressChangedEventHandler(this, &updaterForm::DownloadProgressChanged);

			while (Client->IsBusy){}
		}
		else{
			std::cout << "[DEBUG:UPDATER] SA-MP version md5sum(" << sampVerUpd << ") matches md5sum(" << sampDllSum << ")! No update needed!\n";
		}

		if (iLaunchVerUpd > VERSION)
		{
			this->updateStateChange->Text = L"Pobieranie aktualizacji...";

			printf("[DEBUG:UPDATER:DOWNLOAD] Launcher version %d is newer then %d! Force update!\n", iLaunchVerUpd, VERSION);
			System::Net::WebClient ^Client = gcnew System::Net::WebClient();
			printf("[DEBUG:UPDATER:DOWNLOAD] Download started from https://s3-eu-west-1.amazonaws.com/i32/fslauncher/update/fsLauncher_installer.exe\n");
			Client->DownloadFileAsync(gcnew Uri("https://s3-eu-west-1.amazonaws.com/i32/fslauncher/update/fsLauncher_installer.exe"), "doUpdate.exe");
			Client->DownloadFileCompleted += gcnew AsyncCompletedEventHandler(this, &updaterForm::DownloadFileCompleted);
			Client->DownloadProgressChanged += gcnew System::Net::DownloadProgressChangedEventHandler(this, &updaterForm::DownloadProgressChanged);

			while (Client->IsBusy){}
		}
		else{
			printf("[DEBUG:UPDATER] Launcher version %d matches %d! No update needed!\n", iLaunchVerUpd, VERSION);
		}
	}
	catch (Exception^ e)
	{
		this->updateStateChange->Text = L"Wystapil blad podczas sprawdzania wersji!";
		this->updaterBar->Value = 0;

		printf("[DEBUG:UPDATER] Unhandled exception while checking for update!\n");
		// print exception for debug purposes
		Console::WriteLine("Exception Type:\n   {0}", e->GetType()->ToString());
		Console::WriteLine("Exception Message:\n   {0}", e->Message);
	}

	this->updateStateChange->Text = L"Zakonczono sprawdzanie wersji!";
	this->updaterBar->Value = 100;
}
private: System::Void updaterWorker_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) {
	printf("[DEBUG:UPDATER] Worker exited\n");

	this->Hide();
}
void updaterForm::DownloadFileCompleted(System::Object ^sender, AsyncCompletedEventArgs ^e) {
	printf("[DEBUG:UPDATER:DOWNLOAD] Download finished!\n");
	openBrowser("doUpdate.exe");
	printf("[DEBUG:UPDATER:DOWNLOAD] Opening installer... suspending current process...\n");
	Application::Exit();
}
void updaterForm::DownloadProgressChanged(System::Object ^sender, System::Net::DownloadProgressChangedEventArgs ^e) {
	this->updateStateChange->Text = String::Format("Pobieranie aktualizacji... {0}% ({1} MB/{2} MB)", e->ProgressPercentage, e->BytesReceived / 1024 / 1024, e->TotalBytesToReceive / 1024 / 1024 );
	this->updaterBar->Value = e->ProgressPercentage;
	printf("[DEBUG:UPDATER:DOWNLOAD] Download state: %d\n", e->ProgressPercentage);
}
};
}
