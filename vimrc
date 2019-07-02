 "设置外观 ------------------------------------------------------------------------
set number                      "显示行号 
"set showtabline=0               "隐藏顶部标签栏"
 set guioptions-=r               "隐藏右侧滚动条" 
"set guioptions-=L               "隐藏左侧滚动条"
"set guioptions-=b               "隐藏底部滚动条"
set cursorline                  "突出显示当前行"
set cursorcolumn                "突出显示当前列"
set langmenu=zh_CN.UTF-8        "显示中文菜单
set guifont=Monaco\ 18
set guifont=Monaco:h18

" 编程辅助 -----------------------------------------------------------------------
syntax enable 
syntax on                           "开启语法高亮
colorscheme desert
au filetype c,cpp set cindent
au filetype py set autoindent 
let g:syntastic_cpp_compiler = 'g++' 
let g:syntastic_cpp_compiler_options = '-std=c++11 -stdlib=libc++'

set nowrap                      "设置代码不折行"
set fileformat=unix             "设置以unix的格式保存文件"
set cindent                     "设置C样式的缩进格式"
set tabstop=4                   "一个 tab 显示出来是多少个空格，默认 8
set shiftwidth=4                "每一级缩进是多少个空格
set backspace+=indent,eol,start "set backspace&可以对其重置
set showmatch                   "显示匹配的括号"
set scrolloff=5                 "距离顶部和底部5行"
set laststatus=2                "命令行为两行"

func! CompileGpp()
    exec "w"
    let compilecmd="!g++ "
    let compileflag="-std=c++11 -o %< "
    if search("mpi\.h") != 0
        let compilecmd = "!mpic++ "
    endif
    if search("glut\.h") != 0
        let compileflag .= " -lglut -lGLU -lGL "
    endif
    if search("cv\.h") != 0
        let compileflag .= " -lcv -lhighgui -lcvaux "
    endif
    if search("omp\.h") != 0
        let compileflag .= " -fopenmp "
    endif
    if search("math\.h") != 0
        let compileflag .= " -lm "
    endif
    exec compilecmd." % ".compileflag
endfunc 

  func! CompileCode()
        exec "w"
        if &filetype == "cpp"
                exec "call CompileGpp()"
        elseif &filetype == "c"
                exec "call CompileGcc()"
        endif
      endfunc 
" map <F5> :call CompileCode()<CR>
" imap <F5> <ESC>:call CompileCode()<CR>
" vmap <F5> <ESC>:call CompileCode()<CR>

map <F5> :call CompileRunGcc()<CR>
func! CompileRunGcc()
	exec "w"
	if &filetype == 'c'
		exec "!g++ % -o %<"
		exec "!time ./%<"
	elseif &filetype == 'cpp'
		exec "!g++ -std=c++11 *.cpp -o %<"
		exec "!time ./%<"
	elseif &filetype == 'java' 
		exec "!javac %" 
		exec "!time java %<"
	elseif &filetype == 'sh'
		:!time bash %
	elseif &filetype == 'python'
		exec "!time python2.7 %"
    elseif &filetype == 'html'
        exec "!firefox % &"
    elseif &filetype == 'go'
        exec "!go build %<"
        exec "!time go run %"
    elseif &filetype == 'mkd'
        exec "!~/.vim/markdown.pl % > %.html &"
        exec "!firefox %.html &"
	endif
endfunc

