# $INPUT='echo ""$?""'
echo -n "echo hi" | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=tmp_valgrind-out.txt ./minishell 2>/dev/null >/dev/null
			# Get the number of bytes lost
			definitely_lost=$(cat tmp_valgrind-out.txt | grep "definitely lost:" | awk 'END{print $4}')
			possibly_lost=$(cat tmp_valgrind-out.txt | grep "possibly lost:" | awk 'END{print $4}')
			indirectly_lost=$(cat tmp_valgrind-out.txt | grep "indirectly lost:" | awk 'END{print $4}')
			all_blocks_freed=$(cat tmp_valgrind-out.txt | grep "All heap blocks were freed -- no leaks are possible")
			# echo "$definitely_lost"
			# echo "$possibly_lost"
			# echo "$indirectly_lost"
			# Check if any bytes were lost
			if [ "$definitely_lost" != "0" ] || [ "$possibly_lost" != "0" ] || [ "$indirectly_lost" != "0" ] && [[ -z "$all_blocks_freed" ]];
			then
				echo -ne "❌ " "$definitely_lost" "$possibly_lost" "$indirectly_lost"
				((LEAKS++))
			else
				echo -ne "✅ "
			fi