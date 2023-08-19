//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Confirma.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TDialogoConfirmacao *DialogoConfirmacao;
//---------------------------------------------------------------------
__fastcall TDialogoConfirmacao::TDialogoConfirmacao(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TDialogoConfirmacao::CancelBtnClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------



void __fastcall TDialogoConfirmacao::OKBtnClick(TObject *Sender)
{
  Close();

}
//---------------------------------------------------------------------------