" 添加文件头 -----------------------------------------------------------------------
"新建.c,.h,.sh,.java文件，自动插入文件头 
autocmd BufNewFile *.cpp,*.[ch],*.sh,*.java exec ":call SetTitle()" 
""定义函数SetTitle，自动插入文件头 
func SetTitle() 
	"如果文件类型为.sh文件 
	if &filetype == 'sh' 
		call setline(1, "##########################################################################") 
		call append(line("."), "# File Name: ".expand("%")) 
		call append(line(".")+1, "# Author: 悟空") 
		call append(line(".")+2, "# mail: wusong220@outlook.com") 
		call append(line(".")+3, "# Created Time: ".strftime("%c")) 
		call append(line(".")+4, "#########################################################################/") 
		call append(line(".")+5, "#!/bin/zsh")
		call append(line(".")+6, "PATH=/home/edison/bin:/home/edison/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/work/tools/gcc-3.4.5-glibc-2.3.6/bin")
		call append(line(".")+7, "export PATH")
		call append(line(".")+8, "")
	else 
		call setline(1, "/*************************************************************************") 
		call append(line("."), "	> File Name: ".expand("%")) 
		call append(line(".")+1, "	> Author: 悟空") 
		"call append(line(".")+2, "	> Mail: wusong220@outlook.com ") 
		"call append(line(".")+3, "	> Created Time: ".strftime("%c")) 
		call append(line(".")+4, " ************************************************************************/") 
		call append(line(".")+5, "")
	endif
	if &filetype == 'cpp'
		call append(line(".")+6, "#include<iostream>")
    	call append(line(".")+7, "using namespace std;")
		call append(line(".")+8, "")
	endif
	if &filetype == 'c'
		call append(line(".")+6, "#include<stdio.h>")
		call append(line(".")+7, "")
	endif
	"	if &filetype == 'java'
	"		call append(line(".")+6,"public class ".expand("%"))
	"		call append(line(".")+7,"")
	"	endif
	"新建文件后，自动定位到文件末尾
	autocmd BufNewFile * normal G
endfunc 
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"键盘命令
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
 
nmap <leader>w :w!<cr>
nmap <leader>f :find<cr>
 
" 映射全选+复制 ctrl+a
map <C-A> ggVGY
map! <C-A> <Esc>ggVGY
map <F12> gg=G
" 选中状态下 Ctrl+c 复制
vmap <C-c> "+y


" 其他杂项 -------------------------------------------------------------------------------
set mouse=a                     "启用鼠标"
set selection=exclusive
set selectmode=mouse,key
set matchtime=5
set ignorecase                  "忽略大小写"
set incsearch
set hlsearch                    "高亮搜索项"
set noexpandtab                 "不允许扩展table"
set whichwrap+=<,>,h,l
set autoread

" 复制------------------------------------------------------------------------------------
"使用vim的键盘模式
set nocompatible

"不需要备份
set nobackup

"没有保存或文件只读时弹出确认
set confirm

"鼠标可用
set mouse=a

"tab缩进
set tabstop=4
set shiftwidth=4
set expandtab
set smarttab

"文件自动检测外部更改
set autoread

"c文件自动缩进
set cindent

"自动对齐
set autoindent

"智能缩进
set smartindent

"高亮查找匹配
set hlsearch

"显示匹配
set showmatch

"显示标尺，就是在右下角显示光标位置
set ruler

"去除vi的一致性
"set nocompatible

