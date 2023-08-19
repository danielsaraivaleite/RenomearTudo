//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormPrinc.h"
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <math>
#include <math.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SwCURLLabel"
#pragma link "SwCURLLabel"
#pragma link "SwCURLLabel"
#pragma link "RegularExpression"
#pragma link "RegularExpressionCustom"
#pragma resource "*.dfm"
TFormPrincipal *FormPrincipal;
//---------------------------------------------------------------------------
__fastcall TFormPrincipal::TFormPrincipal(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

/****************************************************************************
* Criação do Formulário - Evento
* Ações:  abrir lista de arquivos do menu de contexto
*****************************************************************************/
void __fastcall TFormPrincipal::FormCreate(TObject *Sender)
{  // verifica se o shell passou o arquivo com a lista de arquivos
   GetCommandLine();
   if(ParamStr(1)=="")
    {
      NomeArq = "";
      BotaoFechar->Caption="&Fechar";
      delete Memo1;  // destrói componente temporário
    }
   else
    {
      NomeArq = ParamStr(1);
    }
   iniciado = false;
   desfazercriado=false;
   PgPrevia->TabVisible = false;

  // Configuração Drag & Drop
  DragAcceptFiles(Handle, true);

}
//---------------------------------------------------------------------------

/****************************************************************************
* Clica na coluna de arquivos - Evento
* Ações:  ordena a coluna
*****************************************************************************/
void __fastcall TFormPrincipal::GridArquivosColumnClick(TObject *Sender,
      TListColumn *Column)
{
     ColunaOrdenar = Column->Index;
     ColunaAnterior = ColunaOrdenar;
     ((TCustomListView *)Sender)->AlphaSort();
}
//---------------------------------------------------------------------------

/****************************************************************************
* Função de ordenação do grid de arquivos
*****************************************************************************/
void __fastcall TFormPrincipal::GridArquivosCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
if(RadioOrdem->ItemIndex!=2)
{
// Coluna de Tamanho
if(ColunaAnterior==3)
 {
   if(RadioOrdem->ItemIndex==0)     // crescente
    {
           if (ColunaOrdenar == 0)
            Compare = CompareValue(StrToFloat(Item1->Caption),StrToFloat(Item2->Caption));
           else
            {
              int ix = (ColunaOrdenar - 1);
              Compare = CompareValue( StrToFloat(Item1->SubItems->Strings[ix]), StrToFloat(Item2->SubItems->Strings[ix]) );
            }
    }

   if(RadioOrdem->ItemIndex==1)     // decrescente
    {
           if (ColunaOrdenar == 0)
            Compare = CompareValue(StrToFloat(Item2->Caption),StrToFloat(Item1->Caption));
           else
            {
              int ix = (ColunaOrdenar - 1);
              Compare = CompareValue( StrToFloat(Item2->SubItems->Strings[ix]),StrToFloat(Item1->SubItems->Strings[ix]));
            }
    }
}
else
{
if(ColunaAnterior==2)  // eh data
 {
     if(RadioOrdem->ItemIndex==0)     // crescente
   {
          if (ColunaOrdenar == 0)
           {
            TDateTime data1(Item1->Caption, TDateTime::Date);
            TDateTime data2(Item2->Caption, TDateTime::Date);
            Compare = CompareDate(data1,data2);
           }
          else
           {
             int ix = (ColunaOrdenar - 1);
             TDateTime data1(Item1->SubItems->Strings[ix], TDateTime::Date);
             TDateTime data2(Item2->SubItems->Strings[ix], TDateTime::Date);
             Compare = CompareDate(data1, data2);
           }
   }

  if(RadioOrdem->ItemIndex==1)     // decrescente
   {
          if (ColunaOrdenar == 0)
           {
            TDateTime data1(Item1->Caption, TDateTime::Date);
            TDateTime data2(Item2->Caption, TDateTime::Date);
            Compare = CompareDate(data2,data1);
           }
          else
           {
             int ix = (ColunaOrdenar - 1);
             TDateTime data1(Item1->SubItems->Strings[ix], TDateTime::Date);
             TDateTime data2(Item2->SubItems->Strings[ix], TDateTime::Date);
             Compare = CompareDate(data2, data1);
           }
   }

} // end Data
else  // texto comum
{

  if(RadioOrdem->ItemIndex==1)     // decrescente
   {
          if (ColunaOrdenar == 0)
           Compare = CompareText(Item2->Caption,Item1->Caption);
          else
           {
             int ix = (ColunaOrdenar - 1);
             Compare = CompareText( Item2->SubItems->Strings[ix],Item1->SubItems->Strings[ix]);
           }
   }
  else    // crescente
   {
          if (ColunaOrdenar == 0)
           Compare = CompareText(Item1->Caption,Item2->Caption);
          else
           {
             int ix = (ColunaOrdenar - 1);
             Compare = CompareText( Item1->SubItems->Strings[ix],Item2->SubItems->Strings[ix]);
           }

   }
} // fim do else da Data (texto)
}
}
else // aleatorio
 {
    if(pos_aleat==10) pos_aleat = 0; // cíclico
    Compare = aleatorio[pos_aleat];
    pos_aleat++;
 }
}// fim do método
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::BitBtn1Click(TObject *Sender)
{
  ShowMessage(GridArquivos->Items->Item[0]->Caption);
}
//---------------------------------------------------------------------------


