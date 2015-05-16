#include "main.h"
#pragma once

namespace fsLauncher {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;
	using namespace System::IO;

	/// <summary>
	/// Summary for mainWindow
	/// </summary>
	public ref class mainWindow : public System::Windows::Forms::Form
	{
	public:
		String^ curDir = Directory::GetCurrentDirectory();
		mainWindow(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~mainWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::WebBrowser^  newsBrowser;
	private: System::Windows::Forms::TextBox^  nickBox;

	private: System::Windows::Forms::Button^  forumButton;
	private: System::Windows::Forms::Button^  forumButtonHover;
	private: System::Windows::Forms::Button^  locationButton;
	private: System::Windows::Forms::Button^  locationButtonHover;
	private: System::Windows::Forms::Button^  connectButtonHover;
	private: System::Windows::Forms::Button^  connectButton;
	private: System::Windows::Forms::Button^  nickButton;

	private: System::Windows::Forms::Button^  nickButtonHover;



	protected:

	protected:

	protected:

	protected:

	protected:

	protected:

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(mainWindow::typeid));
			this->newsBrowser = (gcnew System::Windows::Forms::WebBrowser());
			this->nickBox = (gcnew System::Windows::Forms::TextBox());
			this->forumButton = (gcnew System::Windows::Forms::Button());
			this->forumButtonHover = (gcnew System::Windows::Forms::Button());
			this->locationButton = (gcnew System::Windows::Forms::Button());
			this->locationButtonHover = (gcnew System::Windows::Forms::Button());
			this->connectButtonHover = (gcnew System::Windows::Forms::Button());
			this->connectButton = (gcnew System::Windows::Forms::Button());
			this->nickButton = (gcnew System::Windows::Forms::Button());
			this->nickButtonHover = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// newsBrowser
			// 
			this->newsBrowser->AllowNavigation = false;
			this->newsBrowser->AllowWebBrowserDrop = false;
			this->newsBrowser->IsWebBrowserContextMenuEnabled = false;
			this->newsBrowser->Location = System::Drawing::Point(214, 54);
			this->newsBrowser->MinimumSize = System::Drawing::Size(20, 20);
			this->newsBrowser->Name = L"newsBrowser";
			this->newsBrowser->ScriptErrorsSuppressed = true;
			this->newsBrowser->ScrollBarsEnabled = false;
			this->newsBrowser->Size = System::Drawing::Size(421, 363);
			this->newsBrowser->TabIndex = 0;
			this->newsBrowser->WebBrowserShortcutsEnabled = false;
			// 
			// nickBox
			// 
			this->nickBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(122)), static_cast<System::Int32>(static_cast<System::Byte>(142)),
				static_cast<System::Int32>(static_cast<System::Byte>(144)));
			this->nickBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->nickBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->nickBox->ForeColor = System::Drawing::Color::White;
			this->nickBox->Location = System::Drawing::Point(371, 10);
			this->nickBox->MaxLength = 20;
			this->nickBox->Name = L"nickBox";
			this->nickBox->Size = System::Drawing::Size(178, 13);
			this->nickBox->TabIndex = 1;
			this->nickBox->Text = L"Player";
			this->nickBox->WordWrap = false;
			// 
			// forumButton
			// 
			this->forumButton->BackColor = System::Drawing::Color::Transparent;
			this->forumButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"forumButton.BackgroundImage")));
			this->forumButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->forumButton->FlatAppearance->BorderSize = 0;
			this->forumButton->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->forumButton->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->forumButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->forumButton->ForeColor = System::Drawing::Color::Transparent;
			this->forumButton->Location = System::Drawing::Point(6, 378);
			this->forumButton->Name = L"forumButton";
			this->forumButton->Size = System::Drawing::Size(196, 17);
			this->forumButton->TabIndex = 2;
			this->forumButton->UseVisualStyleBackColor = false;
			this->forumButton->Click += gcnew System::EventHandler(this, &mainWindow::forumButton_Click);
			this->forumButton->MouseEnter += gcnew System::EventHandler(this, &mainWindow::forumButton_MouseEnter);
			this->forumButton->MouseLeave += gcnew System::EventHandler(this, &mainWindow::forumButton_MouseLeave);
			// 
			// forumButtonHover
			// 
			this->forumButtonHover->BackColor = System::Drawing::Color::Transparent;
			this->forumButtonHover->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"forumButtonHover.BackgroundImage")));
			this->forumButtonHover->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->forumButtonHover->Enabled = false;
			this->forumButtonHover->FlatAppearance->BorderSize = 0;
			this->forumButtonHover->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->forumButtonHover->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->forumButtonHover->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->forumButtonHover->ForeColor = System::Drawing::Color::Transparent;
			this->forumButtonHover->Location = System::Drawing::Point(6, 54);
			this->forumButtonHover->Name = L"forumButtonHover";
			this->forumButtonHover->Size = System::Drawing::Size(196, 17);
			this->forumButtonHover->TabIndex = 3;
			this->forumButtonHover->UseVisualStyleBackColor = false;
			this->forumButtonHover->Visible = false;
			// 
			// locationButton
			// 
			this->locationButton->BackColor = System::Drawing::Color::Transparent;
			this->locationButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"locationButton.BackgroundImage")));
			this->locationButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->locationButton->FlatAppearance->BorderSize = 0;
			this->locationButton->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->locationButton->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->locationButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->locationButton->ForeColor = System::Drawing::Color::Transparent;
			this->locationButton->Location = System::Drawing::Point(7, 353);
			this->locationButton->Name = L"locationButton";
			this->locationButton->Size = System::Drawing::Size(196, 17);
			this->locationButton->TabIndex = 4;
			this->locationButton->UseVisualStyleBackColor = false;
			this->locationButton->Click += gcnew System::EventHandler(this, &mainWindow::locationButton_Click);
			this->locationButton->MouseEnter += gcnew System::EventHandler(this, &mainWindow::locationButton_MouseEnter);
			this->locationButton->MouseLeave += gcnew System::EventHandler(this, &mainWindow::locationButton_MouseLeave);
			// 
			// locationButtonHover
			// 
			this->locationButtonHover->BackColor = System::Drawing::Color::Transparent;
			this->locationButtonHover->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"locationButtonHover.BackgroundImage")));
			this->locationButtonHover->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->locationButtonHover->Enabled = false;
			this->locationButtonHover->FlatAppearance->BorderSize = 0;
			this->locationButtonHover->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->locationButtonHover->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->locationButtonHover->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->locationButtonHover->ForeColor = System::Drawing::Color::Transparent;
			this->locationButtonHover->Location = System::Drawing::Point(6, 77);
			this->locationButtonHover->Name = L"locationButtonHover";
			this->locationButtonHover->Size = System::Drawing::Size(196, 17);
			this->locationButtonHover->TabIndex = 5;
			this->locationButtonHover->UseVisualStyleBackColor = false;
			this->locationButtonHover->Visible = false;
			// 
			// connectButtonHover
			// 
			this->connectButtonHover->BackColor = System::Drawing::Color::Transparent;
			this->connectButtonHover->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"connectButtonHover.BackgroundImage")));
			this->connectButtonHover->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->connectButtonHover->Enabled = false;
			this->connectButtonHover->FlatAppearance->BorderSize = 0;
			this->connectButtonHover->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->connectButtonHover->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->connectButtonHover->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->connectButtonHover->ForeColor = System::Drawing::Color::Transparent;
			this->connectButtonHover->Location = System::Drawing::Point(6, 100);
			this->connectButtonHover->Name = L"connectButtonHover";
			this->connectButtonHover->Size = System::Drawing::Size(196, 17);
			this->connectButtonHover->TabIndex = 6;
			this->connectButtonHover->UseVisualStyleBackColor = false;
			this->connectButtonHover->Visible = false;
			// 
			// connectButton
			// 
			this->connectButton->BackColor = System::Drawing::Color::Transparent;
			this->connectButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"connectButton.BackgroundImage")));
			this->connectButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->connectButton->FlatAppearance->BorderSize = 0;
			this->connectButton->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->connectButton->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->connectButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->connectButton->ForeColor = System::Drawing::Color::Transparent;
			this->connectButton->Location = System::Drawing::Point(6, 404);
			this->connectButton->Name = L"connectButton";
			this->connectButton->Size = System::Drawing::Size(196, 17);
			this->connectButton->TabIndex = 7;
			this->connectButton->UseVisualStyleBackColor = false;
			this->connectButton->Click += gcnew System::EventHandler(this, &mainWindow::connectButton_Click);
			this->connectButton->MouseEnter += gcnew System::EventHandler(this, &mainWindow::connectButton_MouseEnter);
			this->connectButton->MouseLeave += gcnew System::EventHandler(this, &mainWindow::connectButton_MouseLeave);
			// 
			// nickButton
			// 
			this->nickButton->BackColor = System::Drawing::Color::Transparent;
			this->nickButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"nickButton.BackgroundImage")));
			this->nickButton->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->nickButton->FlatAppearance->BorderSize = 0;
			this->nickButton->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->nickButton->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->nickButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->nickButton->ForeColor = System::Drawing::Color::Transparent;
			this->nickButton->Location = System::Drawing::Point(563, 10);
			this->nickButton->Name = L"nickButton";
			this->nickButton->Size = System::Drawing::Size(62, 14);
			this->nickButton->TabIndex = 8;
			this->nickButton->UseVisualStyleBackColor = false;
			this->nickButton->Click += gcnew System::EventHandler(this, &mainWindow::nickButton_Click);
			this->nickButton->MouseEnter += gcnew System::EventHandler(this, &mainWindow::nickButton_MouseEnter);
			this->nickButton->MouseLeave += gcnew System::EventHandler(this, &mainWindow::nickButton_MouseLeave);
			// 
			// nickButtonHover
			// 
			this->nickButtonHover->BackColor = System::Drawing::Color::Transparent;
			this->nickButtonHover->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"nickButtonHover.BackgroundImage")));
			this->nickButtonHover->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->nickButtonHover->Enabled = false;
			this->nickButtonHover->FlatAppearance->BorderSize = 0;
			this->nickButtonHover->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->nickButtonHover->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->nickButtonHover->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->nickButtonHover->ForeColor = System::Drawing::Color::Transparent;
			this->nickButtonHover->Location = System::Drawing::Point(6, 123);
			this->nickButtonHover->Name = L"nickButtonHover";
			this->nickButtonHover->Size = System::Drawing::Size(61, 14);
			this->nickButtonHover->TabIndex = 9;
			this->nickButtonHover->UseVisualStyleBackColor = false;
			this->nickButtonHover->Visible = false;
			// 
			// mainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(650, 430);
			this->Controls->Add(this->nickButtonHover);
			this->Controls->Add(this->nickButton);
			this->Controls->Add(this->connectButton);
			this->Controls->Add(this->connectButtonHover);
			this->Controls->Add(this->locationButtonHover);
			this->Controls->Add(this->locationButton);
			this->Controls->Add(this->forumButtonHover);
			this->Controls->Add(this->forumButton);
			this->Controls->Add(this->nickBox);
			this->Controls->Add(this->newsBrowser);
			this->DoubleBuffered = true;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->MaximizeBox = false;
			this->Name = L"mainWindow";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"FullServer.eu - Launcher";
			this->Load += gcnew System::EventHandler(this, &mainWindow::mainWindow_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void forumButton_Click(System::Object^  sender, System::EventArgs^  e) {
		openBrowser("http://i32.pl/");
	}
	private: System::Void forumButton_MouseEnter(System::Object^  sender, System::EventArgs^  e) {
		System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(mainWindow::typeid));
		this->forumButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"forumButtonHover.BackgroundImage")));
	}
	private: System::Void forumButton_MouseLeave(System::Object^  sender, System::EventArgs^  e) {
		System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(mainWindow::typeid));
		this->forumButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"forumButton.BackgroundImage")));
	}
	private: System::Void locationButton_MouseEnter(System::Object^  sender, System::EventArgs^  e) {
		System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(mainWindow::typeid));
		this->locationButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"locationButtonHover.BackgroundImage")));
	}
	private: System::Void locationButton_MouseLeave(System::Object^  sender, System::EventArgs^  e) {
		System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(mainWindow::typeid));
		this->locationButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"locationButton.BackgroundImage")));
	}
	private: System::Void connectButton_MouseEnter(System::Object^  sender, System::EventArgs^  e) {
		System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(mainWindow::typeid));
		this->connectButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"connectButtonHover.BackgroundImage")));
	}
	private: System::Void connectButton_MouseLeave(System::Object^  sender, System::EventArgs^  e) {
		System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(mainWindow::typeid));
		this->connectButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"connectButton.BackgroundImage")));
	}
	private: System::Void nickButton_MouseEnter(System::Object^  sender, System::EventArgs^  e) {
		System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(mainWindow::typeid));
		this->nickButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"nickButtonHover.BackgroundImage")));
	}
	private: System::Void nickButton_MouseLeave(System::Object^  sender, System::EventArgs^  e) {
		System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(mainWindow::typeid));
		this->nickButton->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"nickButton.BackgroundImage")));
	}
	private: System::Void mainWindow_Load(System::Object^  sender, System::EventArgs^  e) {
		
		this->newsBrowser->Navigate(String::Format("{0}\\load.htm", curDir));
		SHDocVw::WebBrowser^ newsWebBrowser = (SHDocVw::WebBrowser^)this->newsBrowser->ActiveXInstance;
		newsWebBrowser->NavigateError += gcnew SHDocVw::DWebBrowserEvents2_NavigateErrorEventHandler(this, &mainWindow::newsWebBrowserErrorHandler);
		newsWebBrowser->Silent = true;
		this->newsBrowser->Navigate("http://i32.pl/fslauncher/news.htm");

		// retrieve version and nickname from registry key
		WCHAR nickName[20];
		DWORD nickNameLength = -1;

		HKEY hKey;
		LONG openRes = RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\SAMP"), 0, KEY_QUERY_VALUE, &hKey);

		if (openRes != ERROR_SUCCESS) {
			printf("[DEBUG:SOFTWARE\\SAMP] Error opening windows registry key!\n");

			long j = RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\SAMP"), 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

			if (j != ERROR_SUCCESS) {
				printf("[DEBUG:SOFTWARE\\SAMP] Error creating windows registry key!\n");
			}
			else{
				printf("[DEBUG:SOFTWARE\\SAMP] Created new registry key\n");
			}
		}
		else{
			LONG qerRes = RegQueryValueExW(hKey, TEXT("PlayerName"), NULL, NULL, (LPBYTE)&nickName, &nickNameLength);

			if (qerRes != ERROR_SUCCESS) {
				printf("[DEBUG:SOFTWARE\\SAMP:PlayerName] Error reading windows registry key!\n");
			}
			else{
				wprintf(L"[DEBUG:SOFTWARE\\SAMP:PlayerName] Readed nickname is %s and length is %d\n", nickName, wcslen(nickName));
				nickBox->Text = gcnew String(nickName);
			}
			LONG closeOut = RegCloseKey(hKey);

			if (closeOut != ERROR_SUCCESS) {
				printf("[DEBUG:SOFTWARE\\SAMP] Error closing windows registry key!\n");
			}
		}

		// write data to reg if empty!

		if (nickNameLength == -1)
		{
			HKEY hKey;
			LONG openRes = RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\SAMP"), 0, KEY_SET_VALUE, &hKey);
			const char* playerNameWrite = PLAYER_NAME;

			if (openRes != ERROR_SUCCESS) {
				printf("[DEBUG:SOFTWARE\\SAMP] Error opening windows registry key!\n");
			}
			else{
				LONG setRes = RegSetValueExA(hKey, "PlayerName", 0, REG_SZ, (LPBYTE)playerNameWrite, strlen(playerNameWrite));

				if (setRes != ERROR_SUCCESS) {
					printf("[DEBUG:SOFTWARE\\SAMP:PlayerName] Error writing windows registry key!\n");
				}
				else{
					printf("[DEBUG:SOFTWARE\\SAMP:PlayerName] Writed nickname is %s and lenth is %d\n", playerNameWrite, strlen(playerNameWrite));
				}
				LONG closeOut = RegCloseKey(hKey);

				if (closeOut != ERROR_SUCCESS) {
					printf("[DEBUG:SOFTWARE\\SAMP] Error closing windows registry key!\n");
				}
			}
		}
	}
	private: System::Void nickButton_Click(System::Object^  sender, System::EventArgs^  e) {
		HKEY hKey;
		LONG openRes = RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\SAMP"), 0, KEY_SET_VALUE, &hKey);
		const char* nickName = (const char*)(void*)Marshal::StringToHGlobalAnsi(nickBox->Text);

		if (ContainsInvalidNickChars((char*)nickName))
		{
			MessageBox::Show("Podany nick zawiera nieprawid³owe znaki i nie mo¿e zostaæ zapisany. Je¿eli nie zmienisz go przed rozpoczêciem gry nie bêdziesz móg³ siê po³¹czyæ z serwerem.", "Wyst¹pi³ b³¹d", MessageBoxButtons::OK, MessageBoxIcon::Error);
			printf("[DEBUG] Nickname contains invalid characters!\n");
			return;
		}

		if (openRes != ERROR_SUCCESS) {
			printf("[DEBUG] Error opening windows registry key!\n");
		}

		LONG setRes = RegSetValueExA(hKey, "PlayerName", 0, REG_SZ, (LPBYTE)nickName, strlen(nickName));

		if (setRes != ERROR_SUCCESS) {
			printf("[DEBUG] Error writing windows registry key!\n");
		}

		printf("[DEBUG] Writed nickname is %s and lenth is %d\n", nickName, strlen(nickName));

		LONG closeOut = RegCloseKey(hKey);

		if (closeOut != ERROR_SUCCESS) {
			printf("[DEBUG] Error closing windows registry key!\n");
		}
	}
	private: System::Void connectButton_Click(System::Object^  sender, System::EventArgs^  e) {
		const char* nickName = (const char*)(void*)Marshal::StringToHGlobalAnsi(nickBox->Text);

		struct hostent *remoteHost;
		struct in_addr addr;

		remoteHost = gethostbyname("samp.fullserver.eu");

		if (remoteHost != NULL){
			addr.s_addr = *(u_long *)remoteHost->h_addr_list[0];
			printf("[DEBUG] DNS query returned address %s\n", inet_ntoa(addr));
			runGame(nickName, inet_ntoa(addr), 7777, "");
		}
		else{
			printf("[DEBUG] DNS query error. Using default config!\n");
			runGame(nickName, "127.0.0.1", 7777, "");
		}
	}
	private: System::Void locationButton_Click(System::Object^  sender, System::EventArgs^  e) {
		openBrowser("https://www.facebook.com/myfanpage");
	}

	private: void newsWebBrowserErrorHandler(System::Object ^pDisp, System::Object ^%URL, System::Object ^%Frame, System::Object ^%StatusCode, bool %Cancel){
		this->newsBrowser->Navigate(String::Format("{0}\\error.htm", curDir));
	}
};
}