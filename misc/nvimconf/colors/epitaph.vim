highlight clear 
if exists("syntax_on") 
 syntax reset 
endif

set background=dark

" Comment
highlight Comment guifg=#7D7D7D
" Bg and fg
highlight Normal guibg=#232323 guifg=#A6978A

" Constant
highlight Constant guifg=#BFBFBF
highlight String guifg=#BFBFBF
highlight Character guifg=#BFBFBF
highlight Number guifg=#BFBFBF
highlight Boolean guifg=#BFBFBF
highlight Float guifg=#BFBFBF

" Identifier
highlight Identifier guifg=#A6978A
highlight Function guifg=#7FBF54

" Statemant
highlight Statement guifg=#D95B5B
highlight Conditional guifg=#D95B5B
highlight Repeat guifg=#D95B5B
highlight Label guifg=#D95B5B
highlight Operator guifg=#D95B5B
highlight Keyword guifg=#D95B5B
highlight Exception guifg=#D95B5B

" PreProc
highlight PreProc guifg=#D8A51D
highlight Define guifg=#D8A51D
highlight Macro guifg=#D8A51D
highlight PreCondit guifg=#D8A51D
highlight Include guifg=#D8A51D

" Type
highlight Type guifg=#56A69C
highlight StorageClass guifg=#56A69C
highlight Structure guifg=#56A69C
highlight Typedef guifg=#56A69C

"Special
highlight Special guifg=#A6978A
highlight SpecialChar guifg=#A6978A
highlight Tag guifg=#A6978A
highlight Delimiter guifg=#A6978A
highlight SpecialComment guifg=#A6978A
highlight Debug guifg=#A6978A

" Else stuff
highlight Todo guifg=#b4ff55 guibg=#1F1F1F 

" UI
highlight StatusLine guibg=#DBDBDB guifg=#A6978A
highlight StatusLineNC guibg=#DBDBDB guifg=#7D7D7D
highlight LineNr guifg=#A6978A
highlight NonText guifg=#A6978A
highlight VertSplit guifg=#232323 guibg=#232323 
highlight IncSearch guibg=#A6978A guibg=#49483E
hi Search guibg=#49483E guifg=#A6978A

highlight PMenu guibg=#49483E guifg=#A6978A
highlight PMenuSel guibg=#7FBF54 guifg=#A6978A

highlight Cursor guifg=#49483E guibg=#7FBF54 
highlight iCursor guifg=#49483E guibg=#D95B5B
set guicursor=n-v-c:block-Cursor
set guicursor+=i:ver100-iCursor
set guicursor+=n-v-c:blinkon0
set guicursor+=i:blinkwait10
