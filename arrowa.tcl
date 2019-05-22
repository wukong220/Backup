mol delete all
#set Npname "Np.7880000.lammpstrj"
#set pbfile "pB.7880000.lammpstrj"
set filename "Wall.lammpstrj"
set open_file "Velocity_g.out"		;#scale:15
#set filename "Blank.lammpstrj"
#set open_file "Velocity_aline.out" ;#scale:3
set lp_num  40000   ;#循环次数
set scale  20	;#箭头长度放大
set color   "red"
set color_1 "blue"
set sel "type 1"
set step 1
set cradius 0.32	
set dradius 0.30
set start   0
set end     4000
set nf [expr $end-$start]
#set dr 18.343
#set rr_num 240
set PI     3.1415926535897932 

set arrow_dat [open $open_file r]
gets $arrow_dat line
for {set i 0} {$i < $lp_num } {incr i} {
    gets $arrow_dat line
    set origin_x($i) [lindex $line 1]
    set origin_y($i) [lindex $line 2]
    set orien_x($i)  [lindex $line 3]
    set orien_y($i)  [lindex $line 4]
}
close $arrow_dat

# #######frame######
set molid [mol new $filename]	;#载入wall分子
#set pbid [mol new $pbfile]
#set wallid [mol new $wallfile]
set filled yes
graphics $molid delete all
graphics $molid color $color

for {set loop 0} { $loop < $lp_num } { incr loop  } {
		set now_x  [expr 0.0+$origin_x($loop)]
        set now_y  [expr 0.0+$origin_y($loop)] 
        set now_z  1.0
        set next_x [expr 0.0+$orien_x($loop)]
        set next_y [expr 0.0+$orien_y($loop)] 
        set next_z 1.0
        set next_x [expr $now_x+$scale*$next_x];
        set next_y [expr $now_y+$scale*$next_y];
        set next_z 1.0

        set start {}
		set end   {}
		lappend start $now_x
		lappend start $now_y
		lappend start $now_z
		lappend end   $next_x
		lappend end   $next_y
		lappend end   $next_z
		set middle [vecadd $start [vecscale 0.6 [vecsub $end $start]]]
        ########################### 
		#puts "$loop $next_x $now_x"
		

        set mid_x  [lindex $middle 0]
        set mid_y  [lindex $middle 1]
        set length [expr abs(sin(30.0*$PI/180.0)*[vecdist $end $middle])]
		
		if { ($now_x==$next_x) || ($now_y==$next_y) } {
		    set k 1.0;
		} else {
			set k [expr ($now_y-$next_y)/($now_x-$next_x)];#斜率
		}
        set re_k   [expr 0.0-1.0/$k]
        set cut    [expr $mid_y-$re_k*$mid_x]
        set len    [expr $length/sqrt(1.0+$re_k*$re_k)]
        set x2_1   [expr $mid_x-$len]
        set x2_2   [expr $len+$mid_x]
        set middle_side_1 [list $x2_1 [expr $re_k*$x2_1+$cut] 1.0]
        set middle_side_2 [list $x2_2 [expr $re_k*$x2_2+$cut] 1.0]
        #############################
     	graphics $molid line $start $end width 2 style solid	;#画流线
        graphics $molid line $middle_side_1 $end width 1 style solid	;#画箭头
        graphics $molid line $middle_side_2 $end width 1 style solid 
}