"设置键盘映射，通过空格设置折叠
nnoremap <space> @=((foldclosed(line('.')<0)?'zc':'zo'))<CR>
""""""""""""""""""""""""""""""""""""""""""""""
"不要闪烁
"set novisualbell

"启动显示状态行
set laststatus=2

"浅色显示当前行
autocmd InsertLeave * se nocul

"用浅色高亮当前行
autocmd InsertEnter * se cul

"显示输入的命令
set showcmd

"被分割窗口之间显示空白
set fillchars=vert:/
set fillchars=stl:/
set fillchars=stlnc:/
" 配色方案
set background=dark
colorscheme solarized
"colorscheme molokai
"colorscheme phd

" 禁止显示菜单和工具条
set guioptions-=m
set guioptions-=T

" 总是显示状态栏
set laststatus=2

" 禁止折行
set nowrap

" 设置状态栏主题风格
let g:Powerline_colorscheme='solarized256'

syntax keyword cppSTLtype initializer_list

" 基于缩进或语法进行代码折叠
"set foldmethod=indent
set foldmethod=syntax
" 启动 vim 时关闭折叠代码
set nofoldenable

"允许用退格键删除字符
set backspace=indent,eol,start

"编码设置
set encoding=utf-8

"共享剪切板
set clipboard=unnamed

" 分割布局 ------------------------------------------------------------------------------------
" 指定屏幕上可以进行分割布局的区域
set splitbelow               " 允许在下部分割布局
set splitright               " 允许在右侧分隔布局

" 组合快捷键：
nnoremap <C-J> <C-W><C-J>    " 组合快捷键：- Ctrl-j 切换到下方的分割窗口
nnoremap <C-K> <C-W><C-K>    " 组合快捷键：- Ctrl-k 切换到上方的分割窗口
nnoremap <C-L> <C-W><C-L>    " 组合快捷键：- Ctrl-l 切换到右侧的分割窗口
nnoremap <C-H> <C-W><C-H>    " 组合快捷键：- Ctrl-h 切换到左侧的分割窗口

"配置Vundle ---------------------------------------------------------------------------------------
set nocompatible              " required
filetype off                  " required

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" alternatively, pass a path where Vundle should install plugins
"call vundle#begin('~/some/path/here')
" let Vundle manage Vundle, required

" 插件目录-----------------------------------------------------------------------------------------
Plugin 'gmarik/Vundle.vim'
"Plugin 'scrooloose/nerdtree' 
"Plugin 'Xuyuanp/nerdtree-git-plugin'  


" Add all your plugins here (note older versions of Vundle used Bundle instead of Plugin)

"添加目录树 ---------------------------------------
Plugin 'scrooloose/nerdtree'
"配置NERDTree
"使用F3键快速调出和隐藏它
map <F2> :NERDTreeToggle<CR>

let NERDTreeChDirMode=1

"显示书签"
let NERDTreeShowBookmarks=1

"设置忽略文件类型"
let NERDTreeIgnore=['\~$', '\.pyc$', '\.swp$']

"窗口大小"
let NERDTreeWinSize=25

" 修改默认箭头
let g:NERDTreeDirArrowExpandable = '▸'
let g:NERDTreeDirArrowCollapsible = '▾'

"How can I open a NERDTree automatically when vim starts up if no files were specified?
autocmd StdinReadPre * let s:std_in=1
autocmd VimEnter * if argc() == 0 && !exists("s:std_in") | NERDTree | endif

" 打开vim时自动打开NERDTree
"autocmd vimenter * NERDTree           

"How can I open NERDTree automatically when vim starts up on opening a directory?
autocmd StdinReadPre * let s:std_in=1
autocmd VimEnter * if argc() == 1 && isdirectory(argv()[0]) && !exists("s:std_in") | exe 'NERDTree' argv()[0] | wincmd p | ene | endif

" 关闭vim时，如果打开的文件除了NERDTree没有其他文件时，它自动关闭，减少多次按:q!
autocmd bufenter * if (winnr("$") == 1 && exists("b:NERDTree") && b:NERDTree.isTabTree()) | q | endif

" 开发的过程中，我们希望git信息直接在NERDTree中显示出来， 和Eclipse一样，修改的文件和增加的文件都给出相应的标注， 这时需要安装的插件就是 nerdtree-git-plugin,配置信息如下
let g:NERDTreeIndicatorMapCustom = {
    \ "Modified"  : "✹",
    \ "Staged"    : "✚",
    \ "Untracked" : "✭",
    \ "Renamed"   : "➜",
    \ "Unmerged"  : "═",
    \ "Deleted"   : "✖",
    \ "Dirty"     : "✗",
    \ "Clean"     : "✔︎",
    \ "Unknown"   : "?"
    \ }

" 显示行号
let NERDTreeShowLineNumbers=1

let NERDTreeAutoCenter=1

" 在终端启动vim时，共享NERDTree
let g:nerdtree_tabs_open_on_console_startup=1

"添加git状态图标---------------------------------------
Plugin 'Xuyuanp/nerdtree-git-plugin' 

"配色方案---------------------------------------------
Plugin 'jakwings/vim-colors'
" 推荐几种配色，使用哪种只需将其取消注释即可

" 经典的solarized
colorscheme solarized               " 白色背景

" Tomorrow系列
"colorscheme Tomorrow               " 白色背景
"colorscheme Tomorrow-Night         " 黑色背景,序号看不清
"colorscheme Tomorrow-Night-Bright      " 黑色背景,高对比度，最清晰
"colorscheme Tomorrow-Night-Eighties        " 黑色背景,低对比度

" 其他
"colorscheme desert
"colorscheme molokai
"colorscheme pablo
"colorscheme phd
"colorscheme pablo
"colorscheme slate
syntax enable
set background=dark
colorscheme solarized

"美化状态栏------------------------------------------
"Plugin 'vim-powerline'
" 将字体设置为Meslo LG S DZ Regular for Powerline 13号大小
set guifont=Meslo\ LG\ S\ DZ\ Regular\ for\ Powerline:h13

let g:Powerline_symbols = 'fancy'       " Powerline_symbols为状态栏中的箭头，unicode没有箭头
"let g:Powerline_symbols= 'unicode'

set laststatus=2                " 必须设置为2,否则状态栏不显示
set t_Co=256                    " 开启256颜色之后，colorschema在vim里好看了许多
let g:Powerline_colorscheme='solarized256'  " 状态栏使用了solarized256配色方案

"set fillchars+=stl:\ ,stlnc:\          " 状态栏如果有\\\\\或^^^^^等符号出现，添加此句再删掉此句就好了

map <C-F12> :!ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .<CR>
" 安装taglist ----------------------------------------
Plugin 'taglist.vim'
" 配置taglist
let Tlist_Use_Right_Window = 1          "让taglist窗口出现在Vim的右边

let Tlist_File_Fold_Auto_Close = 1      "当同时显示多个文件中的tag时，设置为1，可使taglist只显示当前文件tag，其它文件的tag都被折叠起来。

let Tlist_Show_One_File = 1             "只显示一个文件中的tag，默认为显示多个

let Tlist_Sort_Type ='name'             "Tag的排序规则，以名字排序。默认是以在文件中出现的顺序排序

let Tlist_GainFocus_On_ToggleOpen = 1       "Taglist窗口打开时，立刻切换为有焦点状态

let Tlist_Exit_OnlyWindow = 1           "如果taglist窗口是最后一个窗口，则退出vim

let Tlist_WinWidth = 32             "设置窗体宽度为32，可以根据自己喜好设置

let Tlist_Ctags_Cmd='/usr/local/bin/ctags'  "这里比较重要了，设置ctags的位置，不是指向MacOS自带的那个，而是我们用homebrew安装的那个

map t :TlistToggle<CR>              "热键设置，我设置成Leader+t来呼出和关闭Taglist

"添加代码缩进指示线---------------------------------------
Plugin 'Yggdroot/indentLine'
"配置indentLine
" 支持任意ASCII码，也可以使用特殊字符：¦, ┆, or │ ，但只在utf-8编码下有效
let g:indentLine_char='¦'

" 使indentline生效
let g:indentLine_enabled = 1

"代码折叠-------------------------------------------------
Plugin 'tmhedberg/SimpylFold'
" 必须手动输入za来折叠（和取消折叠）
set foldmethod=indent                " 根据每行的缩进开启折叠
set foldlevel=99

" 使用空格键会是更好的选择,所以在你的配置文件中加上这一行命令吧：
nnoremap <space> za

" 希望看到折叠代码的文档字符串？
let g:SimpylFold_docstring_preview=1

" 代码补全--------------------------------------------
Plugin 'Valloric/YouCompleteMe'

let g:ycm_global_ycm_extra_conf='~/.ycm_extra_conf.py'
let g:ycm_confirm_extra_conf = 0

" 补全菜单的开启与关闭
set completeopt=longest,menu                    " 让Vim的补全菜单行为与一般IDE一致(参考VimTip1228)
let g:ycm_min_num_of_chars_for_completion=2             " 从第2个键入字符就开始罗列匹配项
let g:ycm_cache_omnifunc=0                      " 禁止缓存匹配项,每次都重新生成匹配项
let g:ycm_autoclose_preview_window_after_completion=1       " 智能关闭自动补全窗口
autocmd InsertLeave * if pumvisible() == 0|pclose|endif         " 离开插入模式后自动关闭预览窗口

" 补全菜单中各项之间进行切换和选取：默认使用tab  s-tab进行上下切换，使用空格选取。可进行自定义设置：
"let g:ycm_key_list_select_completion=['<c-n>']
"let g:ycm_key_list_select_completion = ['<Down>']      " 通过上下键在补全菜单中进行切换
"let g:ycm_key_list_previous_completion=['<c-p>']
"let g:ycm_key_list_previous_completion = ['<Up>']
inoremap <expr> <CR>       pumvisible() ? "\<C-y>" : "\<CR>"    " 回车即选中补全菜单中的当前项

" 开启各种补全引擎
let g:ycm_collect_identifiers_from_tags_files=1         " 开启 YCM 基于标签引擎
let g:ycm_auto_trigger = 1                  " 开启 YCM 基于标识符补全，默认为1
let g:ycm_seed_identifiers_with_syntax=1                " 开启 YCM 基于语法关键字补全
let g:ycm_complete_in_comments = 1              " 在注释输入中也能补全
let g:ycm_complete_in_strings = 1               " 在字符串输入中也能补全
let g:ycm_collect_identifiers_from_comments_and_strings = 0 " 注释和字符串中的文字也会被收入补全

" 重映射快捷键
"上下左右键的行为 会显示其他信息,inoremap由i 插入模式和noremap不重映射组成，只映射一层，不会映射到映射的映射
inoremap <expr> <Down>     pumvisible() ? "\<C-n>" : "\<Down>"
inoremap <expr> <Up>       pumvisible() ? "\<C-p>" : "\<Up>"
inoremap <expr> <PageDown> pumvisible() ? "\<PageDown>\<C-p>\<C-n>" : "\<PageDown>"
inoremap <expr> <PageUp>   pumvisible() ? "\<PageUp>\<C-p>\<C-n>" : "\<PageUp>"

"nnoremap <F5> :YcmForceCompileAndDiagnostics<CR>           " force recomile with syntastic
"nnoremap <leader>lo :lopen<CR>    "open locationlist
"nnoremap <leader>lc :lclose<CR>    "close locationlist
"inoremap <leader><leader> <C-x><C-o>

nnoremap <leader>jd :YcmCompleter GoToDefinitionElseDeclaration<CR> " 跳转到定义处
let g:ycm_confirm_extra_conf=0                  " 关闭加载.ycm_extra_conf.py确认提示
"let g:ycm_global_ycm_extra_conf = '~/.ycm_extra_conf.py'

" 对符号补全------------------------------------
Plugin 'jiangmiao/auto-pairs'

" 多行注释--------------------------------------
Plugin 'scrooloose/nerdcommenter'
" nerdcommenter默认热键<leader>为'\'，这里将热键设置为','
let mapleader=','

" 设置注释快捷键
map <F4> <leader>ci<CR>

" Markdown 插件--------------------------------
Plugin 'godlygeek/tabular'
Plugin 'plasticboy/vim-markdown'
Plugin 'suan/vim-instant-markdown'

" Python 插件------------------------------------
" Plugin 'isnowfy/python-vim-instant-markdown'
" 执行:Instantmd 命令打开浏览器窗口实时预览,否则访问http://localhost:7000/

" 其他插件-------------------------------------
Plugin 'altercation/vim-colors-solarized'
Plugin 'tomasr/molokai'
Plugin 'vim-scripts/phd'

" 状态栏------------------------------------------
Plugin 'Lokaltog/vim-powerline'
set guifont=DejaVu\ Sans\ Mono\ for\ Powerline:h18
set rtp+=/anaconda2/lib/python2.7/site-packages/powerline/bindings/bash/powerline.sh
let g:Powerline_symbols = 'fancy'
set encoding=utf-8 
set laststatus=2
set nocompatible
set t_Co=256

let g:minBufExplForceSyntaxEnable = 1

set laststatus=2 
set guifont=Source\ Code\ Pro\ for\ Powerline:h12 
"set noshowmode

" -----------------------------------------------------------------
Plugin 'octol/vim-cpp-enhanced-highlight'
Plugin 'Raimondi/delimitMate'

"tagbar ------------------------------------------------------------
Plugin 'majutsushi/tagbar'
let g:tagbar_width=35
let g:tagbar_autofocus=1
let g:tagbar_left = 1
nmap <F3> :TagbarToggle<CR>

"ctrap-------------------------------------------------------
Plugin 'ctrlpvim/ctrlp.vim'
" 打开ctrlp搜索
let g:ctrlp_map = '<leader>ff'
let g:ctrlp_cmd = 'CtrlP'
" 相当于mru功能，show recently opened files
map <leader>fp :CtrlPMRU<CR>
"set wildignore+=*/tmp/*,*.so,*.swp,*.zip     " MacOSX/Linux"
let g:ctrlp_custom_ignore = {
    \ 'dir':  '\v[\/]\.(git|hg|svn|rvm)$',
    \ 'file': '\v\.(exe|so|dll|zip|tar|tar.gz)$',
    \ }
"\ 'link': 'SOME_BAD_SYMBOLIC_LINKS',
let g:ctrlp_working_path_mode=0
let g:ctrlp_match_window_bottom=1
let g:ctrlp_max_height=15
let g:ctrlp_match_window_reversed=0
let g:ctrlp_mruf_max=500
let g:ctrlp_follow_symlinks=1

" 其他插件---------------------------------------------------------------
" syntax check 
 Plugin 'w0rp/ale'
" 显示状态栏+不需要高亮行
 let g:ale_sign_column_always = 1
 let g:ale_set_highlights = 0

" 错误和警告标志
 let g:ale_sign_error = 'x'
 let g:ale_sign_warning = '!'

" 文件保存时，显示警告
 let g:ale_lint_on_text_changed = 'never'
 let g:ale_lint_on_enter = 0

" syntax check 
 let g:ale_linters = {
 \   'c++': ['clang'],
 \   'c': ['clang'],
 \   'python': ['pylint'],
 \}

" plugin cppSyntaxCheck
 let g:include_path=":../include:./include:./tinyxml"
 let g:compile_flag="-D_LINUX_"
 let g:cpp_compiler="/usr/bin/g++"
 let g:enable_warning=1
 let g:cpp_compiler="LANG=C g++ -Wall"

" languagetool
let g:languagetool_jar='/Users/wukong/.vim/LanguageTool-4.5/languagetool-commandline.jar'

"  Improved C++ STL syntax highlighting
Plugin 'STL-improved'

" recommend fetch it from https://github.com/tczengming/autoload_cscope.vim.git which support c and cpp
Plugin 'tczengming/autoload_cscope.vim'

Plugin 'CmdlineComplete'
Plugin 'xptemplate'

"  Ultimate auto completion system for Vim
Plugin 'neocomplcache'

Plugin 'genutils'
" Plugin 'lookupfile'

" Fast file navigation
Plugin 'wincent/Command-T'

" Preview the definition of variables or functions in a preview window
Plugin 'autopreview'
" Under linux need exec 'dos2unix ~/.vim/bundle/QFixToggle/plugin/qfixtoggle.vim'
Plugin 'Toggle'

Plugin 'Color-Sampler-Pack'
Plugin 'txt.vim'
Plugin 'mru.vim'
Plugin 'YankRing.vim'
Plugin 'tpope/vim-surround.git'
Plugin 'DoxygenToolkit.vim'
Plugin 'tczengming/headerGatesAdd.vim'
" Plugin 'ShowMarks'

" Latex 
" REQUIRED. This makes vim invoke Latex-Suite when you open a tex file.
 filetype plugin on
 
 " IMPORTANT: win32 users will need to have 'shellslash' set so that latex
 " can be called correctly.
 set shellslash
 
 " IMPORTANT: grep will sometimes skip displaying the file name if you
 " search in a singe file. This will confuse Latex-Suite. Set your grep
 " program to always generate a file-name.
 set grepprg=grep\ -nH\ $*
 
 " OPTIONAL: This enables automatic indentation as you type.
 filetype indent on
 
 " OPTIONAL: Starting with Vim 7, the filetype of empty .tex files defaults to
 " 'plaintex' instead of 'tex', which results in vim-latex not being loaded.
 " The following changes the default filetype back to 'tex':
 let g:tex_flavor='latex'
 " this is mostly a matter of taste. but LaTeX looks good with just a bit
 " of indentation.
 set sw=2
 
 " TIP: if you write your \label's as \label{fig:something}, then if you
 " type in \ref{fig: and press you will automatically cycle through
 " all the figure labels. Very useful!
 set iskeyword+=:

autocmd Filetype tex setl updatetime=1
let g:livepreview_previewer = 'open -a PDFExpert'






" All of your Plugins must be added before the following line
call vundle#end()            " required
filetype plugin indent on    " required
