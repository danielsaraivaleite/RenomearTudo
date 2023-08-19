object FormTemplates: TFormTemplates
  Left = 345
  Top = 274
  BorderStyle = bsDialog
  Caption = 'Express'#245'es Regulares - Templates'
  ClientHeight = 302
  ClientWidth = 449
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Lista: TListView
    Left = 0
    Top = 0
    Width = 449
    Height = 249
    Align = alTop
    Columns = <
      item
        Caption = 'Template'
        MinWidth = 120
        Width = 120
      end
      item
        Caption = 'Descri'#231#227'o'
        MinWidth = 300
        Width = 300
      end>
    ColumnClick = False
    GridLines = True
    ReadOnly = True
    RowSelect = True
    TabOrder = 0
    ViewStyle = vsReport
    OnDblClick = ListaDblClick
  end
  object BotaoOK: TButton
    Left = 24
    Top = 264
    Width = 89
    Height = 25
    Caption = '&Cancelar'
    TabOrder = 1
    OnClick = BotaoOKClick
  end
  object BotaoSelecionar: TButton
    Left = 342
    Top = 264
    Width = 83
    Height = 25
    Caption = '&Selecionar'
    TabOrder = 2
    OnClick = BotaoSelecionarClick
  end
  object DirList: TFileListBox
    Left = 160
    Top = 264
    Width = 145
    Height = 25
    ItemHeight = 13
    Mask = '*.rgx'
    TabOrder = 3
    Visible = False
  end
end
