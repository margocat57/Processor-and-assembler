.PHONY: all clean 
# эти цели не являются файлами выполняй их даже если соотв файлы существуют
all: assembler
# когда запускаем make без цели, то выполняем первую цель после all, то есть записи make stack make all и make эквивалентны

COMP=clang++

CFLAGS_WITH_DEBUG = -ggdb3 -std=c++20 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla -Wno-c++11-extensions
# CFLAGS = -D _DEBUG

stack_for_calcul/hash.o: stack_for_calcul/hash.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

stack_for_calcul/log.o: stack_for_calcul/log.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

stack_for_calcul/my_assert.o: stack_for_calcul/my_assert.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

stack_for_calcul/stack_func.o: stack_for_calcul/stack_func.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

assembler_task/main_assemb.o: assembler_task/main_assemb.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

assembler_task/file_work.o: assembler_task/file_work.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

assembler_task/parsing_str.o: assembler_task/parsing_str.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

# Правило для файлов в корневой папке
%.o: %.cpp
	$(COMP) -c $< -o $@ $(CFLAGS_WITH_DEBUG)

assembler: assembler_task/main_assemb.o assembler_task/file_work.o assembler_task/parsing_str.o
	$(COMP) -o $@ $^

processor: main_proc.o parse_asm_from_file.o assembler_task/file_work.o calcul.o stack_for_calcul/hash.o stack_for_calcul/log.o stack_for_calcul/my_assert.o stack_for_calcul/stack_func.o
	$(COMP) -o $@ $^

#calc: main.o calcul.o file_work.o parsing_str.o stack_for_calcul/hash.o stack_for_calcul/log.o stack_for_calcul/my_assert.o stack_for_calcul/stack_func.o
#	$(COMP) -o $@ $^

clean:
	rm -f assembler_task/*.o stack_for_calcul/*.o *.o assembler processor
# *.p