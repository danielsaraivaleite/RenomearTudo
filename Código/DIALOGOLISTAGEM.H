//---------------------------------------------------------------------------

#ifndef DialogoListagemH
#define DialogoListagemH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "FormPrinc.h"
#include "Listagem.h"
#include <SysUtils.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TDialogoTipoListagem : public TForm
{
__published:	// IDE-managed Components
        TRadioGroup *RadioConteudo;
        TRadioGroup *RadioEstilo;
        TGroupBox *GrupoOpcoes;
        TButton *BotaoCancelar;
        TButton *BotaoOK;
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall BotaoCancelarClick(TObject *Sender);
        void __fastcall BotaoOKClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TDialogoTipoListagem(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDialogoTipoListagem *DialogoTipoListagem;
//---------------------------------------------------------------------------
#endif