/****************************************************************************
* Ativação do formulário - Evento
* Processa lista de arquivos passados pelo Clipboard
*****************************************************************************/
void __fastcall TFormPrincipal::FormActivate(TObject *Sender)
{
ColunaOrdenar = 0;
ColunaAnterior = 0;


if(NomeArq!="") // passou uma lista de arquivos pela linha de comando
                // ou veio do menu de contexto do Windows
{
     if(NomeArq=="lista_clipboard") // do Windows
      {
         Memo1->PasteFromClipboard();
      }
     else
      {
          // carrega arquivo no objeto "invisível" memo1
          Memo1->Lines->LoadFromFile(NomeArq);
      }

      // pega numero de linhas do memo
      int cont = Memo1->Lines->Count;
      TListItem  *ListItem;     // usado para inserir no TListView
      AnsiString arquivo;       // guarda nome completo
      AnsiString extensao;      // guarda extensão
      AnsiString nome;          // guarda somente o nome
      float tamanho;            // tamanho do arquivo em KBytes
      long file_size;           // usado para ler em Bytes o tam do arquivo
      int fp;                   // flag da função open
      for(int i=0; i<cont; i++)
       { // Pega 1 linha
         arquivo = Memo1->Lines->  operator [](i);  // le do Memo1
         // Ve se o arquivo Existe
         if(!FileExists(arquivo)) continue;
         // Pega tamanho do arquivo
         fp = open(arquivo.c_str(), O_RDONLY|O_BINARY);
         if (fp == -1) // algum erro
          { AnsiString Erro;
            Erro = (AnsiString)"O arquivo:\n"+arquivo+(AnsiString)"\nnão pôde ser aberto. O arquivo pode estar corrompido ou sendo usado por outra aplicação.";
            Application->MessageBox(Erro.c_str() ,"Erro no arquivo.",MB_OK|MB_ICONERROR);
            continue;  // aborta arquivo
          }
         else
          {
            file_size = filelength(fp);
            close(fp);  // fecha o arq
          }
        tamanho = file_size/1024;
        if(tamanho == 0) tamanho = 1; // um arquivo ocupa no mínimo 1 cluster (4096bytes)
        // Insere no TListView (GridArquivos)
         ListItem = GridArquivos->Items->Add(); // pega referência
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
       // Atualiza Total
       LabelTotal->Caption = (AnsiString)"Total: " + (AnsiString)GridArquivos->Items->Count + (AnsiString)" arquivo(s)."   ;
       delete Memo1;  // destrói componente temporário
       // Habilita ou desabilita botões de aplicar à prévia
       habilita_botoes();
}


}
//---------------------------------------------------------------------------


void __fastcall TFormPrincipal::BotaoCancelarClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

/****************************************************************************
* Muda a ordenação do grid de colunas (ordem)
*****************************************************************************/
void __fastcall TFormPrincipal::RadioOrdemClick(TObject *Sender)
{
  Screen->Cursor = crHourGlass; // Exibe a Ampulheta
if(RadioOrdem->ItemIndex!=2)
 {  // Ordena pela Coluna selecionada
    ColunaOrdenar = ColunaAnterior;
 }
else
{ // Aleatório
  ColunaOrdenar = 4; // usa o tamanho
  GridArquivos->AlphaSort();  // embaralha primeiro pelo tamanho
  ColunaOrdenar = 0; // usa o campo nome
  // Gera 10 numeros aleatórios
  for(int i=0; i<10; i++)
   {
     Randomize(); // inicia gerador nºs aleat.
     aleatorio[i] = RandomRange(-1,2);
     Sleep(10); // dá tempo de mudar o relógio
   }
  pos_aleat = 0; // começa na posição zero (é um vet. circular)
}
  GridArquivos->AlphaSort();  // chana o quicksort
  Screen->Cursor = crDefault;         // cursor normal
}

//---------------------------------------------------------------------------

/****************************************************************************
* Move a posição de um arquivo no grid
*****************************************************************************/
void __fastcall TFormPrincipal::BotaoUPClick(TObject *Sender)
{
  if(GridArquivos->ItemFocused!=0&&GridArquivos->ItemFocused->Index!=0&&GridArquivos->Items->Count>1)
   {
     int superior = GridArquivos->ItemFocused->Index - 1;
     TListItem* temp = new TListItem(GridArquivos->Items);
     temp->Assign(GridArquivos->Items->operator [](superior));
     GridArquivos->Items->operator [](superior)->Assign(GridArquivos->Items->operator [](superior+1));
     GridArquivos->Items->operator [](superior+1)->Assign(temp);
     GridArquivos->Items->operator [](superior+1)->Selected = 0;
     GridArquivos->Items->operator [](superior+1)->Focused = 0;
     GridArquivos->Items->operator [](superior)->Selected = 1;
     GridArquivos->Items->operator [](superior)->Focused = 1;
     }        
}
//---------------------------------------------------------------------------

/****************************************************************************
* Move a posição de arquivo no grid
*****************************************************************************/
void __fastcall TFormPrincipal::BotaoDOWNClick(TObject *Sender)
{
    if(GridArquivos->ItemFocused!=0&&GridArquivos->Items->Count>1 && GridArquivos->ItemFocused->Index!=(GridArquivos->Items->Count -1))
   {
     int superior = GridArquivos->ItemFocused->Index;
     TListItem* temp = new TListItem(GridArquivos->Items);
     temp->Assign(GridArquivos->Items->operator [](superior));
     GridArquivos->Items->operator [](superior)->Assign(GridArquivos->Items->operator [](superior+1));
     GridArquivos->Items->operator [](superior+1)->Assign(temp);
     GridArquivos->Items->operator [](superior)->Selected = 0;
     GridArquivos->Items->operator [](superior)->Focused = 0;
     GridArquivos->Items->operator [](superior+1)->Selected = 1;
     GridArquivos->Items->operator [](superior+1)->Focused = 1;
     }        
}
//---------------------------------------------------------------------------

/****************************************************************************
* Remove um arquivo do grid de arquivos abertos
*****************************************************************************/
void __fastcall TFormPrincipal::BotaoAPAGAClick(TObject *Sender)
{
  if(GridArquivos->ItemFocused!=0&&GridArquivos->Items->Count>0)
  {
    GridArquivos->ItemFocused->Delete();
    LabelTotal->Caption="Total: " + IntToStr(GridArquivos->Items->Count) +" arquivo(s).";
    // Habilita ou desabilita botões de aplicar à prévia
    habilita_botoes();
  }
}
//---------------------------------------------------------------------------

/****************************************************************************
* Abre formulário para inclusão de pastas
*****************************************************************************/
void __fastcall TFormPrincipal::BotaoIncluirPastasClick(TObject *Sender)
{
  FormPastas->ShowModal();
}
//---------------------------------------------------------------------------

/****************************************************************************
* Adiciona arquivos ao Grid
*****************************************************************************/
void __fastcall TFormPrincipal::BotaoAdicionarClick(TObject *Sender)
{
  if(DialogoAbriArquivos->Execute())
     {
      Screen->Cursor = crHourGlass;
      // Ordena
      ColunaOrdenar = ColunaAnterior = 4;
      RadioOrdem->ItemIndex=0;
      GridArquivos->AlphaSort(); // quicksort
      Screen->Cursor = crHourGlass;      // ampulheta
      TListItem  *ListItem;      // usado para inserir no TListView
      AnsiString arquivo;        // guarda nome completo
      AnsiString extensao;       // guarda extensão
      AnsiString nome;           // guarda somente o nome
      float tamanho;             // tamanho do arquivo em KBytes
      long file_size;            // usado para ler em Bytes o tam do arquivo
      int fp;                    // flag da função open
      // Tamando da Lista antes das novas inserções
      int tamanho_antes = GridArquivos->Items->Count;
      // varre todos os arquivos passados
      for(int i=0; i<DialogoAbriArquivos->Files->Count; i++)
        {
          arquivo = DialogoAbriArquivos->Files->operator [](i); // grava nome completo
          // procura na lista GridArquivos
          if(!Busca_Bin(GridArquivos,arquivo,4,tamanho_antes))
             { // se não esta em GridArquivos então insere
               // Pega tamanho do arquivo
               fp = open(arquivo.c_str(), O_RDONLY|O_BINARY);
               if (fp == -1) // algum erro na leitura do arq
                 {
                   Screen->Cursor = crDefault; // cursor normal
                   AnsiString Erro;
                   Erro = (AnsiString)"O arquivo:\n"+arquivo+(AnsiString)"\nnão pôde ser aberto. O arquivo pode estar corrompido ou sendo usado por outra aplicação.";
                   Application->MessageBox(Erro.c_str() ,"Erro no arquivo.",MB_OK|MB_ICONERROR);
                   Screen->Cursor = crHourGlass; // ampulheta
                   continue;  // aborta arquivo
                 }
               else
                 {
                    file_size = filelength(fp);   // pega tamanho
                    close(fp);  // fecha o arq
                 }
               tamanho = file_size/1024;
               if(tamanho == 0) tamanho = 1; // um arquivo ocupa no mínimo 1 cluster (4096bytes)
               // Insere no TListView (GridArquivos)
               ListItem = GridArquivos->Items->Add(); // pega referência
               nome = ExtractFileName(arquivo); // pega nome e ext
               extensao = ExtractFileExt(arquivo);
               nome = nome.SubString(1, nome.Length()-extensao.Length()) ; // retira ext
               ListItem->Caption = nome;    // atribui nome
               extensao = extensao.SubString(2,extensao.Length()) ; // retira "."
               ListItem->SubItems->Add(extensao);
               ListItem->SubItems->Add(FileDateToDateTime(FileAge(arquivo)).DateString()); // converte formato usual
               ListItem->SubItems->Add(FloatToStrF(tamanho,ffFixed,5,2)); // coloca dupla precisão
               ListItem->SubItems->Add(arquivo); // path completo
             } // fim se
         } // fim for
       // Atualiza Total
       LabelTotal->Caption = (AnsiString)"Total: " + (AnsiString)GridArquivos->Items->Count + (AnsiString)" arquivo(s)."   ;
       ColunaOrdenar = ColunaAnterior = 0;
       RadioOrdem->ItemIndex=0;
       GridArquivos->AlphaSort();  // quicksort
       // Habilita ou desabilita botões de aplicar à prévia
       habilita_botoes();
       Screen->Cursor = crDefault;   // cursor normal
     }
}
//---------------------------------------------------------------------------

/****************************************************************************
* Algoritmo de pesquisa Binária na lista
*****************************************************************************/
bool TFormPrincipal::Busca_Bin(TListView* lista, AnsiString texto, int coluna, int tam)
{
 	int l,r,meio,achou;

	l = 0;
	r = tam-1;
	achou = 0;   //achou fica falso
        int comp=0;

	while( l<=r && !achou )
	{
		 meio = (l+r)/2;         //pega o meio do vetor
                 comp = CompareText(texto,lista->Items->operator [](meio)->SubItems->operator [](coluna-1));
		 if( comp==0 )
			 achou = true;   //achou fica verdadeiro
		 else if( comp<0 )
			 r = meio-1;
		 else
			 l = meio+1;
	}

      	if( achou )
		return true;
        return false;
}
//---------------------------------------------------------------------------

/****************************************************************************
* Fechar Programa
*****************************************************************************/
void __fastcall TFormPrincipal::BotaoFecharClick(TObject *Sender)
{
  Close();        
}
//---------------------------------------------------------------------------

/****************************************************************************
* Busca sequencial na lista
*****************************************************************************/
bool TFormPrincipal::BuscaSequencial(TListView* lista, AnsiString texto, int coluna, int last)
{   // Só precisa procurar entre os antigos, pois entre os novos não há repetidos
    for(int i=0; i<last; i++)
     {
       if(CompareText(GridArquivos->Items->operator [](i)->SubItems->operator [](coluna-1),texto)==0)
         return true;
     }
    return false;
}

/****************************************************************************
* Remove todos os arquivos do Grid
*****************************************************************************/
void __fastcall TFormPrincipal::BotaoLimpaTudoClick(TObject *Sender)
{
  GridArquivos->Clear();
  // Habilita ou desabilita botões de aplicar à prévia
  habilita_botoes();
  LabelTotal->Caption="Total: 0 arquivo(s)";
}
//---------------------------------------------------------------------------

/****************************************************************************
* Abre a prévia inicial -  com nomes originais
*****************************************************************************/
bool TFormPrincipal::Abrir_Previa()
{
    if(GridArquivos->Items->Count==0) return false; // nao selecionou nada
    if(!iniciado)
     { BarraStatus->Panels->operator [](1)->Text = "Verificando arquivos...";
       iniciado = true;
       PgArquivos->TabVisible = false;
       PgPrevia->TabVisible = true;
       BotaoIncluirPastas->Enabled = false;
       BotaoAdicionar->Enabled = false;
       BotaoAplicar->Enabled = true;
       // Inicia a Transferência dos Arquivos para a prévia
       GridPrevia->RowCount = GridArquivos->Items->Count+1;
       GridPrevia->Cells[0][0] = "Arquivo Original";
       GridPrevia->Cells[1][0] = "Novo nome";
       GridPrevia->Cells[2][0] = "Extensão";
       // Passa os arq originais para a primeira coluna
       for(int i=0; i<GridArquivos->Items->Count; i++)
        {
          GridPrevia->Cells[0][i+1] = GridArquivos->Items->operator [](i)->SubItems->operator [](3);
          GridPrevia->Cells[1][i+1] = GridArquivos->Items->operator [](i)->Caption;
          GridPrevia->Cells[2][i+1] = GridArquivos->Items->operator [](i)->SubItems->operator [](0);
        }
       BarraStatus->Panels->operator [](1)->Text = "" ;
     }
    return true;
}
//---------------------------------------------------------------------------

/****************************************************************************
* Substituição no nome de arquivo
*****************************************************************************/
void __fastcall TFormPrincipal::Subs_AplicarClick(TObject *Sender)
{
  AnsiString localizar = Subs_CaixaLocalizar->Text;
  if(localizar=="") return;
  Screen->Cursor = crHourGlass;
  Subs_Aplicar->Enabled=false;
  if(Abrir_Previa())
   {
     // Cria tabela de Recuperação
     CriaRecuperacao();
     AnsiString novo = Subs_CaixaPor->Text;
     int total = GridArquivos->Items->Count;
     // Prepara a Barra de Progressos
     BarraProgresso->Visible=true;
     BarraProgresso->Min=0;
     BarraProgresso->Max=total;
     BarraProgresso->Step=1;
     // Analisa Opções
     if(Subs_RadioOcorrencias->ItemIndex==0)
      {
       // Opção para todas as ocorrências  ///////////////////////////
       if(Subs_CheckCoincidir->Checked)
        { // Case-Sensitive
          for(int i=1; i<=total; i++)
           {
             GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i],localizar,novo);
             BarraProgresso->StepIt();
           }
        }
       else
        { // Case-not-sensitive
          for(int i=1; i<=total; i++)
           {
             GridPrevia->Cells[1][i] = AnsiReplaceText(GridPrevia->Cells[1][i],localizar,novo);
             BarraProgresso->StepIt();
           }
        }
      }
     else // Apenas a 1ª ocorrência /////////////////////////////////
      {
       if(Subs_CheckCoincidir->Checked)
        { // Case-Sensitive
          for(int i=1; i<=total; i++)
           {
             int pos = GridPrevia->Cells[1][i].AnsiPos(localizar);
             if(pos!=0)
              {
                AnsiString aux = GridPrevia->Cells[1][i].SubString(1,pos-1);
                aux = aux + novo + GridPrevia->Cells[1][i].SubString(pos+localizar.Length(),GridPrevia->Cells[1][i].Length());
                GridPrevia->Cells[1][i] = aux;
              }
             BarraProgresso->StepIt();
           }
        }
       else
        { // Case-not-sensitive
          for(int i=1; i<=total; i++)
           {
             int pos = GridPrevia->Cells[1][i].LowerCase().AnsiPos(localizar.LowerCase());
             if(pos!=0)
              {
                AnsiString aux = GridPrevia->Cells[1][i].SubString(1,pos-1);
                aux = aux + novo + GridPrevia->Cells[1][i].SubString(pos+localizar.Length(),GridPrevia->Cells[1][i].Length());
                GridPrevia->Cells[1][i] = aux;
              }
             BarraProgresso->StepIt();
           }
        }

      }
   }
  Screen->Cursor = crDefault;
  Subs_Aplicar->Enabled=true;
  BarraProgresso->Visible=false;
  Subs_CaixaLocalizar->Text = "";
  Subs_CaixaPor->Text = "";
  PlaySound("SystemAsterisk", 0, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

}
//---------------------------------------------------------------------------



