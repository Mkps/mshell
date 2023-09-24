#!/bin/bash

# Initialize total counts
total_ok_count=0
total_test_count=0

# Function to run a test
run_test() {
    local test_name="$1"
    local input="$2"

    echo "$test_name"
    test_output=$(./mtest.sh "$input")
    ok_count=$(echo "$test_output" | grep -o "\[OK\]" | wc -l)
    
    # Count lines in input as the number of test lines
    test_lines=$(echo -n "$input" | wc -l)
    total_test_count=$((total_test_count + 1))
    
    echo "$test_output"
	if [ $ok_count -eq 3 ]
	then
    	total_ok_count=$((total_ok_count + 1))
	fi
}

# Run each test
# Syntax and assign tests
test_syntax() {
run_test "test 1: enter"""
run_test "test 2: spaces" "               "
run_test "test 3: tabs" "					"
run_test "test 5: empty comment" "#"
run_test "test 5: comment" "#echo test"
run_test "test 6: dummy true" ":"
run_test "test 7: dummy false" "!"
run_test "test 8: io_trunc" ">"
run_test "test 9: io_append" ">>"
run_test "test 10: io_input" "<"
run_test "test 11: io_heredoc" "<<"
run_test "test 12: io_rw" "<>"
run_test "test 13: syntax error: >>>>>" ">>>>>>>"
run_test "test 14: syntax error: >>>>>>>>>>>>" ">>>>>>>>>>>>>>"
run_test "test 15: syntax error: <<<<<" "<<<<<"
run_test "test 16: syntax error: <<<<<<<<<<<<<<" "<<<<<<<<<<<<<<"
run_test "test 17: > > > >" "> > > >"
run_test "test 18: >> >> >> >>" ">> >> >> >>"
run_test "test 19: >>>> >> >>" ">>>> >> >>"
run_test "test 20: /" "/"
run_test "test 21: //" "//"
run_test "test 22: /." "/."
run_test "test 23: /./../../../../.." "/./../../../../.."
run_test "test 24: ///////" "///////"
run_test "test 25: \\" "\\"
run_test "test 26: \\\\" "\\\\"
run_test "test 27: \\\\\\\\" "\\\\\\\\"
run_test "test 28: -" " -"
run_test "test 29: |" "|"
run_test "test 30: | hola" "| hola"
run_test "test 31: | | |" "| | |"
run_test "test 32: ||" "||"
run_test "test 33: |||||" "|||||"
run_test "test 34: ||||||||||||" "||||||||||||||"
run_test "test 35: >>|><" ">>|><"
run_test "test 36: &&" "&&"
run_test "test 37: &&&&&" "&&&&&"
run_test "test 38: &&&&&&&&&&&&&&" "&&&&&&&&&&&&&&"
run_test "test 39: ;;" ";;"
run_test "test 40: ;;;;;" ";;;;;"
run_test "test 41: ;;;;;;;;;;;;;;" ";;;;;;;;;;;;;;"
run_test "test 42: ()" "()"
run_test "test 43: ( ( ) )" "( ( ) )"
run_test "test 44: ( ( ( ( ) ) ) ) " "( ( ( ( ) ) ) )"
run_test 'test 45: ""' '""'
run_test 'test 46: "hola"' '"hola"'
run_test "test 47: 'hola'" "'hola'"
run_test "test 48: ''" "''"
run_test "test 49: *" "*"
run_test "test 50: */*" "*/*"
run_test "test 51: */*" "*/*"
run_test "test 52: .." ".."
run_test "test 53: ~" "~"
run_test "test 54: ABC=hola" "ABC=hola"
run_test "test 55: 4ABC=hola" "4ABC=hola"
run_test "test 56: a" "a"
run_test "test 57: hola que tal" "hola que tal"
run_test "test 58: Makefile" "Makefile"
}
# Echo tests
test_echo() {
run_test "test 59: echo" "echo"
run_test "test 60: echo -n" "echo -n"
run_test "test 61: echo Hola" "echo Hola"
run_test "test 62: echoHola" "echoHola"
run_test "test 63: echo-nHola" "echo-nHola"
run_test "test 64: echo -n Hola" "echo -n Hola"
run_test 'test 65: echo "-n" Hola' 'echo "-n" Hola'
run_test "test 66: echo -nHola" "echo -nHola"
run_test "test 67: echo Hola -n" "echo Hola -n"
run_test "test 68: echo Hola Que Tal" "echo Hola Que Tal"
run_test "test 69: echo         Hola" "echo         Hola"
run_test "test 70: echo    Hola     Que    Tal" "echo    Hola     Que    Tal"
run_test 'test 71: echo      \n hola' 'echo      \n hola'
run_test 'test 72: echo "         " | cat -e' 'echo "         " | cat -e'
run_test 'test 73: echo           | cat -e' 'echo           | cat -e'
run_test "test 74: \"\"''echo hola\"\"'''' que\"\"'' tal\"\"''" "\"\"''echo hola\"\"'''' que\"\"'' tal\"\"''"
run_test "test 75: echo -n -n" "echo -n -n"
run_test "test 76: echo -n -n Hola Que" "echo -n -n Hola Que"
run_test "test 77: echo -p" "echo -p"
run_test "test 78: echo -nnnnn" "echo -nnnnn"
run_test "test 79: echo -n -nnn -nnnn" "echo -n -nnn -nnnn"
run_test "test 80: echo -n-nnn -nnnn" "echo -n-nnn -nnnn"
run_test "test 81: echo -n -nnn hola -nnnn" "echo -n -nnn hola -nnnn"
run_test "test 82: echo -n -nnn-nnnn" "echo -n -nnn-nnnn"
run_test "test 83: echo --------n" "echo --------n"
run_test "test 84: echo -nnn --------n" "echo -nnn --------n"
run_test "test 85: echo -nnn -----nn---nnnn" "echo -nnn -----nn---nnnn"
run_test "test 86: echo -nnn --------nnnn" "echo -nnn --------nnnn"
run_test "test 87: echo $" "echo $"
run_test "test 88: echo $?" "echo $?"
run_test "test 89: echo $?$" "echo $?$"
run_test "test 90: \$? | echo \$? | echo \$?" "\$? | echo \$? | echo \$?"
run_test "test 91: \$:\$= | cat -e" "\$:\$= | cat -e"
run_test "test 92: \" \$ \" | cat -e" "\" \$ \" | cat -e"
run_test "test 93: ' \$ ' | cat -e" "' \$ ' | cat -e"
run_test "test 94: \$HOME" "\$HOME"
run_test "test 95: \$HOME" "\$HOME"
run_test "test 96: echo my shit terminal is [\$TERM]" "echo my shit terminal is [\$TERM]"
run_test "test 97: echo my shit terminal is [\$TERM4" "echo my shit terminal is [\$TERM4"
run_test "test 98: echo my shit terminal is [\$TERM4]" "echo my shit terminal is [\$TERM4]"
run_test "test 99: echo \$UID" "echo \$UID"
run_test "test 100: echo \$HOME9" "echo \$HOME9"
run_test "test 101: echo \$9HOME" "echo \$9HOME"
run_test "test 102: echo \$HOME%" "echo \$HOME%"
run_test "test 103: echo \$UID\$HOME" "echo \$UID\$HOME"
run_test "test 104: echo Le path de mon HOME est \$HOME" "echo Le path de mon HOME est \$HOME"
run_test "test 105: echo \$USER\$var\$USER\$USERtest\$USER" "echo \$USER\$var\$USER\$USERtest\$USER"
run_test "test 106: echo \$hola*" "echo \$hola*"
run_test "test 107: echo -nnnn \$hola" "echo -nnnn \$hola"
run_test "test 108: echo > <" "echo > <"
run_test "test 109: echo | |" "echo | |"
run_test "test 110: EechoE" "EechoE"
run_test "test 111: .echo." ".echo."
run_test "test 112: >echo>" ">echo>"
run_test "test 113: <echo<" "<echo<"
run_test "test 114: >>echo>>" ">>echo>>"
run_test "test 115: |echo|" "|echo|"
run_test "test 116: |echo -n hola" "|echo -n hola"
run_test "test 117: echo *" "echo *"
run_test "test 118: echo '*'" "echo '*'"
run_test "test 119: echo D*" "echo D*"
run_test "test 120: echo *Z" "echo *Z"
run_test "test 121: echo *t hola" "echo *t hola"
run_test "test 122: echo *t" "echo *t"
run_test "test 123: echo $*" "echo $*"
run_test "test 124: echo hola*hola *" "echo hola*hola *"
run_test "test 125: echo $hola*" "echo $hola*"
run_test "test 126: echo $HOME*" "echo $HOME*"
}
# Var tests
test_var() {
run_test "test 127: \$" "'\$'"
run_test "test 128: \"\$\"" "\"'\$'\""
run_test "test 129: '\$''HOME'" "'\$\$HOME'"
run_test "test 130: '\$''HOME'" "'\$\$HOME'"
run_test "test 131: \"\$\"\"HOME\"" "\"'\$'\$HOME\""
run_test "test 132: '\$'\"HOME\"" "'\$\$HOME'"
run_test "test 133: \"'\$'\$HOME'\"" "\"'\$'\$HOME'\""
run_test "test 134: \"\"\$HOME" "\"\"\$HOME"
run_test "test 135: \"\" \$HOME" "\"\" \$HOME"
run_test "test 136: '\$'HO\"ME\"" "'\$\$HO\"ME\"'"
run_test "test 137: '\$'HO''ME'" "'\$\$HO''ME''"
run_test "test 138: '\$'HOME" "'\$\$HOME'"
run_test "test 139: \"\"\$HOME" "\"\"\$HOME"
run_test "test 140: '\$'HOME" "'\$\$HOME'"
run_test "test 141: '\$'=HOME" "'\$\$'=HOME'"
run_test "test 142: \"\"\$HOLA" "\"\"\$HOLA\""
run_test "test 143: echo \$'HOLA'" "echo \$'HOLA'"
run_test "test 144: echo \$DONTEXIST Hola" "echo \$DONTEXIST Hola"
run_test "test 145: echo \"hola\"" "echo \"hola\""
run_test "test 146: echo 'hola'" "echo 'hola'"
run_test "test 147: echo ''hola''" "echo ''hola''"
run_test "test 148: echo ''h'o'la''" "echo ''h'o'la''"
run_test "test 149: echo \"''h'o'la''\"" "echo \"''h'o'la''\""
run_test "test 150: echo \"'h'o'la'\"" "echo \"'h'o'la'\""
run_test "test 151: echo\"'hola'\"" "echo\"'hola'\""
run_test "test 152: echo \"'hola'\"" "echo \"'hola'\""
run_test "test 153: echo '\"hola\"'" "echo '\"hola\"'"
run_test "test 154: echo '''ho\"''''l\"a'''" "echo '''ho\"''''l\"a'''"
run_test "test 155: echo hola\"\"\"\"\"\"\"\"\"\"\"" "echo hola\"\"\"\"\"\"\"\"\"\"\""
run_test "test 156: echo hola\"''''''''''\"" "echo hola\"''''''''''\""
run_test "test 157: echo hola'''''''''''''" "echo hola'''''''''''''"
run_test "test 158: echo hola'\"\"\"\"\"\"\"\"\"\"'" "echo hola'\"\"\"\"\"\"\"\"\"\"'"
run_test "test 159: e\"cho hola\"" "e\"cho hola\""
run_test "test 160: e'cho hola'" "e'cho hola'"
run_test "test 161: echo \"hola     \" | cat -e" "echo \"hola     \" | cat -e"
run_test "test 162: echo \"\"hola" "echo \"\"hola"
run_test "test 163: echo \"\" hola" "echo \"\" hola"
run_test "test 164: echo \"\"             hola" "echo \"\"             hola"
run_test "test 165: echo \"\"hola" "echo \"\"hola"
run_test "test 166: echo \"\" hola" "echo \"\" hola"
run_test "test 167: echo hola\"\"bonjour" "echo hola\"\"bonjour"
run_test "test 168: \"e\"'c'ho 'b'\"o\"nj\"o\"'u'r" "\"e\"'c'ho 'b'\"o\"nj\"o\"'u'r"
run_test "test 169: \"\"e\"'c'ho 'b'\"o\"nj\"o\"'u'r\"" "\"\"e\"'c'ho 'b'\"o\"nj\"o\"'u'r\""
run_test "test 170: echo \"\$DONTEXIST\"Makefile" "echo \"\$DONTEXIST\"Makefile"
run_test "test 171: echo \"\$DONTEXIST\"\"Makefile\"" "echo \"\$DONTEXIST\"\"Makefile\""
run_test "test 172: \"\$DONTEXIST\" \"Makefile\"" "\"\$DONTEXIST\" \"Makefile\""
run_test "test 173: \$?" "\$?"
run_test "test 174: \$?\$?" "\$?\$?"
run_test "test 175: ?\$HOME" "?\$HOME"
run_test "test 176: \$" "\$"
run_test "test 177: \$HOME" "\$HOME"
run_test "test 178: \$HOMEdskjhfkdshfsd" "\$HOMEdskjhfkdshfsd"
run_test "test 179: \"\$HOMEdskjhfkdshfsd\"" "\"\$HOMEdskjhfkdshfsd\""
run_test "test 180: '\$HOMEdskjhfkdshfsd'" "'\$HOMEdskjhfkdshfsd'"
run_test "test 181: \$DONTEXIST" "\$DONTEXIST"
run_test "test 182: \$LESS\$VAR" "\$LESS\$VAR"
}
# env / export / tests
test_env() {
run_test "test 183: env" "env"
run_test "test 184: env hola" "env hola"
run_test "test 185: env hola que tal" "env hola que tal"
run_test "test 186: env env" "env env"
run_test "test 187: env env env env env" "env env env env env"
run_test "test 188: env ls" "env ls"
run_test "test 189: env ./Makefile" "env ./Makefile"
run_test "test 190: export HOLA=bonjour ; env" "export HOLA=bonjour ; env"
run_test "test 191: export       HOLA=bonjour ;  env" "export       HOLA=bonjour ;  env"
run_test "test 192: export" "export"
run_test "test 193: export Hola ; export" "export Hola ; export"
run_test "test 194: export Hola9hey ; export" "export Hola9hey ; export"
run_test "test 195: export \$DONTEXIST" "export \$DONTEXIST"
run_test "test 196: export | grep \"HOME\"" "export | grep \"HOME\""
run_test "test 197: export \"\"" "export \"\""
run_test "test 198: export =" "export ="
run_test "test 199: export %" "export %"
run_test "test 200: export \$?" "export \$?"
run_test "test 201: export ?=2" "export ?=2"
run_test "test 202: export 9HOLA=" "export 9HOLA="
run_test "test 203: export HOLA9=bonjour ; env" "export HOLA9=bonjour ; env"
run_test "test 204: export _HOLA=bonjour ; env" "export _HOLA=bonjour ; env"
run_test "test 205: export ___HOLA=bonjour ; env" "export ___HOLA=bonjour ; env"
run_test "test 206: export _HO_LA_=bonjour ; env" "export _HO_LA_=bonjour ; env"
run_test "test 207: export HOL@=bonjour" "export HOL@=bonjour"
run_test "test 208: export HOL\\~A=bonjour" "export HOL\\~A=bonjour"
run_test "test 209: export -HOLA=bonjour" "export -HOLA=bonjour"
run_test "test 210: export --HOLA=bonjour" "export --HOLA=bonjour"
run_test "test 211: export HOLA-=bonjour" "export HOLA-=bonjour"
run_test "test 212: export HO-LA=bonjour" "export HO-LA=bonjour"
run_test "test 213: export HOL.A=bonjour" "export HOL.A=bonjour"
run_test "test 214: export HOL\\\$A=bonjour" "export HOL\\\$A=bonjour"
run_test "test 215: export HO\\\\LA=bonjour" "export HO\\\\LA=bonjour"
run_test "test 216: export HOL}A=bonjour" "export HOL}A=bonjour"
run_test "test 217: export HOL{A=bonjour" "export HOL{A=bonjour"
run_test "test 218: export HO*LA=bonjour" "export HO*LA=bonjour"
run_test "test 219: export HO#LA=bonjour" "export HO#LA=bonjour"
run_test "test 220: export HO@LA=bonjour" "export HO@LA=bonjour"
run_test "test 221: export HO!LA=bonjour" "export HO!LA=bonjour"
run_test "test 222: export HO$?LA=bonjour ; env" "export HO$?LA=bonjour ; env"
run_test "test 223: export +HOLA=bonjour" "export +HOLA=bonjour"
run_test "test 224: export HOL+A=bonjour" "export HOL+A=bonjour"
run_test "test 225: export HOLA+=bonjour ; env" "export HOLA+=bonjour ; env"
run_test "test 226: export HOLA=bonjour ; export HOLA+=bonjour ; env" "export HOLA=bonjour ; export HOLA+=bonjour ; env"
run_test "test 227: exportHOLA=bonjour ; env" "exportHOLA=bonjour ; env"
run_test "test 228: export HOLA =bonjour" "export HOLA =bonjour"
run_test "test 229: export HOLA = bonjour" "export HOLA = bonjour"
run_test "test 230: export HOLA=bon jour ; env" "export HOLA=bon jour ; env"
run_test "test 231: export HOLA= bonjour ; env" "export HOLA= bonjour ; env"
run_test "test 232: export HOLA=bonsoir ; export HOLA=bonretour ; export HOLA=bonjour ; env" "export HOLA=bonsoir ; export HOLA=bonretour ; export HOLA=bonjour ; env"
run_test "test 233: export HOLA=\$HOME ; env" "export HOLA=\$HOME ; env"
run_test "test 234: export HOLA=bonjour\$HOME ; env" "export HOLA=bonjour\$HOME ; env"
run_test "test 235: export HOLA=\$HOMEbonjour ; env" "export HOLA=\$HOMEbonjour ; env"
run_test "test 236: export HOLA=bon\$jour ; env" "export HOLA=bon\$jour ; env"
run_test "test 237: export HOLA=bon\\jour ; env" "export HOLA=bon\\jour ; env"
run_test "test 238: export HOLA=bon\\\\jour ; env" "export HOLA=bon\\\\jour ; env"
run_test "test 239: export HOLA=bon(jour" "export HOLA=bon(jour"
run_test "test 240: export HOLA=bon()jour" "export HOLA=bon()jour"
run_test "test 241: export HOLA=bon&jour" "export HOLA=bon&jour"
run_test "test 242: export HOLA=bon@jour ; env" "export HOLA=bon@jour ; env"
run_test "test 243: export HOLA=bon;jour ; env" "export HOLA=bon;jour ; env"
run_test "test 244: export HOLA=bon!jour" "export HOLA=bon!jour"
run_test "test 245: export HOLA=bon\"\"jour\"\" ; env" "export HOLA=bon\"\"jour\"\" ; env"
run_test "test 246: export HOLA\$USER=bonjour ; env" "export HOLA\$USER=bonjour ; env"
run_test "test 247: export HOLA=bonjour=casse-toi ; echo \$HOLA" "export HOLA=bonjour=casse-toi ; echo \$HOLA"
run_test "test 248: export \"\"HOLA=bonjour\"\"=casse-toi ; echo \$HOLA" "export \"\"HOLA=bonjour\"\"=casse-toi ; echo \$HOLA"
run_test "test 249: export HOLA=bonjour ; export BYE=casse-toi ; echo \$HOLA et \$BYE" "export HOLA=bonjour ; export BYE=casse-toi ; echo \$HOLA et \$BYE"
run_test "test 250: export HOLA=bonjour BYE=casse-toi ; echo \$HOLA et \$BYE" "export HOLA=bonjour BYE=casse-toi ; echo \$HOLA et \$BYE"
run_test "test 251: export \$HOLA=bonjour ; env" "export \$HOLA=bonjour ; env"
# run_test "test 252: export HOLA=\"\"bonjour      \"\"  ; echo \$HOLA | cat -e" "export HOLA=\"\"bonjour      \"\"  ; echo \$HOLA | cat -e"
# run_test "test 253: export HOLA=\"\"   -n bonjour   \"\" ; echo \$HOLA" "export HOLA=\"\"   -n bonjour   \"\" ; echo \$HOLA"
# run_test "test 254: export HOLA=\"\"bonjour   \"\"/ ; echo \$HOLA" "export HOLA=\"\"bonjour   \"\"/ ; echo \$HOLA"
# run_test "test 255: export HOLA='\"\"' ; echo \"\" \$HOLA \"\" | cat -e" "export HOLA='\"\"' ; echo \"\" \$HOLA \"\" | cat -e"
# run_test "test 256: export HOLA=at ; c\$HOLA Makefile" "export HOLA=at ; c\$HOLA Makefile"
# run_test "test 257: export \"\"\"\" HOLA=bonjour ; env" "export \"\"\"\" HOLA=bonjour ; env"
# run_test "test 258: export HOLA=\"\"cat Makefile | grep NAME\"\"   ; echo \$HOLA" "export HOLA=\"\"cat Makefile | grep NAME\"\"   ; echo \$HOLA"
# run_test "test 259: export HOLA=hey ; echo \$HOLA\$HOLA\$HOLA=hey\$HOLA" "export HOLA=hey ; echo \$HOLA\$HOLA\$HOLA=hey\$HOLA"
# run_test "test 260: export HOLA=\"\"  bonjour  hey  \"\"  ; echo \$HOLA | cat -e" "export HOLA=\"\"  bonjour  hey  \"\"  ; echo \$HOLA | cat -e"
# run_test "test 261: export HOLA=\"\"  bonjour  hey  \"\" ; echo \$HOLA | cat -e" "export HOLA=\"\"  bonjour  hey  \"\" ; echo \$HOLA | cat -e"
}

# Start here: comment out the tests you dont want.
test_syntax
test_echo
test_var
# test_env
# Print total counts
# echo "Total OK count: $total_ok_count / $total_test_count"
echo "Total OK count: $total_ok_count / $total_test_count "
