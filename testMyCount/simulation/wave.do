onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /testbench/SYSCLK
add wave -noupdate /testbench/NSYSRESET
add wave -noupdate /testbench/NIMPULSE
add wave -noupdate /testbench/PIMPULSE
add wave -noupdate /testbench/myCount_0/myRevCount_0/nreset
add wave -noupdate /testbench/myCount_0/myRevCount_0/nEnable
add wave -noupdate /testbench/myCount_0/myRevCount_0/pEnable
add wave -noupdate /testbench/myCount_0/myRevCount_0/count
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {243 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 348
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {99999631 ps} {100000020 ps}