/****************************************************************************
* Mudança de extensão
*****************************************************************************/
void __fastcall TFormPrincipal::Ext_AplicarClick(TObject *Sender)
{
  Screen->Cursor = crHourGlass;
  Ext_Aplicar->Enabled=false;
  if(Abrir_Previa())
   {
     // Cria tabela de Recuperação
     CriaRecuperacao();
     AnsiString novo = Ext_CaixaExt->Text;
     int total = GridArquivos->Items->Count;
     // Prepara a Barra de Progressos
     BarraProgresso->Visible=true;
     BarraProgresso->Min=0;
     BarraProgresso->Max=total;
     BarraProgresso->Step=1;
     for(int i=1; i<=total; i++)
     {
       GridPrevia->Cells[2][i] = novo;
       BarraProgresso->StepIt();
     }
     Ext_CaixaExt->Text = "";
    }
  Screen->Cursor = crDefault;
  Ext_Aplicar->Enabled=true;
  BarraProgresso->Visible=false;
  PlaySound("SystemAsterisk", 0, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

}
//---------------------------------------------------------------------------

/****************************************************************************
* Localiza e substituir na extensão
*****************************************************************************/
void __fastcall TFormPrincipal::Ext_AplicarLocalizarSubstituirClick(
      TObject *Sender)
{
  AnsiString localizar = Ext_Localizar->Text;
  if(localizar=="") return;
  Screen->Cursor = crHourGlass;     // ampulheta
  // desabilita botão
  Ext_AplicarLocalizarSubstituir->Enabled=false;
  if(Abrir_Previa())  // se existem arquivos sel.
   {
     // Cria tabela de Recuperação
     CriaRecuperacao();
     AnsiString novo = Ext_Por->Text;
     int total = GridArquivos->Items->Count;
     // Prepara a Barra de Progressos
     BarraProgresso->Visible=true;
     BarraProgresso->Min=0;
     BarraProgresso->Max=total;
     BarraProgresso->Step=1;
          for(int i=1; i<=total; i++)
           {
             GridPrevia->Cells[2][i] = AnsiReplaceText(GridPrevia->Cells[2][i],localizar,novo);
             BarraProgresso->StepIt();
           }
   }
  Screen->Cursor = crDefault;
  Ext_AplicarLocalizarSubstituir->Enabled=true;
  BarraProgresso->Visible=false;
  Ext_Localizar->Text = "";
  Ext_Por->Text = "";
  PlaySound("SystemAsterisk", 0, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);
}
//---------------------------------------------------------------------------


/****************************************************************************
* Sobre o Renomear Tudo
*****************************************************************************/
void __fastcall TFormPrincipal::Sobre1Click(TObject *Sender)
{
  FormAbout->ShowModal();        
}
//---------------------------------------------------------------------------


/****************************************************************************
* Processa alteração do Radio de Posição de Inserção
*****************************************************************************/
void __fastcall TFormPrincipal::RadioPosicaoClick(TObject *Sender)
{
   if(RadioPosicao->ItemIndex==2)
    {
      CaixaPosicaoEspecifica->Enabled=true;
      BotaoAumentaDiminuiPosicaoEspecifica->Enabled=true;
    }
   else
    {
      CaixaPosicaoEspecifica->Enabled=false;
      BotaoAumentaDiminuiPosicaoEspecifica->Enabled=false;
    }
}
//---------------------------------------------------------------------------

/****************************************************************************
* Processa Radio de texto a inserir
*****************************************************************************/
void __fastcall TFormPrincipal::RadioInserirClick(TObject *Sender)
{
 if(RadioInserir->ItemIndex==0)
  {
    GrupoTextoInserir->Visible=true;
  }
 else
  {
    GrupoTextoInserir->Visible=false;
  }
}
//---------------------------------------------------------------------------

/****************************************************************************
* Só permite números
*****************************************************************************/
void __fastcall TFormPrincipal::CaixaPosicaoEspecificaKeyPress(
      TObject *Sender, char &Key)
{
 // bloquea literais
  if(!((Key>='0'&&Key<='9')||Key=='\b'))
    Key = 0; // elimina o stream
}
//---------------------------------------------------------------------------


/****************************************************************************
* Só permite numeros
*****************************************************************************/
void __fastcall TFormPrincipal::CaixaIniciarEmKeyPress(TObject *Sender,
      char &Key)
{
 // bloquea literais
  if(!((Key>='0'&&Key<='9')||Key=='\b'))
    Key = 0; // elimina o stream           
}
//---------------------------------------------------------------------------


/****************************************************************************
* Substitui caractere na posição específica
*****************************************************************************/
void __fastcall TFormPrincipal::Caract_AplicarClick(TObject *Sender)
{
  Screen->Cursor = crHourGlass;
  Caract_Aplicar->Enabled=false;
  if(Abrir_Previa())
   {
     // Cria tabela de Recuperação
     CriaRecuperacao();
     AnsiString novo = Caract_Por->Text;
     int total = GridArquivos->Items->Count;
     // Prepara a Barra de Progressos
     BarraProgresso->Visible=true;
     BarraProgresso->Min=0;
     BarraProgresso->Max=total;
     BarraProgresso->Step=1;
     int index = StrToInt(Caract_Indice->Text);
     if(novo!=""&&novo.Length()==1)                    // subtitui
     {
       for(int i=1; i<=total; i++)
        {
          if(GridPrevia->Cells[1][i].Length()>=index)
           {
             GridPrevia->Cells[1][i] = GridPrevia->Cells[1][i].SubString(1,index-1)+novo+GridPrevia->Cells[1][i].SubString(index+1,GridPrevia->Cells[1][i].Length());
           }
          BarraProgresso->StepIt();
        }
     }
     else       // apaga um caractere
     {
       for(int i=1; i<=total; i++)
        {
           GridPrevia->Cells[1][i] = GridPrevia->Cells[1][i].Delete(index,1);
           BarraProgresso->StepIt();
        }
     }
     Caract_Por->Text = "";
    }
  Screen->Cursor = crDefault;
  Caract_Aplicar->Enabled=true;       // configs normais
  BarraProgresso->Visible=false;
  PlaySound("SystemAsterisk", 0, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

}
//---------------------------------------------------------------------------

/****************************************************************************
* Localizar e Substituir caractere na posição específica
*****************************************************************************/
void __fastcall TFormPrincipal::CaractLocalizarSubstituirAplicarClick(
      TObject *Sender)
{
  Screen->Cursor = crHourGlass;
  AnsiString localizar = Caract_LocalizarSubstituirLocalizar->Text;
  if(localizar=="") return;
  CaractLocalizarSubstituirAplicar->Enabled=false;
  if(Abrir_Previa())
   {
     // Cria tabela de Recuperação
     CriaRecuperacao();
     AnsiString novo = Caract_LocalizarSubstituirPor->Text;
     int total = GridArquivos->Items->Count;
     // Prepara a Barra de Progressos
     BarraProgresso->Visible=true;
     BarraProgresso->Min=0;
     BarraProgresso->Max=total;
     BarraProgresso->Step=1;
     int index = StrToInt(Caract_LocalizarSubstituirIndice->Text);
     if(novo!=""&&novo.Length()==1)                    // subtitui
     {
       for(int i=1; i<=total; i++)
        {
          if(GridPrevia->Cells[1][i].Length()>=index&&AnsiCompareStr(GridPrevia->Cells[1][i].SubString(index,1),localizar)==0)
           {
             GridPrevia->Cells[1][i] = GridPrevia->Cells[1][i].SubString(1,index-1)+novo+GridPrevia->Cells[1][i].SubString(index+1,GridPrevia->Cells[1][i].Length());
           }
          BarraProgresso->StepIt();
        }
     }
     else       // apaga um caractere
     {
       for(int i=1; i<=total; i++)
        {
          if(GridPrevia->Cells[1][i].Length()>=index&&AnsiCompareStr(GridPrevia->Cells[1][i].SubString(index,1),localizar)==0)
           GridPrevia->Cells[1][i] = GridPrevia->Cells[1][i].Delete(index,1);
           BarraProgresso->StepIt();
        }
     }
     Caract_LocalizarSubstituirPor->Text = "";
     Caract_LocalizarSubstituirLocalizar->Text="";
    }
  Screen->Cursor = crDefault;
  CaractLocalizarSubstituirAplicar->Enabled=true;       // configs normais
  BarraProgresso->Visible=false;
  PlaySound("SystemAsterisk", 0, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);       
}
//---------------------------------------------------------------------------

/****************************************************************************
* Maiúsculas e Minúsculas
*****************************************************************************/
void __fastcall TFormPrincipal::Maisculas_AplicarClick(TObject *Sender)
{
  Screen->Cursor = crHourGlass;
  Maisculas_Aplicar->Enabled=false;
  if(Abrir_Previa())
   {
     // Cria tabela de Recuperação
     CriaRecuperacao();
     int total = GridArquivos->Items->Count;
     // Prepara a Barra de Progressos
     BarraProgresso->Visible=true;
     BarraProgresso->Min=0;
     BarraProgresso->Max=total;
     BarraProgresso->Step=1;
     switch(Maiusculas_RadioOpcao->ItemIndex) {
      case 0:    // minusculas
        for(int i=1; i<=total; i++)
          {
           GridPrevia->Cells[1][i]=GridPrevia->Cells[1][i].LowerCase();
           BarraProgresso->StepIt();
          }
        break;

      case 1: // maiusculas
           for(int i=1; i<=total; i++)
             {
               GridPrevia->Cells[1][i]=GridPrevia->Cells[1][i].UpperCase();
               BarraProgresso->StepIt();
             }
        break;

      case 2:  // somente inicial maiusculas
           for(int i=1; i<=total; i++)
             {
               GridPrevia->Cells[1][i]=GridPrevia->Cells[1][i].SubString(1,1).UpperCase()+GridPrevia->Cells[1][i].SubString(2,GridPrevia->Cells[1][i].Length()).LowerCase();
               BarraProgresso->StepIt();
             }
        break;

      case 3:  // inicial de cada palavra em maiuscula
           for(int i=1; i<=total; i++)
             {
               AnsiString texto = GridPrevia->Cells[1][i].LowerCase();
               AnsiString novo = "";
               TStringList *lista = GetTokens(texto);
               for(int i=0; i<lista->Count; i++) {
                 novo = novo + lista->Strings[i].SubString(1,1).UpperCase() + lista->Strings[i].SubString(2,lista->Strings[i].Length()) +  " ";
               }
               novo = novo.TrimLeft();
               novo = novo.TrimRight();
               GridPrevia->Cells[1][i] = novo;
               BarraProgresso->StepIt();
               delete lista;
             }
        break;

      case 4:  // inicial de cada palavra em maiuscula (exceto preposições)
           TStringList *StopList = new TStringList();
           StopList->LoadFromFile(ExtractFilePath(Application->ExeName)+"stoplist.txt"); 
           for(int i=1; i<=total; i++)
             {
               AnsiString texto = GridPrevia->Cells[1][i].LowerCase();
               AnsiString novo = "";
               TStringList *lista = GetTokens(texto);
               for(int i=0; i<lista->Count; i++) {
                 if(StopList->IndexOf(lista->Strings[i])<0)
                        novo = novo + lista->Strings[i].SubString(1,1).UpperCase() + lista->Strings[i].SubString(2,lista->Strings[i].Length()) +  " ";
                 else
                        novo = novo + lista->Strings[i] +  " ";
               }
               novo = novo.TrimLeft();
               novo = novo.TrimRight();
               novo = novo.SubString(1,1).UpperCase() + novo.SubString(2,novo.Length());
               GridPrevia->Cells[1][i] = novo;
               BarraProgresso->StepIt();
               delete lista;
             }
           delete StopList;
        break;

     } // end case

  }
  Screen->Cursor = crDefault;
  Maisculas_Aplicar->Enabled=true;
  BarraProgresso->Visible=false;
  PlaySound("SystemAsterisk", 0, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

}
//---------------------------------------------------------------------------

/****************************************************************************
* Inserir (texto, data, ...)
*****************************************************************************/
void __fastcall TFormPrincipal::BotaoAplicarInserirClick(TObject *Sender)
{
  Screen->Cursor = crHourGlass;
  BotaoAplicarInserir->Enabled=false;
  if(Abrir_Previa())
   {
     // Cria tabela de Recuperação
     CriaRecuperacao();
     int total = GridArquivos->Items->Count;
     // Prepara a Barra de Progressos
     BarraProgresso->Visible=true;
     BarraProgresso->Min=0;
     BarraProgresso->Max=total;
     BarraProgresso->Step=1;
     AnsiString texto;
     // Info para Data
     TDateTime data;
     data = data.CurrentDateTime();
     switch(RadioInserir->ItemIndex)
      {
       case 0: texto = CaixaTextoInserir->Text; break;   // texto puro
       case 1:  {
                  texto = data.DateString();
                  texto = AnsiReplaceText(texto,"/","-");
                  texto = AnsiReplaceText(texto," ","_");
                }
       case 2:  {
                  texto = data.FormatString("dd/mm/yy");
                  texto = AnsiReplaceText(texto,"/","-");
                  texto = AnsiReplaceText(texto," ","_");
                  break;
                }
       case 3:  {
                  texto = data.DateString();
                  texto+="_"+data.FormatString("hh:nn");
                  texto = AnsiReplaceText(texto,"/","-");
                  texto = AnsiReplaceText(texto,":","-");
                  break;
                }
      }
     int posicao=0;
     if(RadioPosicao->ItemIndex==0&&RadioInserir->ItemIndex!=0) texto = texto+" ";
     if(RadioPosicao->ItemIndex==0) posicao=1;
     if(RadioPosicao->ItemIndex==2) posicao=StrToInt(CaixaPosicaoEspecifica->Text);

     for(int i=1; i<=total; i++)
     {
       if(RadioPosicao->ItemIndex==1) posicao=GridPrevia->Cells[1][i].Length()+1;
       if(RadioInserir->ItemIndex==4)
        {
          texto=AnsiReplaceText(GridArquivos->Items->operator [](i-1)->SubItems->operator [](1),"/","-");
          if(RadioPosicao->ItemIndex==0) texto=texto+" ";
        }
       GridPrevia->Cells[1][i] = GridPrevia->Cells[1][i].Insert(texto,posicao);
       BarraProgresso->StepIt();
     }
     CaixaTextoInserir->Text = "";
    }
  Screen->Cursor = crDefault;
  BotaoAplicarInserir->Enabled=true;
  BarraProgresso->Visible=false;
  PlaySound("SystemAsterisk", 0, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);
}
//---------------------------------------------------------------------------

/****************************************************************************
* Padrão indexado
*****************************************************************************/
void __fastcall TFormPrincipal::BotaoPadraoAplicarClick(TObject *Sender)
{

  // Confere o Padrão /////////////////////////////////////////////////////////
  AnsiString pattern;
  pattern = CaixaPrefixo->Text + CaixaSufixo->Text;
  pattern=AnsiReplaceText(pattern, "<atual>", "");
  if(pattern.Pos(">")!=0||pattern.Pos("<")!=0)
   {
     // Erro
     Application->MessageBox("O formato especificado como padrão não é valido ou há caracteres inválidos na string para um nome de arquivo. Verifique erros de digitação." ,"Padrão Inválido!",MB_OK|MB_ICONERROR);
     return;
   }

  Screen->Cursor = crHourGlass;
  BotaoPadraoAplicar->Enabled=false;
  if(Abrir_Previa())
   {
     // Cria tabela de Recuperação
     CriaRecuperacao();
     int total = GridArquivos->Items->Count;
     // Prepara a Barra de Progressos
     BarraProgresso->Visible=true;
     BarraProgresso->Min=0;
     BarraProgresso->Max=total;
     BarraProgresso->Step=1;
     AnsiString index;
     long comeco = StrToInt(CaixaIniciarEm->Text);
     int incremento = StrToInt(CaixaIncremento->Text);
     for(int i=1; i<=total; i++)
     {
       index = IntToStr(comeco);
       for(int j=index.Length(); j<=RadioEstilo->ItemIndex; j++)
         index = "0" + index;
       AnsiString prefixo, sufixo;
       prefixo = AnsiReplaceText(CaixaPrefixo->Text, "<atual>", GridPrevia->Cells[1][i]);
       sufixo =  AnsiReplaceText(CaixaSufixo->Text, "<atual>", GridPrevia->Cells[1][i]);
       GridPrevia->Cells[1][i] = prefixo + index + sufixo;
       comeco+=incremento;
       BarraProgresso->StepIt();
     }
     CaixaPrefixo->Text = "";
     CaixaSufixo->Text = "";
    }
  Screen->Cursor = crDefault;
  BotaoPadraoAplicar->Enabled=true;
  BarraProgresso->Visible=false;
  PlaySound("SystemAsterisk", 0, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::Subs_CaixaLocalizarKeyPress(
      TObject *Sender, char &Key)
{
 // bloquea caracteres proibidos em nomes de arquivos
  if(Key=='\\'||Key=='/'||Key=='*'||Key=='?'||Key=='"'||Key=='<'||Key=='>'||Key=='|')
    Key = 0; // elimina o stream
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::Subs_CaixaPorKeyPress(TObject *Sender,
      char &Key)
{
 // bloquea caracteres proibidos em nomes de arquivos
  if(Key=='\\'||Key=='/'||Key=='*'||Key=='?'||Key=='"'||Key=='<'||Key=='>'||Key=='|')
    Key = 0; // elimina o stream        
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::Ext_CaixaExtKeyPress(TObject *Sender,
      char &Key)
{
 // bloquea caracteres proibidos em nomes de arquivos
  if(Key=='\\'||Key=='/'||Key=='*'||Key=='?'||Key=='"'||Key=='<'||Key=='>'||Key=='|')
    Key = 0; // elimina o stream        
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::Ext_LocalizarKeyPress(TObject *Sender,
      char &Key)
{
 // bloquea caracteres proibidos em nomes de arquivos
  if(Key=='\\'||Key=='/'||Key=='*'||Key=='?'||Key=='"'||Key=='<'||Key=='>'||Key=='|')
    Key = 0; // elimina o stream        
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::Ext_PorKeyPress(TObject *Sender, char &Key)
{
 // bloquea caracteres proibidos em nomes de arquivos
  if(Key=='\\'||Key=='/'||Key=='*'||Key=='?'||Key=='"'||Key=='<'||Key=='>'||Key=='|')
    Key = 0; // elimina o stream        
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::CaixaTextoInserirKeyPress(TObject *Sender,
      char &Key)
{
 // bloquea caracteres proibidos em nomes de arquivos
  if(Key=='\\'||Key=='/'||Key=='*'||Key=='?'||Key=='"'||Key=='<'||Key=='>'||Key=='|')
    Key = 0; // elimina o stream        
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::CaixaPrefixoKeyPress(TObject *Sender,
      char &Key)
{
 // bloquea caracteres proibidos em nomes de arquivos
  if(Key=='\\'||Key=='/'||Key=='*'||Key=='?'||Key=='"'||Key=='|')
    Key = 0; // elimina o stream
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::CaixaSufixoKeyPress(TObject *Sender,
      char &Key)
{
 // bloquea caracteres proibidos em nomes de arquivos
  if(Key=='\\'||Key=='/'||Key=='*'||Key=='?'||Key=='"'||Key=='|')
    Key = 0; // elimina o stream        
}
//---------------------------------------------------------------------------

/****************************************************************************
* Cri recuperação - para desfazer
*****************************************************************************/
void TFormPrincipal::CriaRecuperacao()
{          
     int total = GridArquivos->Items->Count;
     // Prepara a Barra de Progressos
     BarraProgresso->Visible=true;
     BarraProgresso->Min=0;
     BarraProgresso->Max=total;
     BarraProgresso->Step=1;
     BarraStatus->Panels->operator [](1)->Text="Criando tabela de recuperação...";
     if(!desfazercriado)
     {
       nomes = new AnsiString[GridArquivos->Items->Count+1];
       extensoes = new AnsiString[GridArquivos->Items->Count+1];
     }
     desfazercriado=true;
     for(int i=1; i<=GridArquivos->Items->Count; i++)
      {
        nomes[i] = GridPrevia->Cells[1][i];
        extensoes[i] = GridPrevia->Cells[2][i];
        BarraProgresso->StepIt();
      }
    BarraStatus->Panels->operator [](1)->Text="";
    BarraProgresso->Visible=false;
}
void __fastcall TFormPrincipal::BotaoDesfazerClick(TObject *Sender)
{
    BarraStatus->Panels->operator [](1)->Text="Desfazendo...";
     Screen->Cursor = crHourGlass;
     for(int i=1; i<=GridArquivos->Items->Count; i++)
      {
        GridPrevia->Cells[1][i]  = nomes[i];
        GridPrevia->Cells[2][i] = extensoes[i];
      }
    Screen->Cursor = crDefault;
    BarraStatus->Panels->operator [](1)->Text="";
    PlaySound("SystemAsterisk", 0, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

}
//---------------------------------------------------------------------------

/****************************************************************************
* Desfazer
*****************************************************************************/
void __fastcall TFormPrincipal::Button1Click(TObject *Sender)
{
    BarraStatus->Panels->operator [](1)->Text="Retornando aos nomes originais...";
    Screen->Cursor = crHourGlass;
    for(int i=0; i<GridArquivos->Items->Count; i++)
        {
          GridPrevia->Cells[1][i+1] = GridArquivos->Items->operator [](i)->Caption;
          GridPrevia->Cells[2][i+1] = GridArquivos->Items->operator [](i)->SubItems->operator [](0);
        }
    Screen->Cursor = crDefault;
    BarraStatus->Panels->operator [](1)->Text="";
    PlaySound("SystemAsterisk", 0, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);

}
//---------------------------------------------------------------------------


/****************************************************************************
* Gerar relatório arquivos
*****************************************************************************/
void __fastcall TFormPrincipal::BotaoRelatorioClick(TObject *Sender)
{
  DialogoTipoListagem->ShowModal();
}
//---------------------------------------------------------------------------

/****************************************************************************
* Aplicar alterações
*****************************************************************************/
void __fastcall TFormPrincipal::BotaoAplicarAlteracoesClick(
      TObject *Sender)
{
  if(!iniciado) return;
  int opcao = Application->MessageBox("Tem certeza que deseja renomear definitivamente esses arquivos?","Confirmação",MB_YESNO|MB_ICONQUESTION);
  if(opcao==IDNO) return; // termina método
  ///////////////////
  Cursor = crHourGlass;
  int total = FormPrincipal->GridArquivos->Items->Count;
  FormPrincipal->BarraStatus->Panels->operator [](1)->Text = "Renomeando...";
  FormPrincipal->BarraProgresso->Visible=true;
  FormPrincipal->BarraProgresso->Min=0;
  FormPrincipal->BarraProgresso->Max=total;
  FormPrincipal->BarraProgresso->Step=1;
  AnsiString erros = "";
  int sucessos = 0;
  for(int i=1; i<=total; i++)
   {
     AnsiString velho =   FormPrincipal->GridPrevia->Cells[0][i];
     AnsiString novo = ExtractFilePath(velho)+FormPrincipal->GridPrevia->Cells[1][i]+"."+FormPrincipal->GridPrevia->Cells[2][i];
     if(RenameFile(velho,novo))
      {
        sucessos++;
      }
     else
      {
        erros = erros+"\n"+velho;
      }
     FormPrincipal->BarraProgresso->StepIt();
   }
  Cursor = crDefault;
  PlaySound("SystemAsterisk", 0, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);
  // Mostra resultado
  AnsiString mensagem = IntToStr(sucessos)+" de "+IntToStr(total)+" arquivos foram renomeados.";
  if(erros!="")
   {
      DialogoConfirmacao->Memo1->Visible = true;
      DialogoConfirmacao->LabelErros->Visible = true;
      DialogoConfirmacao->Memo1->Clear();
      DialogoConfirmacao->Memo1->Text = erros;
   }
  else
   {
     DialogoConfirmacao->LabelMensagem->Top = 125;
   }
  DialogoConfirmacao->LabelMensagem->Caption = mensagem;
  DialogoConfirmacao->ShowModal();
  FormPrincipal->Close();         // fecha o programa
}
//---------------------------------------------------------------------------


void __fastcall TFormPrincipal::GridPreviaKeyPress(TObject *Sender,
      char &Key)
{
  if(GridPrevia->Col==0)
   {
     Key=0;
   }
}
//---------------------------------------------------------------------------



void __fastcall TFormPrincipal::GridPreviaSetEditText(TObject *Sender,
      int ACol, int ARow, const AnsiString Value)
{
      LabelNomeNovo->Caption="Nome novo     : " + GridPrevia->Cells[1][ARow] + "." + GridPrevia->Cells[2][ARow];
     LabelNomeVelho->Caption="Nome original :  "+ ExtractFileName(GridPrevia->Cells[0][ARow]);
}
//---------------------------------------------------------------------------

/****************************************************************************
* Seleciona célula do grid prévia
*****************************************************************************/
void __fastcall TFormPrincipal::GridPreviaSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
  if(ACol==0) CanSelect=false;
  else
   {
      LabelNomeNovo->Caption="Nome novo     : " + GridPrevia->Cells[1][ARow] + "." + GridPrevia->Cells[2][ARow];
     LabelNomeVelho->Caption="Nome original :  "+ ExtractFileName(GridPrevia->Cells[0][ARow]);
   }
}
//---------------------------------------------------------------------------


/****************************************************************************
* Tags ID3 - Mp3
*****************************************************************************/
void __fastcall TFormPrincipal::BotaoAplicarMp3Click(TObject *Sender)
{
  // Confere o Padrão /////////////////////////////////////////////////////////
  AnsiString pattern;
  pattern = CaixaPadrao->Text;
  pattern=AnsiReplaceText(pattern, "<Titulo>", "");
  pattern=AnsiReplaceText(pattern, "<Artista>", "");
  pattern=AnsiReplaceText(pattern, "<Genero>", "");
  pattern=AnsiReplaceText(pattern, "<Ano>", "");
  pattern=AnsiReplaceText(pattern, "<Album>", "");
  pattern=AnsiReplaceText(pattern, "<Comentario>", "");
  pattern=AnsiReplaceText(pattern, "<Numero>", "");
  if(pattern.Pos(">")!=0||pattern.Pos("<")!=0||CaixaPadrao->Text.Trim()=="")
   {
     // Erro
     Application->MessageBox("O formato especificado como padrão não é valido ou há caracteres inválidos na string para um nome de arquivo. Verifique erros de digitação." ,"Padrão Inválido!",MB_OK|MB_ICONERROR);
     return;
   }
  //////Analisa as Tagas colocadas pelo usuário //////////////////////////////
  pattern = CaixaPadrao->Text; // padrao da caixa de texto
  bool tem_titulo = false;
  bool tem_artista = false;
  bool tem_genero = false;
  bool tem_ano = false;
  bool tem_album = false;
  bool tem_comentario = false;
  bool tem_numero = false;
  int tags_especificadas = 0;
  if(AnsiContainsText(pattern, "<Titulo>"))
   { tags_especificadas++;
     tem_titulo = true;
   }
  if(AnsiContainsText(pattern, "<Artista>"))
    { tags_especificadas++;
      tem_artista = true;
    }
  if(AnsiContainsText(pattern, "<Genero>"))
   { tags_especificadas++;
     tem_genero = true;
   }
  if(AnsiContainsText(pattern, "<Ano>"))
   { tags_especificadas++;
     tem_ano = true;
   }
  if(AnsiContainsText(pattern, "<Album>"))
    { tags_especificadas++;
      tem_album = true;
    }
  if(AnsiContainsText(pattern, "<Comentario>"))
    { tags_especificadas++;
      tem_comentario = true;
    }

  if(AnsiContainsText(pattern, "<Numero>"))
    { tags_especificadas++;
      tem_numero = true;
    }
  /// Verifica se digitou ao menos uma tag no padrão //////////////////////////
  if(tags_especificadas==0)
   {
     // usuario nao especificou nenhuma tag => erro!!!
     Application->MessageBox("É necessário especificar ao menos uma tag no padrão." ,"Nenhuma tag especifica!",MB_OK|MB_ICONERROR);
     return;
   }
  // começa processo //////////////////////////////////////////////////////////
  Screen->Cursor = crHourGlass;   // exibe a ampulheta
  BotaoAplicarMp3->Enabled = false;        // desabilita botão
  if(Abrir_Previa())
   {
     CriaRecuperacao(); // Cria tabela de Recuperação  (Undo)
     int total = GridArquivos->Items->Count;      // nº total de arquivos
     // Prepara a Barra de Progressos //
     BarraProgresso->Visible=true;    //
     BarraProgresso->Min=0;           //
     BarraProgresso->Max=total;       //
     BarraProgresso->Step=1;          //
     //////////////////////////////////
     // opcoes do usuário ////////////////////////////////////////////////////
     bool onlyall = false;
     if(GrupoCondicaoTags->ItemIndex==0) onlyall = true;
     AnsiString branco = ""; // substitui tags inexistentes    (coringa)
     if(CaixaTextoBranco->Enabled) branco = CaixaTextoBranco->Text;
     ////////////////////////////////////////////////////////////////////////
     AnsiString arquivo;
     // Objeto ID3 //////////////////////////////////////////////////////////
     ID3 info;
     //Iteração /////////////////////////////////////////////////////////////
     for(int i=1; i<=total; i++)
     {
       int tags_encontradas = 0; // tags existentes no arquivo
       if(GridPrevia->Cells[2][i].LowerCase()=="mp3") // eh mp3
        {
         arquivo = GridPrevia->Cells[0][i];      // pega nome completo
         info.Initialize();
         int versao = info.AutoDetect(arquivo.c_str());
         if(versao>=12000)
         {
          info.SetVersion(versao);
          info.LoadID3(arquivo.c_str());
          AnsiString novo = pattern;
          // Pega tags ///////////////////////////////////////////////////////
          if(tem_titulo)
           {
             if(info.PegaTitulo()!="")
               { tags_encontradas++;
                 novo = AnsiReplaceText(novo,"<Titulo>",info.PegaTitulo());
               }
              else
                 novo = AnsiReplaceText(novo,"<Titulo>",branco);
           }
          if(tem_artista)
           {
             if(info.PegaArtista()!="")
               { tags_encontradas++;
                 novo = AnsiReplaceText(novo,"<Artista>",info.PegaArtista());
               }
              else
                 novo = AnsiReplaceText(novo,"<Artista>",branco);
           }
          if(tem_genero)
           {
             if(info.PegaGenero()!="")
               { tags_encontradas++;
                 novo = AnsiReplaceText(novo,"<Genero>",info.PegaGenero());
               }
              else
                 novo = AnsiReplaceText(novo,"<Genero>",branco);
           }
          if(tem_ano)
           {
             if(info.PegaAno()!="")
               { tags_encontradas++;
                 novo = AnsiReplaceText(novo,"<Ano>",info.PegaAno());
               }
              else
                 novo = AnsiReplaceText(novo,"<Ano>",branco);
           }
          if(tem_comentario)
           {
             if(info.PegaComentario()!="")
               { tags_encontradas++;
                 novo = AnsiReplaceText(novo,"<Comentario>",info.PegaComentario());
               }
              else
                 novo = AnsiReplaceText(novo,"<Comentario>",branco);
           }
          if(tem_album)
           {
             if(info.PegaAlbum()!="")
               { tags_encontradas++;
                 novo = AnsiReplaceText(novo,"<Album>",info.PegaAlbum());
               }
              else
                 novo = AnsiReplaceText(novo,"<Album>",branco);
           }
          if(tem_numero)
           {
             if(info.PegaNumero()!="")
               { tags_encontradas++;
                 novo = AnsiReplaceText(novo,"<Numero>",info.PegaNumero());
               }
              else
                 novo = AnsiReplaceText(novo,"<Numero>",branco);
           }


          ////////////////////////////////////////////////////////////////////
         if((!onlyall&&tags_encontradas>=1) || (tags_especificadas==tags_encontradas))
          {  // Renomeia
             switch(RadioRenomearInserir->ItemIndex)
               {
                 case 0: { GridPrevia->Cells[1][i] = novo;   // replace
                           break; }
                 case 1: { GridPrevia->Cells[1][i] = novo + GridPrevia->Cells[1][i];   // antes
                           break; }
                 case 2: { GridPrevia->Cells[1][i] = GridPrevia->Cells[1][i] + novo;   // append
                           break; }
               }  // fim switch
          } // fim if
          info.flush();        // limpa objeto id3
         }
        }   // fim if (eh Mp3??)
       BarraProgresso->StepIt();
     } // fim for
     /////////////////////////////////////////////////////////////////////////
    }   // fim if(abrirprevia)
  Screen->Cursor = crDefault;             // cursor normal
  BotaoAplicarMp3->Enabled=true;  // habilita botão
  BarraProgresso->Visible=false;  // oculta a barra de progresso
  PlaySound("SystemAsterisk", 0, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);
}
//---------------------------------------------------------------------------


void __fastcall TFormPrincipal::Teste1Click(TObject *Sender)
{
int posicao = CaixaPadrao->SelStart+1;
AnsiString nova = CaixaPadrao->Text;
if(CaixaPadrao->SelText!="")
{ int tam = CaixaPadrao->SelLength;
  nova  = CaixaPadrao->Text.Delete(posicao,tam);
}
  nova = nova.Insert("<Titulo>",posicao);
  CaixaPadrao->Text = nova;
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::Arista1Click(TObject *Sender)
{
int posicao = CaixaPadrao->SelStart+1;
AnsiString nova = CaixaPadrao->Text;
if(CaixaPadrao->SelText!="")
{ int tam = CaixaPadrao->SelLength;
  nova  = CaixaPadrao->Text.Delete(posicao,tam);
}
  nova = nova.Insert("<Artista>",posicao);
  CaixaPadrao->Text = nova;
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::Genero1Click(TObject *Sender)
{
int posicao = CaixaPadrao->SelStart+1;
AnsiString nova = CaixaPadrao->Text;
if(CaixaPadrao->SelText!="")
{ int tam = CaixaPadrao->SelLength;
  nova  = CaixaPadrao->Text.Delete(posicao,tam);
}
  nova = nova.Insert("<Genero>",posicao);
  CaixaPadrao->Text = nova;
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::Ano1Click(TObject *Sender)
{
int posicao = CaixaPadrao->SelStart+1;
AnsiString nova = CaixaPadrao->Text;
if(CaixaPadrao->SelText!="")
{ int tam = CaixaPadrao->SelLength;
  nova  = CaixaPadrao->Text.Delete(posicao,tam);
}
  nova = nova.Insert("<Ano>",posicao);
  CaixaPadrao->Text = nova;
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::Comentario1Click(TObject *Sender)
{
int posicao = CaixaPadrao->SelStart+1;
AnsiString nova = CaixaPadrao->Text;
if(CaixaPadrao->SelText!="")
{ int tam = CaixaPadrao->SelLength;
  nova  = CaixaPadrao->Text.Delete(posicao,tam);
}
  nova = nova.Insert("<Comentario>",posicao);
  CaixaPadrao->Text = nova;
}
//---------------------------------------------------------------------------


void __fastcall TFormPrincipal::CaixaPadraoKeyPress(TObject *Sender,
      char &Key)
{
  // bloquea caracteres proibidos em nomes de arquivos
  if(Key=='\\'||Key=='/'||Key=='*'||Key=='?'||Key=='"'||Key=='|')
    Key = 0; // elimina o stream
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::Album1Click(TObject *Sender)
{
int posicao = CaixaPadrao->SelStart+1;
AnsiString nova = CaixaPadrao->Text;
if(CaixaPadrao->SelText!="")
{ int tam = CaixaPadrao->SelLength;
  nova  = CaixaPadrao->Text.Delete(posicao,tam);
}
  nova = nova.Insert("<Album>",posicao);
  CaixaPadrao->Text = nova;
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::GrupoCondicaoTagsClick(TObject *Sender)
{
  if(GrupoCondicaoTags->ItemIndex==1)
   {
     RadioSeNaoExistir->Enabled = true;
   }
  else
   {
     RadioSeNaoExistir->Enabled = false;
     RadioSeNaoExistir->ItemIndex=0;
     CaixaTextoBranco->Enabled=false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::RadioSeNaoExistirClick(TObject *Sender)
{
  if(RadioSeNaoExistir->ItemIndex==1)
   {
     CaixaTextoBranco->Enabled=true;
   }
  else
   {
     CaixaTextoBranco->Enabled=false;
   }
}
//---------------------------------------------------------------------------


void __fastcall TFormPrincipal::CaixaTextoBrancoKeyPress(TObject *Sender,
      char &Key)
{
 // bloquea caracteres proibidos em nomes de arquivos
  if(Key=='\\'||Key=='/'||Key=='*'||Key=='?'||Key=='"'||Key=='<'||Key=='>'||Key=='|')
    Key = 0; // elimina o stream        
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::N1Click(TObject *Sender)
{
  Application->HelpFile = ExtractFilePath(ParamStr(0))+"ajuda.hlp";
  Application->HelpCommand(HELP_CONTENTS, 0);
}
//---------------------------------------------------------------------------


/****************************************************************************
* Habilita ou desabilita botões do grid
*****************************************************************************/
void TFormPrincipal::habilita_botoes()
{             // habilita ou desabilita botoes de aplicar a previa, conf. existem ou nao arquivos sel.
   if(GridArquivos->Items->Count==0)
     {
     Subs_Aplicar->Enabled = false;
     Ext_Aplicar->Enabled = false;
     Ext_AplicarLocalizarSubstituir->Enabled = false;
     Caract_Aplicar->Enabled = false;
     CaractLocalizarSubstituirAplicar->Enabled = false;
     Maisculas_Aplicar->Enabled = false;
     BotaoAplicarInserir->Enabled = false;
     BotaoPadraoAplicar->Enabled = false;
     BotaoAplicarMp3->Enabled = false;
     BotaoUP->Enabled = false;
     BotaoDOWN->Enabled = false;
     BotaoAPAGA->Enabled = false;
     BotaoLimpaTudo->Enabled = false;
     AplicarEliminarSubsequencia->Enabled = false;
     AplicarEliminarEspecial->Enabled = false;
     BotaoAplicarExpressaoRegular->Enabled = false;
     }
   else
     {
        Subs_Aplicar->Enabled = true;
        Ext_Aplicar->Enabled = true;
        Ext_AplicarLocalizarSubstituir->Enabled = true;
        Caract_Aplicar->Enabled = true;
        CaractLocalizarSubstituirAplicar->Enabled = true;
        Maisculas_Aplicar->Enabled = true;
        BotaoAplicarInserir->Enabled = true;
        BotaoPadraoAplicar->Enabled = true;
        BotaoAplicarMp3->Enabled = true;
        BotaoUP->Enabled = true;
        BotaoDOWN->Enabled = true;
        BotaoAPAGA->Enabled = true;
        BotaoLimpaTudo->Enabled = true;
        AplicarEliminarSubsequencia->Enabled = true;
        AplicarEliminarEspecial->Enabled = true;
        BotaoAplicarExpressaoRegular->Enabled = true;
     }
}

/****************************************************************************
* Radio Posição inicial
*****************************************************************************/
void __fastcall TFormPrincipal::RadioPosicaoInicialClick(TObject *Sender)
{
  switch(RadioPosicaoInicial->ItemIndex)
   {
     case 0:
        {
          CaractereInicial->Enabled = false;
          CaixaPosicaoInicial->Enabled = true;
          UpDownInicial->Enabled = true;
          break;
        }
     case 1:
        {
          CaractereInicial->Enabled = true;
          CaixaPosicaoInicial->Enabled = false;
          UpDownInicial->Enabled = false;
          break;
        }          
   }
}
//---------------------------------------------------------------------------

/****************************************************************************
* Radio posição final
*****************************************************************************/
void __fastcall TFormPrincipal::RadioPosicaoFinalClick(TObject *Sender)
{
switch(RadioPosicaoFinal->ItemIndex)
 {
     case 0:
        {
          CaractereFinal->Enabled = false;
          CaixaPosicaoFinal->Enabled = false;
          UpDownFinal->Enabled = false;
          break;
        }
     case 1:
        {
          CaractereFinal->Enabled = false;
          CaixaPosicaoFinal->Enabled = true;
          UpDownFinal->Enabled = true;
          break;
        }
     case 2:
        {
          CaractereFinal->Enabled = true;
          CaixaPosicaoFinal->Enabled = false;
          UpDownFinal->Enabled = false;
          break;
        }


 }
}
//---------------------------------------------------------------------------

/****************************************************************************
* Eliminar caractere especial
*****************************************************************************/
void __fastcall TFormPrincipal::AplicarEliminarEspecialClick(
      TObject *Sender)
{
  Screen->Cursor = crHourGlass;
  AplicarEliminarEspecial->Enabled=false;
  if(Abrir_Previa())
   {
     // Cria tabela de Recuperação
     CriaRecuperacao();
     AnsiString novo = Caract_Por->Text;
     int total = GridArquivos->Items->Count;
     // Prepara a Barra de Progressos
     BarraProgresso->Visible=true;
     BarraProgresso->Min=0;
     BarraProgresso->Max=total;
     BarraProgresso->Step=1;
     for(int i=1; i<=total; i++)
      {
          switch(RadioEliminarEspecial->ItemIndex)
           {
             case 0: // Trim
               {
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], " ", "");
                 break;
               }
             case 1: // remove espaços duplicados
               {
                 AnsiString novo = "";
                 TStringList *lista = GetTokens(GridPrevia->Cells[1][i]);
                 for(int i=0; i<lista->Count; i++) {
                   novo = novo + lista->Strings[i] + " ";
                 }
                 novo = novo.TrimLeft().TrimRight();
                 GridPrevia->Cells[1][i] = novo;
                 delete lista;
                 break;
               }
             case 2: // acentuação
               {
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "Á", "A");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "á", "a");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "É", "E");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "é", "e");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "Í", "I");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "í", "i");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "Ó", "O");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "ó", "o");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "Ú", "U");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "Ü", "U");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "ü", "u");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "Â", "A");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "â", "a");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "Ê", "E");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "ê", "e");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "Î", "I");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "î", "i");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "Ô", "O");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "ô", "o");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "Ç", "C");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "ç", "c");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "Ã", "A");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "ã", "a");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "Õ", "O");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "õ", "o");
                 break;
               }
             case 3:    // elimina numeros
               {
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "0", "");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "1", "");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "2", "");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "3", "");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "4", "");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "5", "");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "6", "");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "7", "");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "8", "");
                 GridPrevia->Cells[1][i] = AnsiReplaceStr(GridPrevia->Cells[1][i], "9", "");
                 break;
               }
           }
          BarraProgresso->StepIt();
      }
   }
  Screen->Cursor = crDefault;
  AplicarEliminarEspecial->Enabled=true;       // configs normais
  BarraProgresso->Visible=false;
  PlaySound("SystemAsterisk", 0, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);
}
//---------------------------------------------------------------------------

