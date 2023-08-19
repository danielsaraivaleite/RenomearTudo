//---------------------------------------------------------------------------

#ifndef DialogoPastasH
#define DialogoPastasH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "FormPrinc.h"
//---------------------------------------------------------------------------
class TFormPastas : public TForm
{
__published:	// IDE-managed Components
        TDirectoryListBox *DirectoryListBox1;
        TDriveComboBox *DriveComboBox1;
        TButton *BotaoOk;
        TButton *BotaoCancelar;
        TGroupBox *GroupBox1;
        TGroupBox *GroupBox2;
        TEdit *BoxExtensao;
        TLabel *Label1;
        TCheckBox *CheckIncluirSubpastas;
        TPanel *Panel1;
        TRadioGroup *RadioGroup1;
        TCheckBox *CheckSistema;
        TCheckBox *CheckOculto;
        TEdit *CaixaDir;
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall DirectoryListBox1Change(TObject *Sender);
        void __fastcall BotaoCancelarClick(TObject *Sender);
        void __fastcall BotaoOkClick(TObject *Sender);
        void __fastcall DriveComboBox1Change(TObject *Sender);
private:	Byte OldDrive;
public:		// User declarations
        __fastcall TFormPastas(TComponent* Owner);
        // Tratador de Exceções
        void __fastcall HandleError(TObject *Sender, Exception *EE);
        // Busca Recursiva em Diretório
        void ProcuraArquivos(const String &directory, int tam_antes);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPastas *FormPastas;
//---------------------------------------------------------------------------
#endif
