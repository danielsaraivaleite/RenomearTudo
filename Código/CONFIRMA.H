//----------------------------------------------------------------------------
#ifndef ConfirmaH
#define ConfirmaH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
#include "FormPrinc.h"
//----------------------------------------------------------------------------
class TDialogoConfirmacao : public TForm
{
__published:        
	TButton *OKBtn;
	TBevel *Bevel1;
        TImage *Image1;
        TLabel *Label1;
        TLabel *Label2;
        TMemo *Memo1;
        TLabel *LabelMensagem;
        TLabel *LabelErros;
        void __fastcall CancelBtnClick(TObject *Sender);
        void __fastcall OKBtnClick(TObject *Sender);
private:
public:
	virtual __fastcall TDialogoConfirmacao(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TDialogoConfirmacao *DialogoConfirmacao;
//----------------------------------------------------------------------------
#endif    