/****************************************************************************
* Elimina sequencia de caracteres
*****************************************************************************/
void __fastcall TFormPrincipal::AplicarEliminarSubsequenciaClick(
      TObject *Sender)
{
  Screen->Cursor = crHourGlass;
  AplicarEliminarSubsequencia->Enabled=false;
  if(Abrir_Previa())
   {
     // Cria tabela de Recuperação
     CriaRecuperacao();
     AnsiString novo = Caract_Por->Text;
     int total = GridArquivos->Items->Count;
     // Prepara a Barra de Progressos
     BarraProgresso->Visible=true;
     BarraProgresso->Min=0;
     BarraProgresso->Max=total;
     BarraProgresso->Step=1;
     int inicio, fim;
     for(int i=1; i<=total; i++)
      {
          switch(RadioPosicaoInicial->ItemIndex)
            {
              case 0:     // determinado
                {
                  inicio = StrToInt(CaixaPosicaoInicial->Text);
                  break;
                }
              case 1:                   // onde tiver o caractere
                {
                  inicio = GridPrevia->Cells[1][i].Pos(CaractereInicial->Text);
                  break;
                }
            }
          switch(RadioPosicaoFinal->ItemIndex)
            {
              case 0:           // final do nome
                {
                  fim = GridPrevia->Cells[1][i].Length();
                  break;
                }
              case 1:    // especificado
                {
                  fim = StrToInt(CaixaPosicaoFinal->Text);
                  break;
                }
              case 2:          // pos q contem o caractere
                {
                  fim = GridPrevia->Cells[1][i].Pos(CaractereFinal->Text);
                  break;
                }
            }
          fim = fim - inicio + 1;
          GridPrevia->Cells[1][i] = GridPrevia->Cells[1][i].Delete(inicio,fim);
          BarraProgresso->StepIt();
      }
     CaractereInicial->Text = "";
     CaractereFinal->Text = "";
   }
  Screen->Cursor = crDefault;
  AplicarEliminarSubsequencia->Enabled=true;       // configs normais
  BarraProgresso->Visible=false;
  PlaySound("SystemAsterisk", 0, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::LabelImportanteMouseEnter(TObject *Sender)
{
  LabelImportante->Font->Color = clBlack;
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::LabelImportanteMouseLeave(TObject *Sender)
{
   LabelImportante->Font->Color = clGray;        
}
//---------------------------------------------------------------------------


void __fastcall TFormPrincipal::Numero1Click(TObject *Sender)
{
int posicao = CaixaPadrao->SelStart+1;
AnsiString nova = CaixaPadrao->Text;
if(CaixaPadrao->SelText!="")
{ int tam = CaixaPadrao->SelLength;
  nova  = CaixaPadrao->Text.Delete(posicao,tam);
}
  nova = nova.Insert("<Numero>",posicao);
  CaixaPadrao->Text = nova;
}
//---------------------------------------------------------------------------


TStringList* TFormPrincipal::GetTokens(AnsiString texto)
{
   char* tokenPtr;
   TStringList* result = new TStringList();
   tokenPtr = strtok(texto.c_str()," ");
   while( tokenPtr != NULL ) {
       result->Append(tokenPtr);
       tokenPtr = strtok(NULL, " ");
   }
   return result;
}
void __fastcall TFormPrincipal::BotaoCarregartemplatesClick(
      TObject *Sender)
{
  FormTemplates->ShowModal();        
}
//---------------------------------------------------------------------------


void __fastcall TFormPrincipal::BotaoAplicarExpressaoRegularClick(
      TObject *Sender)
{
  if((CaixaExpressao->Text).Trim()=="") return;
  Screen->Cursor = crHourGlass;
  BotaoAplicarExpressaoRegular->Enabled=false;
  if(Abrir_Previa())
   {
     // Cria tabela de Recuperação
     CriaRecuperacao();
     int total = GridArquivos->Items->Count;
     // Prepara a Barra de Progressos
     BarraProgresso->Visible=true;
     BarraProgresso->Min=0;
     BarraProgresso->Max=total;
     BarraProgresso->Step=1;
     ER->Expression = CaixaExpressao->Text;
     ER->FormatString = CaixaSubstituirPor->Text;
     for(int i=1; i<=total; i++)
     {
       ER->StringToMatch = GridPrevia->Cells[1][i];
       AnsiString st;
       ER->Merge(st);
       ER->StringToMatch = GridPrevia->Cells[1][i];
       if(ER->Grep() > 0)
         GridPrevia->Cells[1][i] = st;
       BarraProgresso->StepIt();
     }
    }
  Screen->Cursor = crDefault;
  BotaoAplicarExpressaoRegular->Enabled=true;
  BarraProgresso->Visible=false;
  PlaySound("SystemAsterisk", 0, SND_ALIAS | SND_NODEFAULT | SND_ASYNC);        
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::CheckApenasPrimeiraClick(TObject *Sender)
{
FormTemplates->AtualizaOpcoesExpressoes();           
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::BotaoSalvarExpressaoClick(TObject *Sender)
{
  if(DlgSalvarRegex->Execute()) {
    AnsiString nome = DlgSalvarRegex->FileName;
    TIniFile *ini = new TIniFile(nome);
    ini->WriteString("Expressao Regular","Expressao", CaixaExpressao->Text );
    ini->WriteString("Expressao Regular","SubstituirPor", CaixaSubstituirPor->Text );
    ini->WriteBool("Expressao Regular","LocalizarApenasPrimeira", CheckApenasPrimeira->Checked );
    ini->WriteBool("Expressao Regular","LocalizarMenorOcorrencia", CheckMenor->Checked);
    ini->WriteBool("Expressao Regular","ManterRestoInalterado",CheckManter->Checked );
    ini->WriteBool("Expressao Regular","CaseSensitive",CheckCase->Checked  );
    if(AnsiContainsText(nome, "templates")) {
      AnsiString descricao = InputBox("Descrição da Expressão Regular", "Expressões salvas como template devem ter uma descrição:", "Expressão do usuário");
      ini->WriteString("Expressao Regular","Descricao", descricao );
    }
    delete ini;
  }        
}
//---------------------------------------------------------------------------

void __fastcall TFormPrincipal::BotaoAbrirExpressaoClick(TObject *Sender)
{
  if(DlgAbrirRegex->Execute()) {
    FormTemplates->SelecionaExpressao(DlgAbrirRegex->FileName);
  }        
}
//---------------------------------------------------------------------------
void __fastcall TFormPrincipal::WMDropFiles(TWMDropFiles &message)
{
  if(!iniciado)
    {

      Screen->Cursor = crHourGlass;
      // Ordena
      ColunaOrdenar = ColunaAnterior = 4;
      RadioOrdem->ItemIndex=0;
      GridArquivos->AlphaSort(); // quicksort
      Screen->Cursor = crHourGlass;      // ampulheta
      TListItem  *ListItem;      // usado para inserir no TListView
      AnsiString extensao;       // guarda extensão
      AnsiString nome;           // guarda somente o nome
      float tamanho;             // tamanho do arquivo em KBytes
      long file_size;            // usado para ler em Bytes o tam do arquivo
      int fp;                    // flag da função open
      // Tamando da Lista antes das novas inserções
      int tamanho_antes = GridArquivos->Items->Count;
      UINT Count = DragQueryFile( (HDROP)message.Drop, 0xFFFFFFFF, NULL, MAX_PATH);
      // varre todos os arquivos passados
      for(UINT i=0; i<Count; ++i)
        {
          AnsiString arquivo ;
          arquivo.SetLength (MAX_PATH) ;
          int length = DragQueryFile ((HDROP) message.Drop, i, arquivo.c_str (), arquivo.Length ()) ;
          arquivo.SetLength (length) ;
          if(DirectoryExists(arquivo)) continue;
          // procura na lista GridArquivos
          if(!Busca_Bin(GridArquivos,arquivo,4,tamanho_antes))
             { // se não esta em GridArquivos então insere
               // Pega tamanho do arquivo
               fp = open(arquivo.c_str(), O_RDONLY|O_BINARY);
               if (fp == -1) // algum erro na leitura do arq
                 {
                   Screen->Cursor = crDefault; // cursor normal
                   AnsiString Erro;
                   Erro = (AnsiString)"O arquivo:\n"+arquivo+(AnsiString)"\nnão pôde ser aberto. O arquivo pode estar corrompido ou sendo usado por outra aplicação.";
                   Application->MessageBox(Erro.c_str() ,"Erro no arquivo.",MB_OK|MB_ICONERROR);
                   Screen->Cursor = crHourGlass; // ampulheta
                   continue;  // aborta arquivo
                 }
               else
                 {
                    file_size = filelength(fp);   // pega tamanho
                    close(fp);  // fecha o arq
                 }
               tamanho = file_size/1024;
               if(tamanho == 0) tamanho = 1; // um arquivo ocupa no mínimo 1 cluster (4096bytes)
               // Insere no TListView (GridArquivos)
               ListItem = GridArquivos->Items->Add(); // pega referência
               nome = ExtractFileName(arquivo); // pega nome e ext
               extensao = ExtractFileExt(arquivo);
               nome = nome.SubString(1, nome.Length()-extensao.Length()) ; // retira ext
               ListItem->Caption = nome;    // atribui nome
               extensao = extensao.SubString(2,extensao.Length()) ; // retira "."
               ListItem->SubItems->Add(extensao);
               ListItem->SubItems->Add(FileDateToDateTime(FileAge(arquivo)).DateString()); // converte formato usual
               ListItem->SubItems->Add(FloatToStrF(tamanho,ffFixed,5,2)); // coloca dupla precisão
               ListItem->SubItems->Add(arquivo); // path completo
             } // fim se
         } // fim for
       // Atualiza Total
       LabelTotal->Caption = (AnsiString)"Total: " + (AnsiString)GridArquivos->Items->Count + (AnsiString)" arquivo(s)."   ;
       ColunaOrdenar = ColunaAnterior = 0;
       RadioOrdem->ItemIndex=0;
       GridArquivos->AlphaSort();  // quicksort
       // Habilita ou desabilita botões de aplicar à prévia
       habilita_botoes();
       Screen->Cursor = crDefault;   // cursor normal

    }

  DragFinish( (HDROP)message.Drop);
}

