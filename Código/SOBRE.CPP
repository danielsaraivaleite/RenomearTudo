//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "sobre.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAbout *FormAbout;
//---------------------------------------------------------------------------
__fastcall TFormAbout::TFormAbout(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormAbout::Button1Click(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

