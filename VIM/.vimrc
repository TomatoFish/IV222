set nocompatible              " be iMproved, required
filetype off                  " required
set t_Co=256
set encoding=utf-8
set mouse=a
" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
Plugin 'scrooloose/nerdtree'
Plugin 'Mizuchi/STL-Syntax'
Plugin 'bling/vim-airline'
Plugin 'majutsushi/tagbar'

"Plugin 'flazz/vim-colorschemes'
"Plugin 'scrooloose/syntastic'
"Plugin 'Valloric/YouCompleteMe'

Plugin 'tpope/vim-fugitive'
Plugin 'airblade/vim-gitgutter'
Plugin 'SublimeText/CTags'
Plugin 'tpope/vim-surround'
Plugin 'davidhalter/jedi-vim'
Plugin 'morhetz/gruvbox'
call vundle#end() " required
filetype plugin indent on    " required

let g:airline#extensions#tabline#enabled = 1
let g:airline_left_sep = '▶'
let g:airline_right_sep = '◀'

nmap <F8> :TagbarToggle<CR>
nmap <F2> :NERDTree<CR>
nnoremap <buffer> <F5> :exec '!python' shellescape(@%, 1)<cr>
set background=dark
colorscheme gruvbox
