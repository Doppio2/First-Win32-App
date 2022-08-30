call plug#begin()

Plug 'morhetz/gruvbox' " Theme

" Syntax highlighting
Plug 'bfrg/vim-cpp-modern' " C++ syntax
Plug 'vim-python/python-syntax' " Python syntax

Plug 'nathanaelkane/vim-indent-guides'

call plug#end()

set nobackup		" do not keep a backup file
set nowritebackup
set autoindent		" always set autoindenting on
set autoread		" Load changed files without alerting us all the time

if has('mouse')
    set mouse=a
endif

set expandtab
set tabstop=4
set shiftwidth=4
set nowrap
set hlsearch
set splitbelow
set splitright
set scrolloff=5 	 " Scroll vertially before hitting the edges of the window
set sidescrolloff=10 " Scroll horizontally before hitting the edges of the window
set ignorecase		 " Make search case-insensitive by default

set number relativenumber  

source $VIMRUNTIME/mswin.vim
behave mswin

language en_US

" Write files before many operations
set autowrite
" … but keep the undo buffer when we switch files, otherwise autowrite can
" cause some nasty data loss
set hidden

set background=dark
colorscheme epitaph

" Inden-guides
let g:indent_guides_auto_colors = 0 
let g:indent_guides_space_guides = 1
let g:indent_guides_enable_on_vim_startup = 1
hi IndentGuidesOdd  guibg=#1E1E1E ctermbg=235
hi IndentGuidesEven guibg=#282828 ctermbg=236

set guifont=Liberation\ Mono:h11

" Disable leader timeout
set notimeout
set ttimeout

" Key Bindins
" Bindings for window's
nnoremap <M-s> :w<CR>
nnoremap <M-w> <C-w>
nnoremap <M-1> :vsplit<CR>
nnoremap <M-2> :wq<CR>

" Bindings for search in file's
nnoremap <M-/> :noh<CR>

" Bindings for buffers
nnoremap <M-k> :bdelete<CR>

" Autoread file with external changes
nnoremap <C-u> :checktime<CR>

"Disable bold font
function! Highlight_remove_attr(attr)
    " save selection registers
    new
    silent! put

    " get current highlight configuration
    redir @x
    silent! highlight
    redir END
    " open temp buffer
    new
    " paste in
    silent! put x

    " convert to vim syntax (from Mkcolorscheme.vim,
    "   http://vim.sourceforge.net/scripts/script.php?script_id=85)
    " delete empty,"links" and "cleared" lines
    silent! g/^$\| links \| cleared/d
    " join any lines wrapped by the highlight command output
    silent! %s/\n \+/ /
    " remove the xxx's
    silent! %s/ xxx / /
    " add highlight commands
    silent! %s/^/highlight /
    " protect spaces in some font names
    silent! %s/font=\(.*\)/font='\1'/

    " substitute bold with "NONE"
    execute 'silent! %s/' . a:attr . '\([\w,]*\)/NONE\1/geI'
    " yank entire buffer
    normal ggVG
    " copy
    silent! normal "xy
    " run
    execute @x

    " remove temp buffer
    bwipeout!

    " restore selection registers
    silent! normal ggVGy
    bwipeout!
endfunction
autocmd BufNewFile,BufRead * call Highlight_remove_attr("bold")
