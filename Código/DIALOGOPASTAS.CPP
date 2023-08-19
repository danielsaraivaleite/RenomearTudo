//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DialogoPastas.h"
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPastas *FormPastas;
//---------------------------------------------------------------------------
__fastcall TFormPastas::TFormPastas(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormPastas::FormActivate(TObject *Sender)
{
  OldDrive = DriveComboBox1->Drive;
  // Tratamento de Exceções de IO
  Application->OnException = HandleError;
  CaixaDir->Text=DirectoryListBox1->Directory;
}
//---------------------------------------------------------------------------
void __fastcall TFormPastas::HandleError(TObject *Sender, Exception *EE)
{
Application->MessageBox("O dispositivo não está pronto." ,"Erro na  unidade",MB_OK|MB_ICONERROR);
 DriveComboBox1->Drive = OldDrive;
}
//---------------------------------------------------------------------------



void __fastcall TFormPastas::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Application->OnException = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TFormPastas::DirectoryListBox1Change(TObject *Sender)
{
  CaixaDir->Text=DirectoryListBox1->Directory;
}
//---------------------------------------------------------------------------

void __fastcall TFormPastas::BotaoCancelarClick(TObject *Sender)
{
  Close();        
}
//---------------------------------------------------------------------------
void TFormPastas::ProcuraArquivos(const String &directory, int tam_antes)
{
   // Busca Recursiva de todos os arquivos num diretório

  WIN32_FIND_DATA filedata ;  // Estrutura de Dados para Arquivo
  HANDLE filehandle ;         // Handle para procura

  // Passo 1 - Busca no diretório
  filehandle = FindFirstFile ((directory + Label1->Caption + BoxExtensao->Text).c_str (), &filedata) ;
  if (filehandle != INVALID_HANDLE_VALUE)
  {
    do
    {
      // Elimina o que não interessa
      if ((filedata.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0 && ! CheckSistema->Checked)
        continue ;
      if ((filedata.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0 && ! CheckOculto->Checked)
        continue ;
      ////// Insere Arquivo  /////////////////////////////////////////////////
      AnsiString arquivo = (AnsiString)(directory + filedata.cFileName);
      AnsiString extensao;      // guarda extensão
      AnsiString nome;          // guarda somente o nome
      if(arquivo[arquivo.Length()]!='.')
      {
        TListItem  *ListItem;     // usado para inserir no TListView
        float tamanho;            // tamanho do arquivo em KBytes
        long file_size;           // usado para ler em Bytes o tam do arquivo
         int fp;                   // flag da função open
           // procura na lista GridArquivos
            if(!FormPrincipal->Busca_Bin(FormPrincipal->GridArquivos,arquivo,4,tam_antes))
               { // se não esta em GridArquivos então insere
                 // Pega tamanho do arquivo
                 fp = open(arquivo.c_str(), O_RDONLY|O_BINARY);
                 if (fp != -1) // algum erro
                   {
                      file_size = filelength(fp);
                      close(fp);  // fecha o arq
                      tamanho = file_size/1024;
                      if(tamanho == 0) tamanho = 1; // um arquivo ocupa no mínimo 1 cluster (4096bytes)
                      // Insere no TListView (GridArquivos)
                      ListItem = FormPrincipal->GridArquivos ->Items->Add(); // pega referência
                      nome = ExtractFileName(arquivo); // pega nome e ext
                      extensao = ExtractFileExt(arquivo);
                      nome = nome.SubString(1, nome.Length()-extensao.Length()) ; // retira ext
                      ListItem->Caption = nome;    // atribui nome
                      extensao = extensao.SubString(2,extensao.Length()) ; // retira "."
                      ListItem->SubItems->Add(extensao);
                      ListItem->SubItems->Add(FileDateToDateTime(FileAge(arquivo)).DateString()); // converte formato usual
                      ListItem->SubItems->Add(FloatToStrF(tamanho,ffFixed,5,2)); // coloca dupla precisão
                      ListItem->SubItems->Add(arquivo); // path completo
                   }
               } // fim se
      }
      ////////////////////////////////////////////////////////////////////////


      Application->ProcessMessages () ;

    } while (FindNextFile (filehandle, &filedata)) ;
    FindClose (filehandle) ;
  }
  if(!CheckIncluirSubpastas->Checked) return;
  // Passo 2 - Percorre os Diretórios Internos
  String dir = directory + "*.*" ;
  filehandle = FindFirstFile ((directory + "*.*").c_str (), &filedata) ;
  if (filehandle != INVALID_HANDLE_VALUE)
  {
    do
    {
      if ((filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0
           && String (filedata.cFileName) != "."
           && String (filedata.cFileName) != "..")
      {
        if ((filedata.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0 && ! CheckSistema->Checked)
          continue ;
        if ((filedata.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0 && ! CheckOculto->Checked)
          continue ;
        // Chamada Recursiva
        ProcuraArquivos(directory + filedata.cFileName + "\\", tam_antes) ;
      }

      Application->ProcessMessages () ;
    } while (FindNextFile (filehandle, &filedata)) ;
    FindClose (filehandle) ;
  }

  Application->ProcessMessages () ;
  return ;
}
//---------------------------------------------------------------------------
void __fastcall TFormPastas::BotaoOkClick(TObject *Sender)
{

  if(CheckIncluirSubpastas->Checked&&CaixaDir->Text.Length()==3)
   {
     int opcao = Application->MessageBox("Você está prestes a inserir todos os arquivos de uma unidade inteira! Este processo pode demorar muito.Tem certeza que deseja continuar?","Unidade selecionada",MB_YESNO|MB_ICONINFORMATION);
     if(opcao==IDNO) return; // termina método
   }
  BotaoOk->Enabled = false;
  Screen->Cursor = crHourGlass;
  String directory = DirectoryListBox1->Directory;
  if(directory[directory.Length()-1]!='\\')
   {
     directory = directory+"\\";
   }
  // Ordena
  FormPrincipal->ColunaOrdenar = FormPrincipal->ColunaAnterior = 4;
  FormPrincipal->RadioOrdem->ItemIndex=0;
  FormPrincipal->GridArquivos->AlphaSort(); // quicksort
  Screen->Cursor = crHourGlass;      // ampulheta
  // Faz a Pesquisa
  ProcuraArquivos(directory, FormPrincipal->GridArquivos->Items->Count);
  // Organiza
  FormPrincipal->LabelTotal->Caption = (AnsiString)"Total: " + (AnsiString)FormPrincipal->GridArquivos->Items->Count + (AnsiString)" arquivo(s)."   ;
  FormPrincipal->ColunaOrdenar = FormPrincipal->ColunaAnterior = 0;
  FormPrincipal->RadioOrdem->ItemIndex=0;
  FormPrincipal->GridArquivos->AlphaSort(); //quicksort
  // volta ao q era antes
  BotaoOk->Enabled = true;  // habilita botao
  Screen->Cursor = crDefault;       // cursor normal
  // Habilita ou desabilita botões de aplicar à prévia
  FormPrincipal->habilita_botoes();
  Close();                  // fecha form
}
//---------------------------------------------------------------------------

void __fastcall TFormPastas::DriveComboBox1Change(TObject *Sender)
{
  CaixaDir->Text=DirectoryListBox1->Directory;        
}
//---------------------------------------------------------------------------

